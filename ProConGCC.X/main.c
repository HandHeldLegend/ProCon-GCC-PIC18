// PIC18F25K42 Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>
#include "main.h"

// Bool to tell if we are calibrating the sticks
bool stickcalibration = false;
bool xboxmode = false;

void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    
    // Set ADC Read channel to RA0
    ADPCH = 0x00;
    // Turn on the ADC module
    ADCON0bits.ADON = 1;
    
    // Load default config
    loadsettings();
    
    // Load whether xbox mode is enabled
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
    
    // Increase deadzone
    if (!DU_IN_PORT)
    {
        SettingData.deadZone += 4;
        if (SettingData.deadZone >= 28)
        {
            SettingData.deadZone = 28;
        }
        setstickmultipliers();
        savesettings();
        loadsettings();
    }
    
    // Decrease deadzone
    if (!DD_IN_PORT)
    {
        SettingData.deadZone -= 4;
        if (SettingData.deadZone <= 0 || SettingData.deadZone > 28)
        {
            SettingData.deadZone = 0;
        }
        setstickmultipliers();
        savesettings();
        loadsettings();
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
        stickcalibration = true;
        __delay_ms(1200);
        // Zero out our config options to the default center values
        zerosticks();
    }
    
    while (stickcalibration)
    {   
        // Pressing start saves settings
        if (!START_IN_PORT)
        {
            setstickmultipliers();
            savesettings();
            loadsettings();
            stickcalibration = false;
            break;
        }
        else
        {
            calibratesticks();
            __delay_ms(1);
        }
    }
    
    // Enable interrupts
    INTCON0bits.GIEH = 1;
    
    // Clear out the MSB of mode data
    // as we already extracted whether
    // xbox mode is on/off
    SettingData.modeData &= ~(1 << 7);
    
    while (1)
    {
        // check for incoming command
        commandreader();
        // Fix desync if needed
        if (gInStatus & 0x01)
        {
            desyncfix();
        }
        // check if we need to send a response
        bytepush();
        // check if we need to do a cleanup
        bytecleanup();
        
        // if the check stick bit is set, scan the sticks
        // handle starting or stopping rumble before scanning sticks
        if (gInStatus & (1 << 3))
        {
            
            if(gInPacket[2] == 1 && SettingData.rumbleData)
            {
                PORTBbits.RB4 = 1;
                CCPR1H = 0xF0;
            }
            else if (gInPacket[2] == 2 && SettingData.rumbleData)
            {
                PORTBbits.RB4 = 1;
                CCPR1H = 0x60;
            }
            else
            {
                PORTBbits.RB4 = 0;
            }
            
            scansticks();
        }
        
        // if the button check bit is set, scan the buttons
        if (gInStatus & (1 << 2))
        {
            checkbuttons();
        }
    }
}
/**
 End of File
*/
