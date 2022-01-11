

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef BUTTON_H
#define	BUTTON_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define BUTTON_PORT_A   0
#define BUTTON_PORT_B   1
#define TRIGGER_PORT_L  6
#define TRIGGER_PORT_R  7

#define TRIG_MODE_ALL   0   // L and R activate both digital and half analog press.
#define TRIG_MODE_ANG   1   // L and R ONLY activate half analog press
#define TRIG_MODE_DIG   2   // L and R ONLY activate digital press
#define TRIG_MODE_SPT   3   // L and R digital/analog are split up

// Masks for determine how to set a bit
#define MASK_A          0x1
#define MASK_B          0x2
#define MASK_X          0x4
#define MASK_Y          0x8
#define MASK_START      0x10
#define MASK_DLEFT      0x1
#define MASK_DRIGHT     0x2
#define MASK_DDOWN      0x4
#define MASK_DUP        0x8
#define MASK_Z          0x10
#define MASK_RDIGITAL   0x40
#define MASK_LDIGITAL   0x80

#define MASK_LANALOG    0x80    // Provides value of 128 to analog triggers
#define MASK_RANALOG    0x80

void checkbuttons(void);

#endif	/* BUTTON_H */

