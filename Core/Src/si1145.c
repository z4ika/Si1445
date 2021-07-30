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
	uint16_t measurment = HAL_I2C_Mem_Read(&hi2c1, SI1145_PARAM_ADDR, v1, I2C_MEMADD_SIZE_16BIT, &data, 2, 100);
	return measurment;
}

void si1145_config()
{
	  printf("Init. config");
	  // program LED current
	  writeValue(SI1145_REG_PSLED21, 0x03); // 20mA for LED 1 only
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
}

void si1145_startAuto()
{
	printf("Init. auto measurments");
    // Enable coefficients
	writeValue(SI1145_REG_UCOEFF0, 0x29);
	writeValue(SI1145_REG_UCOEFF1, 0x89);
	writeValue(SI1145_REG_UCOEFF2, 0x82);
	writeValue(SI1145_REG_UCOEFF3, 0x00);

    // Measurement rate setting for auto mode
	writeValue(SI1145_REG_MEASRATE0, 0xFF);

    // UV Sensor init
	writeParam(SI1145_PARAM_CHLIST, SI1145_PARAM_CHLIST_ENUV | SI1145_PARAM_CHLIST_ENALSIR |SI1145_PARAM_CHLIST_ENALSVIS | SI1145_PARAM_CHLIST_ENPS1);

	// enable interrupt on every sample
	writeValue(SI1145_REG_INTCFG, SI1145_REG_INTCFG_INTOE);
	writeValue(SI1145_REG_IRQEN, SI1145_REG_IRQEN_ALSEVERYSAMPLE);

	// measurement rate for auto
	writeValue(SI1145_REG_MEASRATE0, 0xFF); // 255 * 31.25uS = 8ms
    // auto run
	writeValue(SI1145_REG_COMMAND, SI1145_PSALS_AUTO);
}

// Reset

void si1145_reset()
{
	printf("Init. reset");
	writeValue(SI1145_REG_MEASRATE0, 0);
	writeValue(SI1145_REG_MEASRATE1, 0);
	writeValue(SI1145_REG_IRQEN, 0);
	writeValue(SI1145_REG_IRQMODE1, 0);
	writeValue(SI1145_REG_IRQMODE2, 0);
    writeValue(SI1145_REG_INTCFG, 0);
    writeValue(SI1145_REG_IRQSTAT, 0xFF);
	writeValue(SI1145_REG_COMMAND, SI1145_RESET);

	HAL_Delay(100);

	writeValue(SI1145_REG_HWKEY, 0x17);

	HAL_Delay(100);
}

// Read UV measurements

uint16_t readUltraViolet()
{
	printf("read UV");
	data = readMeasurment(0x2C) / 100;
    return data;
}

// Read visible light spectrum measurements

uint16_t readVisible()
{
	printf("read Visible");
	data = readMeasurment(0x22) / 100;
    return data;
}

// Read IR measurements

uint16_t readInfrared()
{
	printf("read Infrared");
	data = readMeasurment(0x24) / 100;
    return data;
}

// Read proximity measurements

/*
uint16_t readProximity(void)
{
	data = readMeasurment(0x26) / 100;
    return data;
}
}*/
