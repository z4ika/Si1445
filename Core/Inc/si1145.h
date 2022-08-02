#include "i2c.h"
#include <stdbool.h>

#ifndef SI1445_SI1145_H
#define SI1445_SI1145_H

/* Typedef */
typedef struct 
{
    I2C_HandleTypeDef *i2c;
    uint16_t address;
    uint8_t i2c_delay;
    uint8_t buffer;
    OS_t os;
} Si1145_t;

typedef enum 
{
    OS_FREERTOS,
    OS_STM32
} OS_t;

/* 
    I2C address  
    left shifted according to ST documentation
    0x60 << 1 = 0xC0
                                               */
#define SI1145_PARAM_ADDR (0xC0)

/* Commands */
#define SI1145_PARAM_QUERY 0x80
#define SI1145_PARAM_SET 0xA0
#define SI1145_NOP 0x0
#define SI1145_RESET 0x01
#define SI1145_BUSADDR 0x02
#define SI1145_PS_FORCE 0x05
#define SI1145_ALS_FORCE 0x06
#define SI1145_PSALS_FORCE 0x07
#define SI1145_PS_PAUSE 0x09
#define SI1145_ALS_PAUSE 0x0A
#define SI1145_PSALS_PAUSE 0xB
#define SI1145_PS_AUTO 0x0D
#define SI1145_ALS_AUTO 0x0E
#define SI1145_PSALS_AUTO 0x0F
#define SI1145_GET_CAL 0x12

/* Parameters */
#define SI1145_PARAM_I2CADDR 0x00
#define SI1145_PARAM_CHLIST 0x01
#define SI1145_PARAM_CHLIST_ENUV 0x80
#define SI1145_PARAM_CHLIST_ENAUX 0x40
#define SI1145_PARAM_CHLIST_ENALSIR 0x20
#define SI1145_PARAM_CHLIST_ENALSVIS 0x10
#define SI1145_PARAM_CHLIST_ENPS1 0x01
#define SI1145_PARAM_CHLIST_ENPS2 0x02
#define SI1145_PARAM_CHLIST_ENPS3 0x04
#define SI1145_PARAM_PSLED12SEL 0x02
#define SI1145_PARAM_PSLED12SEL_PS2NONE 0x00
#define SI1145_PARAM_PSLED12SEL_PS2LED1 0x10
#define SI1145_PARAM_PSLED12SEL_PS2LED2 0x20
#define SI1145_PARAM_PSLED12SEL_PS2LED3 0x40
#define SI1145_PARAM_PSLED12SEL_PS1NONE 0x00
#define SI1145_PARAM_PSLED12SEL_PS1LED1 0x01
#define SI1145_PARAM_PSLED12SEL_PS1LED2 0x02
#define SI1145_PARAM_PSLED12SEL_PS1LED3 0x04
#define SI1145_PARAM_PSLED3SEL 0x03
#define SI1145_PARAM_PSENCODE 0x05
#define SI1145_PARAM_ALSENCODE 0x06
#define SI1145_PARAM_PS1ADCMUX 0x07
#define SI1145_PARAM_PS2ADCMUX 0x08
#define SI1145_PARAM_PS3ADCMUX 0x09
#define SI1145_PARAM_PSADCOUNTER 0x0A
#define SI1145_PARAM_PSADCGAIN 0x0B
#define SI1145_PARAM_PSADCMISC 0x0C
#define SI1145_PARAM_PSADCMISC_RANGE 0x20
#define SI1145_PARAM_PSADCMISC_PSMODE 0x04
#define SI1145_PARAM_ALSIRADCMUX 0x0E
#define SI1145_PARAM_AUXADCMUX 0x0F
#define SI1145_PARAM_ALSVISADCOUNTER 0x10
#define SI1145_PARAM_ALSVISADCGAIN 0x11
#define SI1145_PARAM_ALSVISADCMISC 0x12
#define SI1145_PARAM_ALSVISADCMISC_VISRANGE 0x20
#define SI1145_PARAM_ALSIRADCOUNTER 0x1D
#define SI1145_PARAM_ALSIRADCGAIN 0x1E
#define SI1145_PARAM_ALSIRADCMISC 0x1F
#define SI1145_PARAM_ALSIRADCMISC_RANGE 0x20
#define SI1145_PARAM_ADCCOUNTER_511CLK 0x70
#define SI1145_PARAM_ADCMUX_SMALLIR 0x00
#define SI1145_PARAM_ADCMUX_LARGEIR 0x03

/* Registers */
#define SI1145_REG_PARTID 0x00
#define SI1145_REG_REVID 0x01
#define SI1145_REG_SEQID 0x02
#define SI1145_REG_INTCFG 0x03
#define SI1145_REG_INTCFG_INTOE 0x01
#define SI1145_REG_INTCFG_INTMODE 0x02
#define SI1145_REG_IRQEN 0x04
#define SI1145_REG_IRQEN_ALSEVERYSAMPLE 0x01
#define SI1145_REG_IRQEN_PS1EVERYSAMPLE 0x04
#define SI1145_REG_IRQEN_PS2EVERYSAMPLE 0x08
#define SI1145_REG_IRQEN_PS3EVERYSAMPLE 0x10
#define SI1145_REG_IRQMODE1 0x05
#define SI1145_REG_IRQMODE2 0x06
#define SI1145_REG_HWKEY 0x07
#define SI1145_REG_MEASRATE0 0x08
#define SI1145_REG_MEASRATE1 0x09
#define SI1145_REG_PSRATE 0x0A
#define SI1145_REG_PSLED12 0x0F
#define SI1145_REG_PSLED3 0x10
#define SI1145_REG_UCOEFF0 0x13
#define SI1145_REG_UCOEFF1 0x14
#define SI1145_REG_UCOEFF2 0x15
#define SI1145_REG_UCOEFF3 0x16
#define SI1145_REG_PARAMWR 0x17
#define SI1145_REG_COMMAND 0x18
#define SI1145_REG_RESPONSE 0x20
#define SI1145_REG_IRQSTAT 0x21
#define SI1145_REG_IRQSTAT_ALS 0x01
/* Visible light registers address offsets */
#define SI1145_REG_ALSVISDATA0 0x22
#define SI1145_REG_ALSVISDATA1 0x23
/* Infrared light registers address offsets */
#define SI1145_REG_ALSIRDATA0 0x24
#define SI1145_REG_ALSIRDATA1 0x25
/* Proximity sense 1 registers address offsets */
#define SI1145_REG_PS1DATA0 0x26
#define SI1145_REG_PS1DATA1 0x27
/* Proximity sense 2 registers address offsets */
#define SI1145_REG_PS2DATA0 0x28
#define SI1145_REG_PS2DATA1 0x29
/* Proximity sense 3 registers address offsets */
#define SI1145_REG_PS3DATA0 0x2A
#define SI1145_REG_PS3DATA1 0x2B
/* Ultra-violet light registers address offsets */
#define SI1145_REG_UVINDEX0 0x2C
#define SI1145_REG_UVINDEX1 0x2D
#define SI1145_REG_PARAMRD 0x2E
#define SI1145_REG_CHIPSTAT 0x30

/* extern functions */
extern HAL_StatusTypeDef Si1445_default( Si1145_t *Si1145, I2C_HandleTypeDef *i2c )
extern void Si1145_writeValue( Si1145_t *Si1145, uint8_t reg, uint8_t val )
extern uint8_t Si1145_readValue( Si1145_t *Si1145, uint8_t reg )
extern uint8_t Si1145_writeParam( Si1145_t *Si1145, uint8_t val1, uint8_t val2 )
extern uint8_t Si1145_readParam( uint8_t val )
extern uint16_t Si1145_readMeasurment( Si1145_t *Si1145, uint8_t offset )
extern void Si1145_reset( Si1145_t *Si1145 )
extern void Si1145_default_config( Si1145_t *Si1145 )
extern float Si1145_readUltraViolet( Si1145_t *Si1145 )
extern uint16_t readVisible( Si1145_t *Si1145 )
extern uint16_t readInfrared( Si1145_t *Si1145 )
extern uint16_t readProximity( Si1145_t *Si1145 )

#endif
