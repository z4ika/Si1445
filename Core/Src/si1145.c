#include "si1145.h"

uint16_t data = 0;

void writeValue(uint8_t reg, uint8_t val)
{
	HAL_I2C_Mem_Write(&hi2c1, (uint8_t*) SI1145_PARAM_ADDR, (uint8_t*) reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*) val, 1, 100);
}

uint8_t readValue(uint8_t reg)
{
	uint8_t value = HAL_I2C_Mem_Read(&hi2c1, (uint8_t*) SI1145_PARAM_ADDR, (uint8_t*) reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
	return value;
}

uint8_t writeParam(uint8_t v1, uint8_t v2)
{
	writeValue(SI1145_REG_PARAMWR, v1);
	writeValue(SI1145_REG_COMMAND, v2 | SI1145_PARAM_SET);
	return readValue(SI1145_REG_PARAMRD);
}

uint16_t readMeasurment(uint8_t v1)
{
	uint16_t measurment = HAL_I2C_Mem_Read(&hi2c1, (uint8_t*) SI1145_PARAM_ADDR, (uint8_t*) v1, I2C_MEMADD_SIZE_16BIT, &data, 2, 100);
	return measurment;
}

float readMeasurmentFloat(uint8_t v1)
{
	float data = 0;
	float measurment = HAL_I2C_Mem_Read(&hi2c1, (uint8_t*) SI1145_PARAM_ADDR, (uint8_t*) v1, I2C_MEMADD_SIZE_16BIT, &data, 2, 100);
	return measurment;
}

void si1145_init_continous()
{
	si1145_reset();

	// UV register default calibration values
	writeValue(SI1145_REG_UCOEFF0, 0x29);
	writeValue(SI1145_REG_UCOEFF1, 0x89);
	writeValue(SI1145_REG_UCOEFF2, 0x82);
	writeValue(SI1145_REG_UCOEFF3, 0x00);

	// Enable interrupts on every sample
	writeValue(SI1145_REG_INTCFG, SI1145_REG_INTCFG_INTOE);
	writeValue(SI1145_REG_IRQEN, 0x05);

	// Pause in between measurments = 31.25 us * rate
    writeValue(SI1145_REG_MEASRATE0, 0x00FF);

    // LED Current equal to 2.24mA
    writeValue(SI1145_REG_PSLED21, 0x03);

    // Proximity sensor 1 enabled with LED1
	writeParam(SI1145_PARAM_PSLED12SEL, SI1145_PARAM_PSLED12SEL_PS1LED1);

	// PS photodioed selected as bigger
	writeParam(SI1145_PARAM_PS1ADCMUX, SI1145_PARAM_ADCMUX_LARGEIR);
	writeParam(SI1145_PARAM_PSADCGAIN, 0);

	writeParam(SI1145_PARAM_ALSIRADCMUX, SI1145_PARAM_ADCMUX_SMALLIR);

	writeParam(SI1145_PARAM_ALSIRADCGAIN, 0);

	writeParam(SI1145_PARAM_ALSIRADCOUNTER, SI1145_PARAM_ADCCOUNTER_511CLK);

	writeParam(SI1145_PARAM_ALSIRADCMISC, SI1145_PARAM_ALSIRADCMISC_RANGE);

	writeParam(SI1145_PARAM_ALSVISADCGAIN, 0);

	writeParam(SI1145_PARAM_ALSVISADCOUNTER, SI1145_PARAM_ADCCOUNTER_511CLK);

	writeParam(SI1145_PARAM_ALSVISADCMISC, SI1145_PARAM_ALSVISADCMISC_VISRANGE);
}

void si1145_config()
{
	  // Enable coefficients for UV measurment
	  writeValue(SI1145_REG_UCOEFF0, 0x29);
	  writeValue(SI1145_REG_UCOEFF1, 0x89);
	  writeValue(SI1145_REG_UCOEFF2, 0x82);
	  writeValue(SI1145_REG_UCOEFF3, 0x00);

	  // UV Sensor init
	  writeParam(SI1145_PARAM_CHLIST, SI1145_PARAM_CHLIST_ENUV | SI1145_PARAM_CHLIST_ENALSIR |SI1145_PARAM_CHLIST_ENALSVIS | SI1145_PARAM_CHLIST_ENPS1);

	  // enable interrupt on every sample
	  writeValue(SI1145_REG_INTCFG, SI1145_REG_INTCFG_INTOE);
	  writeValue(SI1145_REG_IRQEN, SI1145_REG_IRQEN_ALSEVERYSAMPLE);

	  // program LED current
	  writeValue(SI1145_REG_PSLED21, 0x03);
	  writeParam(SI1145_PARAM_PS1ADCMUX, SI1145_PARAM_ADCMUX_LARGEIR);
	  // prox sensor #1 uses LED #1
	  writeParam(SI1145_PARAM_PSLED12SEL, SI1145_PARAM_PSLED12SEL_PS1LED1);
	  // fastest clocks, clock div 1
	  writeParam(SI1145_PARAM_PSADCGAIN, 0);
	  // take 511 clocks to measure
	  writeParam(SI1145_PARAM_PSADCOUNTER, SI1145_PARAM_ADCCOUNTER_511CLK);
	  // in prox mode, high range
	  writeParam(SI1145_PARAM_PSADCMISC, SI1145_PARAM_PSADCMISC_RANGE | SI1145_PARAM_PSADCMISC_PSMODE);

	  writeParam(SI1145_PARAM_ALSIRADCMUX, SI1145_PARAM_ADCMUX_SMALLIR);
	  // fastest clocks, clock div 1
	  writeParam(SI1145_PARAM_ALSIRADCGAIN, 0);
	  // take 511 clocks to measure
	  writeParam(SI1145_PARAM_ALSIRADCOUNTER, SI1145_PARAM_ADCCOUNTER_511CLK);
	  // in high range mode
	  writeParam(SI1145_PARAM_ALSIRADCMISC, SI1145_PARAM_ALSIRADCMISC_RANGE);
	  // fastest clocks, clock div 1
	  writeParam(SI1145_PARAM_ALSVISADCGAIN, 0);
	  // take 511 clocks to measure
	  writeParam(SI1145_PARAM_ALSVISADCOUNTER, SI1145_PARAM_ADCCOUNTER_511CLK);
	  // in high range mode (not normal signal)
	  writeParam(SI1145_PARAM_ALSVISADCMISC, SI1145_PARAM_ALSVISADCMISC_VISRANGE);
	  // Measurement rate setting for auto mode
	  writeValue(SI1145_REG_MEASRATE0, 0xFF);

	  // Starts/Restarts autonomous ALS and PS loop
      writeValue(SI1145_REG_COMMAND, SI1145_PSALS_AUTO);
      HAL_Delay(100);
}

// Reset

void si1145_reset()
{
	writeValue(SI1145_REG_COMMAND, SI1145_RESET);

	HAL_Delay(500);

	writeValue(SI1145_REG_HWKEY, 0x17);

	HAL_Delay(500);
}

// Read UV measurements

float readUltraViolet()
{
	float UV = readMeasurmentFloat(0x2C) / 100.0f;
    return UV;
}

// Read visible light spectrum measurements

uint16_t readVisible(void)
{
	uint16_t Visible = readMeasurment(0x22);
    return Visible;
}

// Read IR measurements

uint16_t readInfrared(void)
{
	uint16_t Infrared = readMeasurment(0x24);
    return Infrared;
}

// Read proximity measurements

uint16_t readProximity(void)
{
	uint16_t Proximity = readMeasurment(0x26);
    return Proximity;
}
