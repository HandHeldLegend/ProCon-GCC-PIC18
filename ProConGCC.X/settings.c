/*
 * File:   settings.c
 * Author: Mitch
 *
 * Created on January 8, 2022, 3:31 PM
 */


#include <xc.h>
#include "main.h"

// Main Settings
Settings SettingData;

void setdefaultsettings(void)
{
    SettingData.buffer = 0x0000;
    SettingData.configKey = 0x4EF4;
  
    SettingData.sx_low = 0x0008;
    SettingData.sx_center = 254U;
    SettingData.sx_high = 385U;

    SettingData.sy_low = 140U;
    SettingData.sy_center = 252U;
    SettingData.sy_high = 398U;

    SettingData.cx_low = 108U;
    SettingData.cx_center = 252U;
    SettingData.cx_high = 355U;

    SettingData.cy_low = 127U;
    SettingData.cy_center = 254U;
    SettingData.cy_high = 385U;

    SettingData.sx_highMultiplier = 200U;
    SettingData.sx_lowMultiplier = 232U;

    SettingData.sy_highMultiplier = 175U;
    SettingData.sy_lowMultiplier = 228U;

    SettingData.cx_highMultiplier = 257U;
    SettingData.cx_lowMultiplier = 177U;

    SettingData.cy_highMultiplier = 195U;
    SettingData.cy_lowMultiplier = 201U;

    SettingData.trigger_mode = 0x0;
}

void zerosticks(void)
{
    SettingData.sx_low = 250U;
    SettingData.sx_high = 250U;

    SettingData.sy_low = 250U;
    SettingData.sy_high = 250U;

    SettingData.cx_low = 250U;
    SettingData.cx_high = 250U;

    SettingData.cy_low = 250U;
    SettingData.cy_high = 250U;
}

void setstickmultipliers(void)
{
    SettingData.sx_highMultiplier = getmultiplier(SettingData.sx_high, SettingData.sx_center);
    SettingData.sx_lowMultiplier = getmultiplier(SettingData.sx_high, SettingData.sx_center);
    
    SettingData.cx_highMultiplier = getmultiplier(SettingData.cx_high, SettingData.cx_center);
    SettingData.cx_lowMultiplier = getmultiplier(SettingData.cx_center, SettingData.cx_low);
    
    SettingData.sy_highMultiplier = getmultiplier(SettingData.sy_high, SettingData.sy_center);
    SettingData.sy_lowMultiplier = getmultiplier(SettingData.sy_center, SettingData.sy_low);
    
    SettingData.cy_highMultiplier = getmultiplier(SettingData.cy_high, SettingData.cy_center);
    SettingData.cy_lowMultiplier = getmultiplier(SettingData.cy_center, SettingData.cy_low);
}

void loadsettings(void) 
{
    //Clear all parameters
    memset((void*)&SettingData, 0, sizeof(Settings));
    
    uint8_t* loading = (uint8_t*)&SettingData;
    uint8_t i;
    
    for (i = 0; i < sizeof(Settings); i++) 
    {
        asm("clrwdt");
        loading[i] = DATAEE_ReadByte(i);

        // Check if our configKey does not match the character "NM" :)
        if (i == 3U && SettingData.configKey != 0x4EF4)
        {
            // We did not match, let's write the default values to EEPROM
            setdefaultsettings();
            savesettings();
            return;
        }
    }
}

void savesettings(void) 
{
    uint8_t i;
    uint8_t* writing = (uint8_t*)&SettingData;
    for (i = 0; i < sizeof(Settings); i++) 
    {
        DATAEE_WriteByte(i, writing[i]);
    }
}


