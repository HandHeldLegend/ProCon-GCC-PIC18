
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SETTINGS_H
#define	SETTINGS_H

#include <xc.h> // include processor files - each processor file is guarded.
#include "main.h"

struct Settings{
    
    uint16_t configKey;
    
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
    
    uint8_t trigger_mode;
    
};

struct Settings SettingData;

#endif	/* SETTINGS_H */

