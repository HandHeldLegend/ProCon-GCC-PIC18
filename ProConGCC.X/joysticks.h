// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef JOYSTICKS_H
#define	JOYSTICKS_H

#include <xc.h> // include processor files - each processor file is guarded. 

uint16_t sx_low;
uint16_t sx_center;
uint16_t sx_high;

uint16_t sy_low;
uint16_t sy_center;
uint16_t sy_high;

uint16_t cx_low;
uint16_t cx_center;
uint16_t cx_high;

uint16_t cy_low;
uint16_t cy_center;
uint16_t cy_high;

uint16_t sx_highMultiplier;
uint16_t sx_lowMultiplier;

uint16_t sy_highMultiplier;
uint16_t sy_lowMultiplier;

uint16_t cx_highMultiplier;
uint16_t cx_lowMultiplier;

uint16_t cy_highMultiplier;
uint16_t cy_lowMultiplier;

uint16_t adc_read;

static uint16_t tmphigh;
static uint16_t tmplow;
static uint16_t tmpcenter;
static uint16_t tmphighm;
static uint16_t tmplowm;
static uint8_t stickIdx;
static bool initiatedRead;
static bool clearRead;

void scansticks(void);

void calibratesticks(void);

unsigned char slinjim(uint16_t dividend, uint16_t multiplier);

uint16_t getmultiplier(uint16_t high, uint16_t low);

#endif	/* JOYSTICKS_H */

