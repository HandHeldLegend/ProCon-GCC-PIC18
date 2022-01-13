/*
 * File:   joysticks.c
 * Author: Mitch
 *
 * Created on December 15, 2021, 12:05 AM
 */

#include <xc.h>
#include "main.h"

uint16_t sx_low = 131;
uint16_t sx_center = 254;
uint16_t sx_high = 385;

uint16_t sy_low = 140;
uint16_t sy_center = 252;
uint16_t sy_high = 398;

uint16_t cx_low = 108;
uint16_t cx_center = 252;
uint16_t cx_high = 355;

uint16_t cy_low = 127;
uint16_t cy_center = 254;
uint16_t cy_high = 385;

uint16_t sx_highMultiplier = 200;
uint16_t sx_lowMultiplier = 232;

uint16_t sy_highMultiplier = 175;
uint16_t sy_lowMultiplier = 228;

uint16_t cx_highMultiplier = 257;
uint16_t cx_lowMultiplier = 177;

uint16_t cy_highMultiplier = 195;
uint16_t cy_lowMultiplier = 201;

uint16_t adc_read;

static uint16_t tmphigh;
static uint16_t tmplow;
static uint16_t tmpcenter;
static uint16_t tmphighm;
static uint16_t tmplowm;

static uint8_t stickIdx = 0;
static bool initiatedRead = false;
static bool clearRead = false;

// This function multiplies the input by the multiplier, then
// it divides by 265. Amazing solution by slinjim :)
// Bitshifting 8 is equal to dividing by 256 (close enough)
// 
unsigned char slinjim(uint16_t dividend, uint16_t multiplier)
{
    uint16_t conversion = (dividend * multiplier+1) >> 8;
    return (unsigned char) conversion;
}

// This function generates a multiplier allowing
// scaling to a 0 out of 100 value. The output is
// the numerator over a denominator of value 256.
uint16_t getmultiplier(uint16_t high, uint16_t low)
{
    
    uint16_t distance = high-low;
    uint16_t ratio = ((10000U / distance) * 256U)/100U;
    
    return ratio;
}



void scansticks(void) {
    
    // On our first time through this function
    // we can initiate the first read
    if (!initiatedRead)
    {
        ADPCH = SX_A;           // Set the ADC to the channel for Stick X (see adcc.h)
        ADCON0bits.ADGO = 1;    // Set the ADC conversion to GO. (runs on hardware nonblocking)
        stickIdx = 0;           // Set the stick index to 0
        clearRead = false;
        initiatedRead = true;   // Set the bool to true indicating we already started our first read
    }
    else
    {
        // We are here if we already initiated a read
        
        if (!ADCON0bits.ADGO) // A conversion is completed, otherwise skip
        {
            if (clearRead) // We performed a blank read to clear the ADC cap charge.
            {
                switch(stickIdx)
                {
                    case 0:
                        // Set the ADC Read Channel bit
                        ADPCH = SY_A;
                        break;
                    case 1:
                        ADPCH = CX_A;
                        break;
                    case 2:
                        ADPCH = CY_A;
                        break;
                    case 3:
                        asm("BCF _gInStatus, 3, 0"); // Clear the stick read enable flag
                        asm("BSF _gInStatus, 2, 0"); // Set the button read enable flag
                        initiatedRead = false;       // Reset the bool to initialize a read
                        clearRead = false;
                        
                        //DEBUG
                        // This will pull the line low once the last read is completed.
                        //asm("BCF	    TRISB, 2");
                        //asm("BCF	PORTB, 2");
                        //END DEBUG

                        return;
                        
                }
                // Increase the stick index for the next read.
                stickIdx++;
                clearRead = false;
                ADCON0bits.ADGO = 1;                
            }
            else    // We are reading a non-blank answer
            {
                clearRead = true;
                // Set ADC read channel to ground read to clear the
                // charge capacitor for the next read.
                ADPCH = channel_VSS;
                ADCON0bits.ADGO = 1;
            
                // Get the ADC reading and bitshift 3 times to scale it.
                // Todo: just set the register to do this scaling automatically :)
                adc_read = ((adc_result_t)((ADRESH << 8) + ADRESL)) >> 3;

                // Perform a different operation depending
                // on which stick we should be reading
                switch (stickIdx)
                {
                    case 0: // Stick X

                        tmphigh     = SettingData.sx_high;
                        tmplow      = SettingData.sx_low;
                        tmpcenter   = SettingData.sx_center;
                        tmphighm    = SettingData.sx_highMultiplier;
                        tmplowm     = SettingData.sx_lowMultiplier;

                        break;

                    case 1: // Stick Y

                        tmphigh     = SettingData.sy_high;
                        tmplow      = SettingData.sy_low;
                        tmpcenter   = SettingData.sy_center;
                        tmphighm    = SettingData.sy_highMultiplier;
                        tmplowm     = SettingData.sy_lowMultiplier;

                        break;

                    case 2: // C-Stick X

                        tmphigh     = SettingData.cx_high;
                        tmplow      = SettingData.cx_low;
                        tmpcenter   = SettingData.cx_center;
                        tmphighm    = SettingData.cx_highMultiplier;
                        tmplowm     = SettingData.cx_lowMultiplier;

                        break;

                    case 3: // C-Stick Y

                        tmphigh     = SettingData.cy_high;
                        tmplow      = SettingData.cy_low;
                        tmpcenter   = SettingData.cy_center;
                        tmphighm    = SettingData.cy_highMultiplier;
                        tmplowm     = SettingData.cy_lowMultiplier;

                        break;

                };

                // Determine if we are in the upper or lower stick range
                // then we perform our stick value conversion and store
                // the value in our outgoing byte.

                if (adc_read > tmpcenter+4)
                {
                    if (adc_read >= tmphigh)
                    {
                        gConPollPacket[stickIdx+2] = 228U;
                    }
                    else
                    {
                        gConPollPacket[stickIdx+2] = 128U + slinjim(adc_read - tmpcenter, tmphighm);
                    }
                }
                else if (adc_read < tmpcenter-4)
                {
                    if (adc_read <= tmplow)
                    {
                        gConPollPacket[stickIdx+2] = 28U;
                    }
                    else
                    {
                        gConPollPacket[stickIdx+2] = 127U - slinjim(tmpcenter - adc_read, tmplowm);
                    }
                }
                else
                {
                    gConPollPacket[stickIdx+2] = 128U;
                }

            }
        }
 
    }
    
    return; 
}

// This is a modified function which works in parity
// with the regular read function to ensure the same values
// are spit out. We only set the current calibration values to the maximum read in this mode, however.
void calibratesticks(void) {
    
    // On our first time through this function
    // we can initiate the first read
    if (!initiatedRead)
    {
        ADPCH = SX_A;           // Set the ADC to the channel for Stick X (see adcc.h)
        ADCON0bits.ADGO = 1;    // Set the ADC conversion to GO. (runs on hardware nonblocking)
        stickIdx = 0;           // Set the stick index to 0
        clearRead = false;
        initiatedRead = true;   // Set the bool to true indicating we already started our first read
    }
    else
    {
        // We are here if we already initiated a read
        
        if (!ADCON0bits.ADGO) // A conversion is completed, otherwise skip
        {
            if (clearRead) // We performed a blank read to clear the ADC cap charge.
            {
                switch(stickIdx)
                {
                    case 0:
                        // Set the ADC Read Channel bit
                        ADPCH = SY_A;
                        break;
                    case 1:
                        ADPCH = CX_A;
                        break;
                    case 2:
                        ADPCH = CY_A;
                        break;
                    case 3:
                        initiatedRead = false;       // Reset the bool to initialize a read
                        clearRead = false;
                        return;
                        
                }
                // Increase the stick index for the next read.
                stickIdx++;
                clearRead = false;
                ADCON0bits.ADGO = 1;                
            }
            else    // We are reading a non-blank answer
            {
                clearRead = true;
                // Set ADC read channel to ground read to clear the
                // charge capacitor for the next read.
                ADPCH = channel_VSS;
                ADCON0bits.ADGO = 1;
            
                // Get the ADC reading and bitshift 3 times to scale it.
                // Todo: just set the register to do this scaling automatically :)
                adc_read = ((adc_result_t)((ADRESH << 8) + ADRESL)) >> 3;

                // Perform a different operation depending
                // on which stick we should be reading
                switch (stickIdx)
                {
                    case 0: // Stick X

                        if (adc_read > SettingData.sx_high)
                        {
                            SettingData.sx_high = adc_read;
                        }
                        
                        if (adc_read < SettingData.sx_low)
                        {
                            SettingData.sx_low = adc_read;
                        }
                        
                        SettingData.sx_center = adc_read;

                        break;

                    case 1: // Stick Y

                        if (adc_read > SettingData.sy_high)
                        {
                            SettingData.sy_high = adc_read;
                        }
                        
                        if (adc_read < SettingData.sy_low)
                        {
                            SettingData.sy_low = adc_read;
                        }
                        
                        SettingData.sy_center = adc_read;

                        break;

                    case 2: // C-Stick X

                        if (adc_read > SettingData.cx_high)
                        {
                            SettingData.cx_high = adc_read;
                        }
                        
                        if (adc_read < SettingData.cx_low)
                        {
                            SettingData.cx_low = adc_read;
                        }
                        
                        SettingData.cx_center = adc_read;

                        break;

                    case 3: // C-Stick Y

                        if (adc_read > SettingData.cy_high)
                        {
                            SettingData.cy_high = adc_read;
                        }
                        
                        if (adc_read < SettingData.cy_low)
                        {
                            SettingData.cy_low = adc_read;
                        }
                        
                        SettingData.cy_center = adc_read;

                        break;

                };

            }
        }
 
    }
    
    return; 
}