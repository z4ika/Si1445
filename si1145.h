#include "i2c.h"

#ifndef SI1445_SI1145_H
#define SI1445_SI1145_H

//uint8_t SI1145_PARAM_QUERY = 0x80;

#define SI1145_PARAM_QUERY 		    	(uint8_t*) 0x80
#define SI1145_RESET 			    	(uint8_t*) 0x01
#define SI1145_PSALS_AUTO      	     	(uint8_t*) 0x0F
#define SI1145_PARAM_ADDR 		     	(uint8_t*) 0x60
#define SI1145_PARAM_CHLIST 	 	 	(uint8_t*) 0x01
#define SI1145_PARAM_CHLIST_ENALSIR  	(uint8_t*) 0x20
#define SI1145_PARAM_CHLIST_ENALSVIS 	(uint8_t*) 0x10
#define SI1145_PARAM_CHLIST_ENPS1    	(uint8_t*) 0x01
#define SI1145_REG_INTCFG 			 	(uint8_t*) 0x03
#define SI1145_REG_INTCFG_INTOE 	 	(uint8_t*) 0x01
#define SI1145_REG_IRQEN 			 	(uint8_t*) 0x04
#define SI1145_REG_IRQEN_ALSEVERYSAMPLE (uint8_t*) 0x01
#define SI1145_REG_IRQMODE1 			(uint8_t*) 0x05
#define SI1145_REG_IRQMODE2 			(uint8_t*) 0x06
#define SI1145_REG_HW_KEY 				(uint8_t*) 0x07
#define SI1145_REG_MEASRATE0		    (uint8_t*) 0x08
#define SI1145_REG_MEASRATE1 			(uint8_t*) 0x09
#define SI1145_REG_UCOEFF0 				(uint8_t*) 0x13
#define SI1145_REG_UCOEFF1 				(uint8_t*) 0x14c
#define SI1145_REG_UCOEFF2 				(uint8_t*) 0x15
#define SI1145_REG_UCOEFF3 				(uint8_t*) 0x16
#define SI1145_REG_COMMAND 				(uint8_t*) 0x18
#define SI1145_REG_IRQSTAT 				(uint8_t*) 0x21
#define SI1145_REG_PARAMRD 				(uint8_t*) 0x2E

extern uint8_t readValue(uint8_t v);
extern void si1145_startAuto();
extern void si1145_reset();
extern uint16_t readUltraViolet();
extern uint16_t readVisible();
extern uint16_t readInfrared();

#endif
