/*
 * File:   joysticks.c
 * Author: Mitch
 *
 * Created on December 15, 2021, 12:05 AM
 */

#include <xc.h>
#include "joysticks.h"

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

uint16_t x_samples_total = 0x00;
uint16_t y_samples_total = 0x00;
uint8_t x_samples[AVERAGE_COUNT];
uint8_t y_samples[AVERAGE_COUNT];
uint8_t sb_old_ptr = 0;
uint8_t sb_new_ptr = 22;
uint8_t snapback_sample_setup = FALSE;
uint8_t x_sample_avg = 0;
uint8_t y_sample_avg = 0;

void joysticksetup(void)
{
    stickIdx = 0x00;
    initiatedRead = 0x00;
    clearRead = 0x00;
    
    adc_read = 0x00;
    tmphigh = 0x00;
    tmplow = 0x00;
    tmpcenter = 0x00;
    tmphighm = 0x00;
    tmplowm = 0x00;
}

uint8_t average_count = AVERAGE_COUNT;
// Controls how much snapback reduction filter is applied
uint8_t fader_val = 0;

uint8_t fastfilter(uint8_t original, uint8_t filtered, uint8_t fader)
{
    uint32_t tmp = 0;
    
    switch(fader)
    {
        // No filter
        case 0:
            tmp = original;
            break;
            
        // 8 + 1f = 9
        case 1:
            tmp = ( ( (original*8) + filtered ) * 28 ) >> 8;
            break;
        // 5 + 1f = 6
        case 2:
            tmp = ( ( (original*5) + filtered ) * 42 ) >> 8;
            break;
        // 3 + 1f = 4
        case 3:
            tmp = ( (original*3) + filtered ) >> 2;
            break;
        // 2.5 + 1f = 3.5
        case 4:
            tmp = ( ( (original*2) + (original >> 1) + filtered ) * 73 ) >> 8;
            break;
        // 2 + 1f = 3
        case 5:
            tmp = ( ( (original*2) + filtered ) * 85 ) >> 8;
            break;
        // 1.5 + 1f = 2.5
        case 6:
            tmp = ( ( (original) + (original >> 1) + filtered ) * 102 ) >> 8;
            break;
        // 1 and 1 -- even
        case 7:
            tmp = (original + filtered) >> 1;
            break;
        // 1.5 + 1
        case 8:
            tmp = ( ( (filtered) + (filtered >> 1) + original ) * 102 ) >> 8;
            break; 
        // 2 + 1
        case 9:
            tmp = ( ( (filtered*2) + original ) * 85 ) >> 8;
            break;
        // 2.5 + 1
        case 10:
            tmp = ( ( (filtered*2) + (filtered >> 1) + original ) * 73 ) >> 8;
            break;
        // 3 + 1
        case 11:
            tmp = ( (filtered*3) + original ) >> 2;
            break;
        // 5 + 1
        case 12:
            tmp = ( ( (filtered*5) + original ) * 42 ) >> 8;
            break;
        // 8 + 1
        case 13:
            tmp = ( ( (filtered*8) + original ) * 28 ) >> 8;
            break;
            
        // Full filter
        case 14:
            tmp = filtered;
            break;
            
        // Not valid
        default:
            tmp = original;
    }
    asm("NOP");
    return (uint8_t) tmp;
}

uint8_t fastdivide(uint16_t input, uint8_t divisor)
{
    uint32_t tmp;
    switch(divisor)
    {
        case 0:
            return 0;
        case 1:
            return input;
        case 2:
            return input >> 1;
        case 3:
            tmp = input*85;
            return tmp >> 8;
        case 4:
            return input >> 2;
        case 5:
            tmp = input*51;
            return tmp >> 8;
        case 6:
            tmp = input*43;
            return tmp >> 8;
        case 7:
            tmp = input*37;
            return tmp >> 8;
        case 8:
            return input >> 3;
        case 9:
            tmp = input*28;
            return tmp >> 8;
        case 10:
            tmp = input*25;
            return tmp >> 8;
        case 11:
            tmp = input*23;
            return tmp >> 8;
        case 12:
            tmp = input*21;
            return tmp >> 8;
        case 13:
            tmp = input*20;
            return tmp >> 8;
        case 14:
            tmp = input*18;
            return tmp >> 8;
        case 15:
            tmp = input*17;
            return tmp >> 8;
        case 16:
            return input >> 4;
        case 17:
            tmp = input*15;
            return tmp >> 8;
        case 18:
            tmp = input*14;
            return tmp >> 8;
        case 19:
            tmp = input*13;
            return tmp >> 8;
        case 20:
            tmp = input*13;
            return tmp >> 8;
        case 21:
            tmp = input*12;
            return tmp >> 8;
        case 22:
            tmp = input*11;
            return tmp >> 8;
        default:
            return input;   
    }
}


uint8_t changeprime = 0x0;
void applysnapback(void)
{

    // Set up our samples if the rate changes
    if (!snapback_sample_setup)
    {
        x_samples_total = average_count * gPollPacket[STICK_SX_PACKET];
        y_samples_total = average_count * gPollPacket[STICK_SY_PACKET];
        sb_old_ptr = 0;
        sb_new_ptr = average_count-1;
        snapback_sample_setup = TRUE;
        x_sample_avg = gPollPacket[STICK_SX_PACKET];
        y_sample_avg = gPollPacket[STICK_SY_PACKET];
                
        for (uint8_t i = 0; i < average_count; i++)
        {
            x_samples[i] = gPollPacket[STICK_SX_PACKET];
            y_samples[i] = gPollPacket[STICK_SY_PACKET];
        }
    }
    // Our sample rate has been set up, apply averages accordingly
    else
    {
        // Remove the value of the old sample
        x_samples_total -= x_samples[sb_old_ptr];
        y_samples_total -= y_samples[sb_old_ptr];
        
        // increase the new sample ptr value
        sb_new_ptr += 1;
        if (sb_new_ptr > average_count-1) sb_new_ptr = 0;
        
        // First add the value of the new sample
        x_samples_total += gPollPacket[STICK_SX_PACKET];
        x_samples[sb_new_ptr] = gPollPacket[STICK_SX_PACKET];
        y_samples_total += gPollPacket[STICK_SY_PACKET];
        y_samples[sb_new_ptr] = gPollPacket[STICK_SY_PACKET];
        
        // increase the old sample ptr value
        sb_old_ptr +=1;
        if (sb_old_ptr > average_count-1) sb_old_ptr = 0;
        
        // Get the average and primary filter value
        x_sample_avg = fastdivide(x_samples_total, average_count);
        y_sample_avg = fastdivide(y_samples_total, average_count);
        
        if ( gPollPacket[STICK_SX_PACKET] > 128 || gPollPacket[STICK_SX_PACKET] < 126)
        {
            gPollPacket[STICK_SX_PACKET] = fastfilter(gPollPacket[STICK_SX_PACKET], x_sample_avg, SettingData.x_snapback_strength);
        }
        if ( gPollPacket[STICK_SY_PACKET] > 128 || gPollPacket[STICK_SY_PACKET] < 126)
        {
            gPollPacket[STICK_SY_PACKET] = fastfilter(gPollPacket[STICK_SY_PACKET], y_sample_avg, SettingData.y_snapback_strength);
        }
    }
    
}

// This function multiplies the input by the multiplier, then
// it divides by 265. Amazing solution by slinjim :)
// Bitshifting 8 is equal to dividing by 256 (close enough)
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


// The ADC probably takes the most time
// of all operations. This is to ensure that
// the scanning gives as smooth of a result as possible.
// 
// First the ADC pin is scanned, then the ADC is connected to ground to
// discharge the ADC charge capacitor. The ADC pin is changed to the next
// stick axis, and it's checked again. This process repeats.
// Once the ADC read starts, it happens all on the peripheral side.
// Rather than blocking everything just to wait for the ADC to finish, we can
// perform other operations such as scaling the stick input. The ADC will be 
// done by the time we get back :)
void scansticks(void) {
    
    // On our first time through this function
    // we can initiate the first read
    if (!initiatedRead)
    {
        ADPCH = channel_VSS; 
        ADCON0bits.ADGO = 1;
        // Wait until ADC read is done for the first time through.
        while (ADCON0bits.ADGO)
        {
        }
        
        // 
        ADPCH = SX_A;           // Set the ADC to the channel for Stick X (see adcc.h)
        ADCON0bits.ADGO = 1;    // Set the ADC conversion to GO. (runs on hardware nonblocking)
        stickIdx = 0;           // Set the stick index to 0
        clearRead = FALSE;
        initiatedRead = TRUE;   // Set the bool to true indicating we already started our first read
    }
    
    // We are here if we already initiated a read
    else
    {
        if (!ADCON0bits.ADGO) // A conversion is completed, otherwise skip
        {
            if (clearRead) // We performed a blank read to clear the ADC cap charge.
            {
                switch(stickIdx)
                {
                    // Set up stick read L Y axis
                    case 0:
                        // Set the ADC Read Channel bit
                        ADPCH = SY_A;
                        break;
                    
                    // Set up stick read C X axis
                    case 1:
                        ADPCH = CX_A;
                        break;
                        
                    // Set up stick read C Y axis
                    case 2:
                        ADPCH = CY_A;
                        break;
                        
                    // Finalize read operation and clear flags to let button scan go.
                    case 3:
                        // Check if we're calibrating snapback
                        applysnapback();
                        gPollStatus = POLL_STATUS_BUTTONS_FIRST;
                        initiatedRead = FALSE;       // Reset the bool to initialize a read
                        clearRead = FALSE;
                        
                        //DEBUG
                        // This will pull the line low once the last read is completed.
                        //asm("BCF	    TRISB, 2");
                        //asm("BCF	PORTB, 2");
                        //asm("NOP");
                        //END DEBUG

                        return;
                        
                }
                // Increase the stick index for the next read.
                stickIdx++;
                clearRead = FALSE;
                ADCON0bits.ADGO = 1;                
            }
            else    // We are reading a non-blank answer
            {
                clearRead = TRUE;
                
                uint16_t adc_out = ((adc_result_t)((ADRESH << 8) + ADRESL)) >> 3;
                // Get the ADC reading and bitshift 3 times to scale it.
                // Todo: just set the register to do this scaling automatically :)
                adc_read = adc_out;
                
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
                        gPollPacket[stickIdx+2] = 228U;
                    }
                    else
                    {
                        gPollPacket[stickIdx+2] = 129U + slinjim(adc_read - tmpcenter, tmphighm);
                    }
                }
                else if (adc_read < tmpcenter - SettingData.deadZone)
                {
                    tmpcenter -= SettingData.deadZone;
                    tmpcenter--;
                    if (adc_read <= tmplow)
                    {
                        gPollPacket[stickIdx+2] = 28U;
                    }
                    else
                    {
                        gPollPacket[stickIdx+2] = 127U - slinjim(tmpcenter - adc_read, tmplowm);
                    }
                }
                else
                {
                    gPollPacket[stickIdx+2] = STICK_CENTER_GCC;
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
        clearRead = FALSE;
        initiatedRead = TRUE;   // Set the bool to true indicating we already started our first read
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
                        initiatedRead = FALSE;       // Reset the bool to initialize a read
                        clearRead = FALSE;
                        return;
                        
                }
                // Increase the stick index for the next read.
                stickIdx++;
                clearRead = FALSE;
                ADCON0bits.ADGO = 1;                
            }
            else    // We are reading a non-blank answer
            {
                clearRead = TRUE;
            
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
