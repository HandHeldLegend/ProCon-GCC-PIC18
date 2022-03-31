/*
 * File:   settings.c
 * Author: Mitch
 *
 * Created on January 8, 2022, 3:31 PM
 */


#include <xc.h>
#include "main.h"

// Main Settings (save 50 bytes for settings)
Settings SettingData __at(0x03CD);

void setdefaultsettings(void)
{
    SettingData.buffer = 0x0000;
    SettingData.configKey = 0x4E;
  
    SettingData.sxlow = 140U;
    SettingData.sxcenter = 254U;
    SettingData.sxhigh = 385U;

    SettingData.sylow = 140U;
    SettingData.sycenter = 252U;
    SettingData.syhigh = 398U;

    SettingData.cxlow = 108U;
    SettingData.cxcenter = 252U;
    SettingData.cxhigh = 355U;

    SettingData.cylow = 127U;
    SettingData.cycenter = 254U;
    SettingData.cyhigh = 385U;

    SettingData.sx_highMultiplier = 200U;
    SettingData.sx_lowMultiplier = 232U;

    SettingData.sy_highMultiplier = 175U;
    SettingData.sy_lowMultiplier = 228U;

    SettingData.cx_highMultiplier = 257U;
    SettingData.cx_lowMultiplier = 177U;

    SettingData.cy_highMultiplier = 195U;
    SettingData.cy_lowMultiplier = 201U;
    
    setstickmultipliers();

    SettingData.modeData = 0U;
    // 0 - Dual mode: ZL/ZR act as Analog AND Digital press for L/R
    // 1 - Rightmost analog mode: ZL acts as L, L acts as Z, ZR is full press, R is half analog press
    // 2 - Leftmost analog mode: ZL is digital L, L is analog L, ZR is R digital/analog, R is Z
    // 3 - Full trigger mode: ZL - L digital, L - L analog, ZR - R digital, ZR - R analog
    // If MSB is set to 1, Xbox button layout is enabled, otherwise default Procon layout
    
    SettingData.rumbleData = 0;
    SettingData.deadZone = 16U;
}

void zerosticks(void)
{
    uint16_t aread;
    
    ADPCH = channel_VSS;
    ADCON0bits.ADGO = 1;
    while (ADCON0bits.ADGO)
    {}
    ADPCH = SX_A;           // Set the ADC to the channel for Stick X (see adcc.h)
    ADCON0bits.ADGO = 1;    // Set the ADC conversion to GO. (runs on hardware nonblocking)
    while (ADCON0bits.ADGO)
    {}
    aread = ((adc_result_t)((ADRESH << 8) + ADRESL)) >> 3;
    SettingData.sxhigh = aread;
    SettingData.sxlow = aread;
    SettingData.sxcenter = aread;

    ADPCH = channel_VSS;
    ADCON0bits.ADGO = 1;
    while (ADCON0bits.ADGO)
    {}
    ADPCH = SY_A;           // Set the ADC to the channel for Stick X (see adcc.h)
    ADCON0bits.ADGO = 1;    // Set the ADC conversion to GO. (runs on hardware nonblocking)
    while (ADCON0bits.ADGO)
    {}
    aread = ((adc_result_t)((ADRESH << 8) + ADRESL)) >> 3;
    SettingData.syhigh = aread;
    SettingData.sylow = aread;
    SettingData.sycenter = aread;

    ADPCH = channel_VSS;
    ADCON0bits.ADGO = 1;
    while (ADCON0bits.ADGO)
    {}
    ADPCH = CX_A;           // Set the ADC to the channel for Stick X (see adcc.h)
    ADCON0bits.ADGO = 1;    // Set the ADC conversion to GO. (runs on hardware nonblocking)
    while (ADCON0bits.ADGO)
    {}
    aread = ((adc_result_t)((ADRESH << 8) + ADRESL)) >> 3;
    SettingData.cxhigh = aread;
    SettingData.cxlow = aread;
    SettingData.cxcenter = aread;

    ADPCH = channel_VSS;
    ADCON0bits.ADGO = 1;
    while (ADCON0bits.ADGO)
    {}
    ADPCH = CY_A;           // Set the ADC to the channel for Stick X (see adcc.h)
    ADCON0bits.ADGO = 1;    // Set the ADC conversion to GO. (runs on hardware nonblocking)
    while (ADCON0bits.ADGO)
    {}
    aread = ((adc_result_t)((ADRESH << 8) + ADRESL)) >> 3;
    SettingData.cyhigh = aread;
    SettingData.cylow = aread;
    SettingData.cycenter = aread;

}

void setstickmultipliers(void)
{
    SettingData.sx_highMultiplier = getmultiplier(SettingData.sxhigh, SettingData.sxcenter+SettingData.deadZone);
    SettingData.sx_lowMultiplier = getmultiplier(SettingData.sxcenter-SettingData.deadZone, SettingData.sxlow);
    
    SettingData.cx_highMultiplier = getmultiplier(SettingData.cxhigh, SettingData.cxcenter+SettingData.deadZone);
    SettingData.cx_lowMultiplier = getmultiplier(SettingData.cxcenter-SettingData.deadZone, SettingData.cxlow);
    
    SettingData.sy_highMultiplier = getmultiplier(SettingData.syhigh, SettingData.sycenter+SettingData.deadZone);
    SettingData.sy_lowMultiplier = getmultiplier(SettingData.sycenter-SettingData.deadZone, SettingData.sylow);
    
    SettingData.cy_highMultiplier = getmultiplier(SettingData.cyhigh, SettingData.cycenter+SettingData.deadZone);
    SettingData.cy_lowMultiplier = getmultiplier(SettingData.cycenter-SettingData.deadZone, SettingData.cylow);
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
        if (i == 2U && SettingData.configKey != 0x4E)
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


