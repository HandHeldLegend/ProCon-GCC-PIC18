// PIC18F25K42 Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>
#include "main.h"

// Bool to tell if we are calibrating the sticks
volatile uint8_t stickcalibration    __at(MEM_CALIBRATE_EN);
volatile uint8_t xboxmode            __at(MEM_XBOXMODE);

void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    joysticksetup();
    gcdatainit();
    
    // Set ADC Read channel to RA0
    ADPCH = 0x00;
    // Turn on the ADC module
    ADCON0bits.ADON = 1;
    
    // Load default config
    loadsettings();
    
    stickcalibration = STICK_CALIBRATE_NOPE;
    //stickcalibration = STICK_CALIBRATE_SNAP;
    
    // Load whether xbox mode is enabled
    // Don't forget to clear it first because dumb
    xboxmode = FALSE;
    xboxmode |= SettingData.modeData >> 7;
    
    // Set Default Trigger Mode
    if (!L_IN_PORT && !R_IN_PORT)
    {
        SettingData.modeData = 0 + (xboxmode << 7);
        savesettings();
    }
    else if (!L_IN_PORT && !ZL_IN_PORT)
    {
        SettingData.modeData = 2 + (xboxmode << 7);
        savesettings();
    }
    else if (!R_IN_PORT && !ZR_IN_PORT)
    {
        SettingData.modeData = 1 + (xboxmode << 7);
        savesettings();
    }
    else if (!ZR_IN_PORT && !ZL_IN_PORT)
    {
        SettingData.modeData = 3 + (xboxmode << 7);
        savesettings();
    }
    
    // CUSTOMER MODE REQUEST
    else if (!B_IN_PORT)
    {
        SettingData.modeData = 4 + (xboxmode << 7);
        savesettings();
    }
    // Set or unset Xbox button layout
    else if (!Y_IN_PORT)
    {
        xboxmode = !xboxmode;
        SettingData.modeData &= ~(1 << 7);
        SettingData.modeData |= xboxmode << 7;
        savesettings(); 
    }
    
    // Set defaults
    if (!SELECT_IN_PORT && !START_IN_PORT)
    {
        setdefaultsettings();
        savesettings();
    }
    // Enable/Disable rumble mode
    else if (!START_IN_PORT)
    {
        SettingData.rumbleData = !SettingData.rumbleData;
        savesettings();
    }
    // Do stick calibration if Minus or Capture is held on boot.
    else if (!SELECT_IN_PORT)
    {
        stickcalibration = STICK_CALIBRATE_AXIS;
        __delay_ms(1200);
        // Zero out our config options to the default center values
        zerosticks();
    }
    
    while (stickcalibration == STICK_CALIBRATE_AXIS)
    {   
        // Pressing start saves settings
        if (!START_IN_PORT)
        {
            setstickmultipliers();
            savesettings();
            loadsettings();
            stickcalibration = STICK_CALIBRATE_NOPE;
            break;
        }
        else
        {
            calibratesticks();
            __delay_ms(1);
        }
    }
    
    __delay_ms(1);
    
    // Clear out the MSB of mode data
    // as we already extracted whether
    // xbox mode is on/off
    SettingData.modeData &= ~(1 << 7);
    
    INTCON0bits.GIEH = 1;
    
    T6CONbits.TMR6ON = 1;
    
    while (1)
    {   
        // if the check stick bit is set, scan the sticks
        // handle starting or stopping rumble before scanning sticks
        while (gPollStatus != POLL_STATUS_NULL)
        {
            
            if ( gPollStatus == POLL_STATUS_STICKS ) 
            {
                // check if rumble should be enables
                if(gRumbleStatus == RUMBLE_STATUS_EN && SettingData.rumbleData)
                {
                    PORTBbits.RB4 = 1;
                    CCPR1H = 0xF0;
                }
                else
                {
                    PORTBbits.RB4 = 0;
                }
                scansticks();
                // check for setting changes
                livesettings();
            }
                
            checkbuttons();
            
            if ( gInBitCounter == 25 && gInStatus == 0x40 )
            {
                // Poll command
                INTCON0bits.GIEH = 0;
                SMT1CON1bits.SMT1GO = 0;
                sendpoll();
                gInBitCounter = 0;
                gInStatus = 0;
                SMT1STATbits.CPWUP = 0x1;
                gPollStatus = POLL_STATUS_STICKS;
                INTCON0bits.GIEH = 1;
                SMT1CON1bits.SMT1GO = 1;
                break;
            }
            else if ( (gInBitCounter > 9) && !(gInStatus & 0x40) )
            {
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
                break;
            }
            
        }
        
        while (gPollStatus == POLL_STATUS_NULL)
        {
            if ( (gInBitCounter == 9) && (gInStatus == 0x41) )
            {
                // Origin command
                INTCON0bits.GIEH = 0;
                SMT1CON1bits.SMT1GO = 0;
                sendorigin();
                gInBitCounter = 0;
                gInStatus = 0;
                SMT1STATbits.CPWUP = 0x1;
                asm("NOP");
                gPollStatus = POLL_STATUS_NULL;
                INTCON0bits.GIEH = 1;
                SMT1CON1bits.SMT1GO = 1;
            }
            else if ( gInBitCounter == 25 )
            {
                // Origin command
                INTCON0bits.GIEH = 0;
                SMT1CON1bits.SMT1GO = 0;
                sendorigin();
                gInBitCounter = 0;
                gInStatus = 0;
                SMT1STATbits.CPWUP = 0x1;
                asm("NOP");
                gPollStatus = POLL_STATUS_NULL;
                INTCON0bits.GIEH = 1;
                SMT1CON1bits.SMT1GO = 1;
            }
            else if ( (gInBitCounter == 9) && (gInStatus == 0x0) )
            {
                // Handle 0x00 probe response
                INTCON0bits.GIEH = 0;
                SMT1CON1bits.SMT1GO = 0;
                // Probe command
                sendprobe();
                gInBitCounter = 0;
                gInStatus = 0;
                SMT1STATbits.CPWUP = 0x1;
                asm("NOP");
                gPollStatus = POLL_STATUS_NULL;
                INTCON0bits.GIEH = 1;
                SMT1CON1bits.SMT1GO = 1;  
            }
            else if ( (gInBitCounter == 9) && (gInStatus == 0xFF) )
            {
                // Handle 0xFF probe response
                INTCON0bits.GIEH = 0;
                SMT1CON1bits.SMT1GO = 0;
                // Probe command
                sendprobe();
                gInBitCounter = 0;
                gInStatus = 0;
                SMT1STATbits.CPWUP = 0x1;
                asm("NOP");
                gPollStatus = POLL_STATUS_NULL;
                INTCON0bits.GIEH = 1;
                SMT1CON1bits.SMT1GO = 1;  
            }
            else if ( gInBitCounter == 25 && gInStatus == 0x40 )
            {
                // Poll command
                INTCON0bits.GIEH = 0;
                SMT1CON1bits.SMT1GO = 0;
                sendpoll();
                gInBitCounter = 0;
                gInStatus = 0;
                gPollStatus = POLL_STATUS_STICKS;
                INTCON0bits.GIEH = 1;
                SMT1CON1bits.SMT1GO = 1;
                break;
            }
            else if ( (gInBitCounter > 9) && !(gInStatus & 0x40) )
            {
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
}
/**
 End of File
*/
