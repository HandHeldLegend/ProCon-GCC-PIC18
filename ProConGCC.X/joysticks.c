/*
 * File:   joysticks.c
 * Author: Mitch
 *
 * Created on December 15, 2021, 12:05 AM
 */

#include <xc.h>
#include "main.h"

uint16_t adc_read __at(0x07C0);

uint16_t tmphigh __at(0x07BE);
uint16_t tmplow __at(0x07BC);
uint16_t tmpcenter __at(0x07BA);
uint16_t tmphighm __at(0x07B8);
uint16_t tmplowm __at(0x07B6);

uint16_t conversion __at(0x07B4);

uint8_t stickIdx = 0 ;
bool initiatedRead = false;
bool clearRead = false;

// This function multiplies the input by the multiplier, then
// it divides by 265. Amazing solution by slinjim :)
// Bitshifting 8 is equal to dividing by 256 (close enough)
// 
unsigned char slinjim(uint16_t dividend, uint16_t multiplier)
{
    // We must subtract the deadzone amount to get the proper output range.
    conversion = dividend * (multiplier+1) >> 8;
    return (unsigned char) conversion;
}

// This function generates a multiplier allowing
// scaling to a 0 out of 100 value. The output is
// the numerator over a denominator of value 256.
uint16_t getmultiplier(uint16_t high, uint16_t low)
{
    // We want the range to only be equal to the range deadzone.
    // This will ensure we have the full range of motion between
    // the deadzone beginning and the stick maximum.
    uint16_t distance = high - low;
    uint16_t ratio = ((10000U / distance) * 256U)/100U;
    
    return ratio;
}



void scansticks(void) {
    
    // On our first time through this function
    // we can initiate the first read
    if (!initiatedRead)
    {
        ADPCH = channel_VSS;
        ADCON0bits.ADGO = 1;
        while (ADCON0bits.ADGO)
        {}
        
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
                
            
                // Get the ADC reading and bitshift 3 times to scale it.
                // Todo: just set the register to do this scaling automatically :)
                adc_read = ((adc_result_t)((ADRESH << 8) + ADRESL)) >> 3;
                
                // Set ADC read channel to ground read to clear the
                // charge capacitor for the next read.
                ADPCH = channel_VSS;
                ADCON0bits.ADGO = 1;

                // Perform a different operation depending
                // on which stick we should be reading
                switch (stickIdx)
                {
                    case 0: // Stick X

                        tmphigh     = SettingData.sxhigh;
                        tmplow      = SettingData.sxlow;
                        tmpcenter   = SettingData.sxcenter;
                        tmphighm    = SettingData.sx_highMultiplier;
                        tmplowm     = SettingData.sx_lowMultiplier;

                        break;

                    case 1: // Stick Y

                        tmphigh     = SettingData.syhigh;
                        tmplow      = SettingData.sylow;
                        tmpcenter   = SettingData.sycenter;
                        tmphighm    = SettingData.sy_highMultiplier;
                        tmplowm     = SettingData.sy_lowMultiplier;

                        break;

                    case 2: // C-Stick X

                        tmphigh     = SettingData.cxhigh;
                        tmplow      = SettingData.cxlow;
                        tmpcenter   = SettingData.cxcenter;
                        tmphighm    = SettingData.cx_highMultiplier;
                        tmplowm     = SettingData.cx_lowMultiplier;

                        break;

                    case 3: // C-Stick Y

                        tmphigh     = SettingData.cyhigh;
                        tmplow      = SettingData.cylow;
                        tmpcenter   = SettingData.cycenter;
                        tmphighm    = SettingData.cy_highMultiplier;
                        tmplowm     = SettingData.cy_lowMultiplier;

                        break;

                };

                // Determine if we are in the upper or lower stick range
                // then we perform our stick value conversion and store
                // the value in our outgoing byte.

                if (adc_read > tmpcenter + SettingData.deadZone)
                {
                    tmpcenter += SettingData.deadZone;
                    tmpcenter--;
                    if (adc_read >= tmphigh)
                    {
                        gConPollPacket[stickIdx+2] = 228U;
                    }
                    else
                    {
                        gConPollPacket[stickIdx+2] = 129U + slinjim(adc_read - tmpcenter, tmphighm);
                    }
                }
                else if (adc_read < tmpcenter - SettingData.deadZone)
                {
                    tmpcenter -= SettingData.deadZone;
                    tmpcenter--;
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
        ADPCH = channel_VSS;
        ADCON0bits.ADGO = 1;
        while (ADCON0bits.ADGO)
        {}
        
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
            
                // Get the ADC reading and bitshift 3 times to scale it.
                // Todo: just set the register to do this scaling automatically :)
                adc_read = ((adc_result_t)((ADRESH << 8) + ADRESL)) >> 3;
                
                // Set ADC read channel to ground read to clear the
                // charge capacitor for the next read.
                ADPCH = channel_VSS;
                ADCON0bits.ADGO = 1;

                // Perform a different operation depending
                // on which stick we should be reading
                switch (stickIdx)
                {
                    case 0: // Stick X

                        if (adc_read > SettingData.sxhigh)
                        {
                            SettingData.sxhigh = adc_read;
                        }
                        
                        if (adc_read < SettingData.sxlow)
                        {
                            SettingData.sxlow = adc_read;
                        }
                        
                        SettingData.sxcenter = adc_read;

                        break;

                    case 1: // Stick Y

                        if (adc_read > SettingData.syhigh)
                        {
                            SettingData.syhigh = adc_read;
                        }
                        
                        if (adc_read < SettingData.sylow)
                        {
                            SettingData.sylow = adc_read;
                        }
                        
                        SettingData.sycenter = adc_read;

                        break;

                    case 2: // C-Stick X

                        if (adc_read > SettingData.cxhigh)
                        {
                            SettingData.cxhigh = adc_read;
                        }
                        
                        if (adc_read < SettingData.cxlow)
                        {
                            SettingData.cxlow = adc_read;
                        }
                        
                        SettingData.cxcenter = adc_read;

                        break;

                    case 3: // C-Stick Y

                        if (adc_read > SettingData.cyhigh)
                        {
                            SettingData.cyhigh = adc_read;
                        }
                        if (adc_read < SettingData.cylow)
                        {
                            SettingData.cylow = adc_read;
                        }
                        
                        SettingData.cycenter = adc_read;

                        break;

                };

            }
        }
 
    }
    
    return; 
}
