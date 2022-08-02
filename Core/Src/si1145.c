#include "si1145.h"

void Si1145_delay( Si1145_t *Si1145, uint8_t delay )
{
	if( Si1145->os == OS_FREERTOS )
		vTaskDelay( delay );
	else
		HAL_Delay( delay );
}

HAL_StatusTypeDef Si1445_default( Si1145_t *Si1145, I2C_HandleTypeDef *i2c )
{
	Si1145->i2c = i2c;
	Si1145->address = SI1145_PARAM_ADDR;
	Si1145->i2c_delay = 100;
	Si1145->buffer = 0;

	Si1145_default_config( Si1145 );
	return HAL_OK;
}

void Si1145_writeValue( Si1145_t *Si1145, uint8_t reg, uint8_t val )
{
	HAL_I2C_Mem_Write( Si1145->i2c, Si1145->address, reg, I2C_MEMADD_SIZE_8BIT, &val, 1, Si1145->i2c_delay );
}

uint8_t Si1145_readValue( Si1145_t *Si1145, uint8_t reg )
{
	HAL_I2C_Mem_Read( Si1145->i2c, Si1145->address, reg, I2C_MEMADD_SIZE_8BIT, Si1145->buffer, 1, Si1145->i2c_delay );
	return Si1145->buffer;
}

uint8_t Si1145_writeParam( Si1145_t *Si1145, uint8_t val1, uint8_t val2 )
{
	Si1145_writeValue( Si1145, SI1145_REG_PARAMWR, val1);
	Si1145_writeValue( Si1145, SI1145_REG_COMMAND, val2 | SI1145_PARAM_SET );
	return Si1145_readValue( Si1145, SI1145_REG_PARAMRD );
}

uint8_t Si1145_readParam( Si1145_t *Si1145, uint8_t val )
{
	Si1145_writeValue( Si1145, SI1145_REG_COMMAND, val | SI1145_PARAM_QUERY );
	return Si1145_readValue( Si1145, SI1145_REG_PARAMRD );
}

uint16_t Si1145_readMeasurment( Si1145_t *Si1145, uint8_t offset )
{
	uint16_t data;
	uint8_t readbuffer[2];
	HAL_I2C_Mem_Read( Si1145->i2c, Si1145->address, offset, I2C_MEMADD_SIZE_16BIT, readbuffer, 2, Si1145->i2c_delay );
	data = ( readbuffer[0] | ( (uint16_t) (readbuffer[1]) << 8 ) );
	return data;
}

void Si1145_reset( Si1145_t *Si1145 )
{
	Si1145_writeValue( Si1145, SI1145_REG_MEASRATE0, 0x00 );
	Si1145_writeValue( Si1145, SI1145_REG_MEASRATE1, 0x00 );
	Si1145_writeValue( Si1145, SI1145_REG_IRQEN,     0x00) ;
	Si1145_writeValue( Si1145, SI1145_REG_IRQMODE1,  0x00 );
	Si1145_writeValue( Si1145, SI1145_REG_IRQMODE2,  0x00 );
	Si1145_writeValue( Si1145, SI1145_REG_INTCFG,    0x00 );
	Si1145_writeValue( Si1145, SI1145_REG_IRQSTAT,   0xFF );

	Si1145_writeValue( Si1145, SI1145_REG_COMMAND, SI1145_RESET );

	Si1145_delay( Si1145, 100);

	Si1145_writeValue( Si1145, SI1145_REG_HWKEY, 0x17 );

	Si1145_delay( Si1145, 100);
}

void Si1145_default_config( Si1145_t *Si1145 )
{
	  /* Apply recommended UV coefficients values */
	  Si1145_writeValue( Si1145, SI1145_REG_UCOEFF0, 0x29 );
	  Si1145_writeValue( Si1145, SI1145_REG_UCOEFF1, 0x89 );
	  Si1145_writeValue( Si1145, SI1145_REG_UCOEFF2, 0x82 );
	  Si1145_writeValue( Si1145, SI1145_REG_UCOEFF3, 0x00 );

	  /* Initialise UV sensor */
	  Si1145_writeParam( Si1145, SI1145_PARAM_CHLIST, SI1145_PARAM_CHLIST_ENUV | SI1145_PARAM_CHLIST_ENALSIR |SI1145_PARAM_CHLIST_ENALSVIS | SI1145_PARAM_CHLIST_ENPS1 );

	  /* Enable interrupts on every sample event */
	  Si1145_writeValue( Si1145, SI1145_REG_INTCFG, SI1145_REG_INTCFG_INTOE );
	  Si1145_writeValue( Si1145, SI1145_REG_IRQEN, SI1145_REG_IRQEN_ALSEVERYSAMPLE );

	  /* Apply LED current for proximity LED #1 */
	  Si1145_writeValue( Si1145, SI1145_REG_PSLED12, 0x03 );
	  Si1145_writeParam( Si1145, SI1145_PARAM_PS1ADCMUX, SI1145_PARAM_ADCMUX_LARGEIR );
	  
	  /* Use proximity LED #1 for sensor */
	  Si1145_writeParam( Si1145, SI1145_PARAM_PSLED12SEL, SI1145_PARAM_PSLED12SEL_PS1LED1 );

	  /* Set ADC clock divider to fastest (value 0 according to docs) */
	  Si1145_writeParam( Si1145, SI1145_PARAM_PSADCGAIN, 0 );

	  /* Set recovery peroid the ADC takes before making a PS measurment to 511 */
	  Si1145_writeParam( Si1145, SI1145_PARAM_PSADCOUNTER, SI1145_PARAM_ADCCOUNTER_511CLK );

	  /* Proximity sensor mode set to high range */
	  Si1145_writeParam( Si1145, SI1145_PARAM_PSADCMISC, SI1145_PARAM_PSADCMISC_RANGE | SI1145_PARAM_PSADCMISC_PSMODE );

      /* Use small IR photodiode (0x00) */
	  Si1145_writeParam( Si1145, SI1145_PARAM_ALSIRADCMUX, SI1145_PARAM_ADCMUX_SMALLIR );

	  /* Set ADC clock divider to fastest */
	  Si1145_writeParam( Si1145, SI1145_PARAM_ALSIRADCGAIN, 0 );

	  /* Set recovery peroid the ADC takes before making a IR measurment to 511 */
	  Si1145_writeParam( Si1145, SI1145_PARAM_ALSIRADCOUNTER, SI1145_PARAM_ADCCOUNTER_511CLK );

	  /* IR sensor mode set to high range */
	  Si1145_writeParam( Si1145, SI1145_PARAM_ALSIRADCMISC, SI1145_PARAM_ALSIRADCMISC_RANGE );

	  /* Set ADC clock divider to fastest */
	  Si1145_writeParam( Si1145, SI1145_PARAM_ALSVISADCGAIN, 0 );

	  /* Set recovery peroid the ADC takes before making a visible light measurment to 511 */
	  Si1145_writeParam( Si1145, SI1145_PARAM_ALSVISADCOUNTER, SI1145_PARAM_ADCCOUNTER_511CLK );

	   /* IR sensor mode set to high range */
	  Si1145_writeParam( Si1145, SI1145_PARAM_ALSVISADCMISC, SI1145_PARAM_ALSVISADCMISC_VISRANGE );

	  /* Rate which represents time interval at which sensor wakes up to perform measurment */
	  Si1145_writeValue( Si1145, SI1145_REG_MEASRATE0, 0xFF );

	  /* Autonomous ALS and PS measurment mode in a loop */
      Si1145_writeValue( Si1145, SI1145_REG_COMMAND, SI1145_PSALS_AUTO );
	
	  Si1145_delay( Si1145, 100);
}

/* Read UV measurements 
   Divided by 100 according to docs */

float Si1145_readUltraViolet( Si1145_t *Si1145 )
{
	return ( (float) Si1145_readMeasurment( Si1145, 0x2C ) / 100.0f );
}

/* Read visible light measurements */

uint16_t Si1145_readVisible( Si1145_t *Si1145 )
{
	return Si1145_readMeasurment( Si1145, SI1145_REG_ALSVISDATA0 );
}

/* Read IR measurements */

uint16_t Si1145_readInfrared( Si1145_t *Si1145 )
{
	return Si1145_readMeasurment( Si1145, SI1145_REG_ALSIRDATA0 );
}

/* Read proximity measurements */

uint16_t Si1145_readProximity( Si1145_t *Si1145 )
{
	return Si1145_readMeasurment( Si1145, SI1145_REG_PS1DATA0 );
}
