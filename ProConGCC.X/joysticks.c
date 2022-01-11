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
    uint16_t conversion = (dividend * multiplier) >> 8;
    return (unsigned char) conversion;
}

// This function generates a multiplier allowing
// scaling to a 0 out of 100 value. The output is
// the numerator over a denominator of value 256.
uint16_t getmultiplier(uint16_t high, uint16_t low)
{
    uint16_t distance = high - low;
    float tmpmultiplier = (100/(float) distance) * 256;
    
    return (uint16_t) tmpmultiplier;
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

                        tmphigh = sx_high;
                        tmplow = sx_low;
                        tmpcenter = sx_center;
                        tmphighm = sx_highMultiplier;
                        tmplowm = sx_lowMultiplier;

                        break;

                    case 1: // Stick Y

                        tmphigh = sy_high;
                        tmplow = sy_low;
                        tmpcenter = sy_center;
                        tmphighm = sy_highMultiplier;
                        tmplowm = sy_lowMultiplier;

                        break;

                    case 2: // C-Stick X

                        tmphigh = cx_high;
                        tmplow = cx_low;
                        tmpcenter = cx_center;
                        tmphighm = cx_highMultiplier;
                        tmplowm = cx_lowMultiplier;

                        break;

                    case 3: // C-Stick Y

                        tmphigh = cy_high;
                        tmplow = cy_low;
                        tmpcenter = cy_center;
                        tmphighm = cy_highMultiplier;
                        tmplowm = cy_lowMultiplier;

                        break;

                };

                // Determine if we are in the upper or lower stick range
                // then we perform our stick value conversion and store
                // the value in our outgoing byte.

                if (adc_read > tmpcenter)
                {
                    if (adc_read >= tmphigh)
                    {
                        gConPollPacket[stickIdx+2] = (unsigned char) 228;
                    }
                    else
                    {
                        gConPollPacket[stickIdx+2] = (unsigned char) 128 + slinjim(adc_read - tmpcenter, tmphighm);
                    }
                }
                else if (adc_read < tmpcenter)
                {
                    if (adc_read <= tmplow)
                    {
                        gConPollPacket[stickIdx+2] = (unsigned char) 27;
                    }
                    else
                    {
                        gConPollPacket[stickIdx+2] = (unsigned char) 128 - slinjim(tmpcenter - adc_read, tmplowm);
                    }
                }
                else
                {
                    gConPollPacket[stickIdx+2] = (unsigned char) 128;
                }

            }
        }
 
    }
    
    return; 
}
