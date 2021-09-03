#include "si1145.h"

void writeValue(uint8_t reg, uint8_t val)
{
	HAL_I2C_Mem_Write(&hi2c1, SI1145_PARAM_ADDR, reg, I2C_MEMADD_SIZE_8BIT, &val, 1, 100);
}

uint8_t readValue(uint8_t reg)
{
	uint8_t data;
	HAL_I2C_Mem_Read(&hi2c1, SI1145_PARAM_ADDR, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
	return data;
}

uint8_t writeParam(uint8_t val1, uint8_t val2)
{
	writeValue(SI1145_REG_PARAMWR, val1);
	writeValue(SI1145_REG_COMMAND, val2 | SI1145_PARAM_SET);
	return readValue(SI1145_REG_PARAMRD);
}

uint8_t readParam(uint8_t val)
{
	writeValue(SI1145_REG_COMMAND, val | SI1145_PARAM_QUERY);
	return readValue(SI1145_REG_PARAMRD);
}

uint16_t readMeasurment(uint8_t val)
{
	uint16_t data;
	uint8_t readbuffer[2];
	HAL_I2C_Mem_Read(&hi2c1, SI1145_PARAM_ADDR, val, I2C_MEMADD_SIZE_16BIT, readbuffer, 2, 100);
	data = ( readbuffer[0] | ( (uint16_t) (readbuffer[1]) << 8 ) );
	return measurment;
}

void si1145_reset()
{
	writeValue(SI1145_REG_MEASRATE0, 0x00);
	writeValue(SI1145_REG_MEASRATE1, 0x00);
	writeValue(SI1145_REG_IRQEN, 0x00);
	writeValue(SI1145_REG_IRQMODE1, 0x00);
	writeValue(SI1145_REG_IRQMODE2, 0x00);
	writeValue(SI1145_REG_INTCFG, 0x00);
	writeValue(SI1145_REG_IRQSTAT, 0xFF);

	writeValue(SI1145_REG_COMMAND, SI1145_RESET);

	HAL_Delay(100);

	writeValue(SI1145_REG_HWKEY, 0x17);

	HAL_Delay(100);
}

void si1145_config()
{
	  /* Apply recommended UV coefficients values */
	  writeValue(SI1145_REG_UCOEFF0, 0x29);
	  writeValue(SI1145_REG_UCOEFF1, 0x89);
	  writeValue(SI1145_REG_UCOEFF2, 0x82);
	  writeValue(SI1145_REG_UCOEFF3, 0x00);

	  /* Initialise UV sensor */
	  writeParam(SI1145_PARAM_CHLIST, SI1145_PARAM_CHLIST_ENUV | SI1145_PARAM_CHLIST_ENALSIR |SI1145_PARAM_CHLIST_ENALSVIS | SI1145_PARAM_CHLIST_ENPS1);

	  /* Enable interrupts on every sample event */
	  writeValue(SI1145_REG_INTCFG, SI1145_REG_INTCFG_INTOE);
	  writeValue(SI1145_REG_IRQEN, SI1145_REG_IRQEN_ALSEVERYSAMPLE);

	  /* Apply LED current for proximity LED #1 */
	  writeValue(SI1145_REG_PSLED12, 0x03);
	  writeParam(SI1145_PARAM_PS1ADCMUX, SI1145_PARAM_ADCMUX_LARGEIR);
	  
	  /* Use proximity LED #1 for sensor */
	  writeParam(SI1145_PARAM_PSLED12SEL, SI1145_PARAM_PSLED12SEL_PS1LED1);

	  /* Set ADC clock divider to fastest (value 0 according to docs) */
	  writeParam(SI1145_PARAM_PSADCGAIN, 0);

	  /* Set recovery peroid the ADC takes before making a PS measurment to 511 */
	  writeParam(SI1145_PARAM_PSADCOUNTER, SI1145_PARAM_ADCCOUNTER_511CLK);

	  /* Proximity sensor mode set to high range */
	  writeParam(SI1145_PARAM_PSADCMISC, SI1145_PARAM_PSADCMISC_RANGE | SI1145_PARAM_PSADCMISC_PSMODE);

      /* Use small IR photodiode (0x00) */
	  writeParam(SI1145_PARAM_ALSIRADCMUX, SI1145_PARAM_ADCMUX_SMALLIR);

	  /* Set ADC clock divider to fastest */
	  writeParam(SI1145_PARAM_ALSIRADCGAIN, 0);

	  /* Set recovery peroid the ADC takes before making a IR measurment to 511 */
	  writeParam(SI1145_PARAM_ALSIRADCOUNTER, SI1145_PARAM_ADCCOUNTER_511CLK);

	  /* IR sensor mode set to high range */
	  writeParam(SI1145_PARAM_ALSIRADCMISC, SI1145_PARAM_ALSIRADCMISC_RANGE);

	  /* Set ADC clock divider to fastest */
	  writeParam(SI1145_PARAM_ALSVISADCGAIN, 0);

	  /* Set recovery peroid the ADC takes before making a visible light measurment to 511 */
	  writeParam(SI1145_PARAM_ALSVISADCOUNTER, SI1145_PARAM_ADCCOUNTER_511CLK);

	   /* IR sensor mode set to high range */
	  writeParam(SI1145_PARAM_ALSVISADCMISC, SI1145_PARAM_ALSVISADCMISC_VISRANGE);

	  /* Rate which represents time interval at which sensor wakes up to perform measurment */
	  writeValue(SI1145_REG_MEASRATE0, 0xFF);

	  /* Autonomous ALS and PS measurment mode in a loop */
      writeValue(SI1145_REG_COMMAND, SI1145_PSALS_AUTO);

      HAL_Delay(100);
}

/* Read UV measurements 
   Divided by 100 according to docs */

float readUltraViolet(void)
{
	float UV = readMeasurment(0x2C) / 100.0f;
    return UV;
}

/* Read visible light measurements */

uint16_t readVisible(void)
{
	uint16_t Visible = readMeasurment(0x22);
    return Visible;
}

/* Read IR measurements */

uint16_t readInfrared(void)
{
	uint16_t Infrared = readMeasurment(0x24);
    return Infrared;
}

/* Read proximity measurements */

uint16_t readProximity(void)
{
	uint16_t Proximity = readMeasurment(0x26);
    return Proximity;
}
