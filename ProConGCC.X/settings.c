/*
 * File:   settings.c
 * Author: Mitch
 *
 * Created on January 8, 2022, 3:31 PM
 */


#include <xc.h>
#include "main.h"

// Main Settings (save 50 bytes for settings)
Settings SettingData __at(MEM_SETTINGS);

#define MAGIC_KEY    0x4B

void setdefaultsettings(void)
{
    SettingData.buffer = 0x0000;
    SettingData.configKey = MAGIC_KEY;
  
    SettingData.sxlow = 94U;
    SettingData.sxcenter = 254U;
    SettingData.sxhigh = 382U;

    SettingData.sylow = 124U;
    SettingData.sycenter = 255U;
    SettingData.syhigh = 415U;

    SettingData.cxlow = 108U;
    SettingData.cxcenter = 236U;
    SettingData.cxhigh = 385U;

    SettingData.cylow = 95U;
    SettingData.cycenter = 236U;
    SettingData.cyhigh = 380U;

    SettingData.sx_highMultiplier = 227U;
    SettingData.sx_lowMultiplier = 176U;

    SettingData.sy_highMultiplier = 163U;
    SettingData.sy_lowMultiplier = 227U;

    SettingData.cx_highMultiplier = 192U;
    SettingData.cx_lowMultiplier = 227U;

    SettingData.cy_highMultiplier = 194U;
    SettingData.cy_lowMultiplier = 204U;
    
    SettingData.x_snapback_strength = 0x0;
    SettingData.y_snapback_strength = 0x0;
    
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

#define SETTING_NOT_LIVE    0x0
#define SETTING_SELECT_HELD 0x1
#define SETTING_ZR_HELD     0x2
#define SETTING_ZL_HELD     0x3
#define SETTING_R_HELD      0x4
#define SETTING_L_HELD      0x5
#define SETTING_DDOWN_HELD  0x6
#define SETTING_DUP_HELD    0x7
#define SETTING_SAVE_ALL    0x8

uint8_t setting_marker = 0x0;
void livesettings(void)
{
    if (!SELECT_IN_PORT && !setting_marker)
    {
        if(!ZR_IN_PORT)
        {
            setting_marker = SETTING_ZR_HELD;
        }
        else if (!ZL_IN_PORT)
        {
            setting_marker = SETTING_ZL_HELD;
        }
        else if (!L_IN_PORT)
        {
            setting_marker = SETTING_L_HELD;
        }
        else if (!R_IN_PORT)
        {
            setting_marker = SETTING_R_HELD;
        }
        else if (!DU_IN_PORT)
        {
            setting_marker = SETTING_DUP_HELD;
        }
        else if (!DD_IN_PORT)
        {
            setting_marker = SETTING_DDOWN_HELD;
        }
    }
    else if (setting_marker > 0)
    {
        if(ZR_IN_PORT && setting_marker == SETTING_ZR_HELD)
        {
            SettingData.x_snapback_strength += 1;
            if (SettingData.x_snapback_strength > 14) SettingData.x_snapback_strength = 14;
            setting_marker = SETTING_SAVE_ALL;
        }
        else if (ZL_IN_PORT && setting_marker == SETTING_ZL_HELD)
        {
            
            if (SettingData.x_snapback_strength == 0) SettingData.x_snapback_strength = 0;
            else SettingData.x_snapback_strength -= 1;
            setting_marker = SETTING_SAVE_ALL;
        }
        else if (R_IN_PORT && setting_marker == SETTING_L_HELD)
        {
            SettingData.y_snapback_strength += 1;
            if (SettingData.y_snapback_strength > 14) SettingData.y_snapback_strength = 14;
            setting_marker = SETTING_SAVE_ALL;
        }
        else if (L_IN_PORT && setting_marker == SETTING_R_HELD)
        {
            if (SettingData.y_snapback_strength == 0) SettingData.y_snapback_strength = 0;
            else SettingData.y_snapback_strength -= 1;
            setting_marker = SETTING_SAVE_ALL;
        }
        else if (DU_IN_PORT && setting_marker == SETTING_DUP_HELD)
        {
            SettingData.deadZone += 4;
            if (SettingData.deadZone >= 28)
            {
                SettingData.deadZone = 28;
            }
            setting_marker = SETTING_SAVE_ALL;
        }
        else if (DD_IN_PORT && setting_marker == SETTING_DDOWN_HELD)
        {
            SettingData.deadZone -= 4;
            if (SettingData.deadZone <= 0 || SettingData.deadZone > 28)
            {
                SettingData.deadZone = 0;
            }
            setting_marker = SETTING_SAVE_ALL;
        }
        // This means we need to save a change
        else if (setting_marker == SETTING_SAVE_ALL)
        {
            INTCON0bits.GIEH = 0;
            SMT1CON1bits.SMT1GO = 0x0;
            setstickmultipliers();
            savesettings();
            __delay_us(450);
            loadsettings();
            __delay_us(450);
            setting_marker = 0x0;
            
            PIR1bits.SMT1PWAIF = 0;
            SMT1STATbits.CPRUP = 0x1;
            SMT1STATbits.CPWUP = 0x1;
            asm("NOP");
            PIE1bits.SMT1PWAIE = 1;
            
            T6CONbits.TMR6ON = 0;

            gInBitCounter = 0;
            gInStatus = 0;
            gPollStatus = POLL_STATUS_NULL;
            gRumbleStatus = RUMBLE_STATUS_OFF;
            gSynced = 0;
            
            TMR6_Initialize();
            INTCON0bits.GIEH = 1;
            SMT1CON1bits.SMT1GO = 1;
            T6CONbits.TMR6ON = 1;
        }
    }
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
        if (i == 2U && SettingData.configKey != MAGIC_KEY)
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


