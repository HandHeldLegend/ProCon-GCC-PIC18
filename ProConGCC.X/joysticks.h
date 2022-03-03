// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef JOYSTICKS_H
#define	JOYSTICKS_H

#include <xc.h> // include processor files - each processor file is guarded. 

uint16_t adc_read;

uint16_t tmphigh;
uint16_t tmplow;
uint16_t tmpcenter;
uint16_t tmphighm;
uint16_t tmplowm;
uint8_t stickIdx;
bool initiatedRead;
bool clearRead;

void scansticks(void);

void calibratesticks(void);

unsigned char slinjim(uint16_t dividend, uint16_t multiplier);

uint16_t getmultiplier(uint16_t high, uint16_t low);

#endif	/* JOYSTICKS_H */

