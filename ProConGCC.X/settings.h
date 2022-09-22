
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SETTINGS_H
#define	SETTINGS_H

#include <xc.h> // include processor files - each processor file is guarded.
#include "main.h"
#include "gcdata.h"

// Save 50 bytes for loading settings
typedef struct {
    
    uint16_t buffer;
    uint8_t configKey;
    
    uint16_t sxlow;
    uint16_t sxcenter;
    uint16_t sxhigh;

    uint16_t sylow;
    uint16_t sycenter;
    uint16_t syhigh;

    uint16_t cxlow;
    uint16_t cxcenter;
    uint16_t cxhigh;

    uint16_t cylow;
    uint16_t cycenter;
    uint16_t cyhigh;

    uint16_t sx_highMultiplier;
    uint16_t sx_lowMultiplier;

    uint16_t sy_highMultiplier;
    uint16_t sy_lowMultiplier;

    uint16_t cx_highMultiplier;
    uint16_t cx_lowMultiplier;

    uint16_t cy_highMultiplier;
    uint16_t cy_lowMultiplier;
    
    uint8_t modeData;
    uint8_t rumbleData;
    
    uint8_t x_snapback_strength;
    uint8_t y_snapback_strength;
    
    uint8_t deadZone;
} Settings;

void setdefaultsettings(void);
void livesettings(void);
void zerosticks(void);
void setstickmultipliers(void);

void loadsettings(void);
void savesettings(void);

Settings SettingData;

#endif	/* SETTINGS_H */

