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
    T6CONbits.TMR6ON = 1;
    
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
    
    // Clear out the MSB of mode data
    // as we already extracted whether
    // xbox mode is on/off
    SettingData.modeData &= ~(1 << 7);
    
    // Enable interrupts
    INTCON0bits.GIEH = 1;
    
    // Main loop for running the controller function
    while (1)
    {
        
        // if the check stick bit is set, scan the sticks
        // handle starting or stopping rumble before scanning sticks
        if (0)
        {
            
            // check if rumble should be enables
            if(0 && SettingData.rumbleData)
            {
                PORTBbits.RB4 = 1;
                CCPR1H = 0xF0;
            }
            // if rumble byte = 0x02 then we should brake the rumble
            else if (0 == 2 && SettingData.rumbleData)
            {
                PORTBbits.RB4 = 1;
                CCPR1H = 0x60;
            }
            // otherwise turn rumble OFF
            else
            {
                PORTBbits.RB4 = 0;
            }
            
            SMT1CON1bits.SMT1GO = 0x1;
            // scan the sticks
            scansticks();
            // check for setting changes
            livesettings();
            
        }
        
        // if the button check bit is set, scan the buttons
        if (0)
        {
            checkbuttons();
        }
    }
}
/**
 End of File
*/
