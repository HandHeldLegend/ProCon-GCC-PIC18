// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef JOYSTICKS_H
#define	JOYSTICKS_H

#include <xc.h> // include processor files - each processor file is guarded.
#include "main.h"
#include "memorymap.h"
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>

#define STICK_CENTER_GCC                127 // Default center value for gamecube

#define AVERAGE_COUNT   16

// Non-allocated memory items just for this class
volatile uint8_t stickIdx        __at(MEM_STICKIDX);
volatile uint8_t initiatedRead   __at(MEM_STICKINIT);
volatile uint8_t clearRead       __at(MEM_STICKCLR);

// Store all these parameters below Setting data in memory
volatile uint16_t adc_read   __at(MEM_ADC_READ);
volatile uint16_t tmphigh    __at(MEM_TMPHIGH);
volatile uint16_t tmplow     __at(MEM_TMPLOW);
volatile uint16_t tmpcenter  __at(MEM_TMPCENTER);
volatile uint16_t tmphighm   __at(MEM_TMPHIGHM);
volatile uint16_t tmplowm    __at(MEM_TMPLOWM);

volatile uint16_t conversion __at(MEM_ADC_CONV);

volatile uint16_t sb_start_time  __at(MEM_SB_TIME);   // Store the current time from Timer 0
volatile int thistime            __at(MEM_SB_TIMEINT);
volatile uint8_t sb_time_of      __at(MEM_SB_TOFLOW);
volatile int sb_tof              __at(MEM_SB_TOFLIGHT);


// 0 - not measuring
// 1 - triggered but not met velocity requirements
// 2 - triggered, met velocity requirements.
// If number 2, we must wait for input to come back to 
// neutral to capture the data.
volatile uint8_t     sb_xp_measuring __at(MEM_SB_XPM);
volatile uint8_t     sb_xn_measuring __at(MEM_SB_XNM);


void joysticksetup(void);

void applysnapback(void);

void calibratesnapback(void);

void scansticks(void);

void calibratesticks(void);

unsigned char slinjim(uint16_t dividend, uint16_t multiplier);

uint16_t getmultiplier(uint16_t high, uint16_t low);

#endif	/* JOYSTICKS_H */

