// PIC18F25K42 Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>
#include "main.h"

// Bool to tell if we are calibrating the sticks
bool stickcalibration = false;

void main(void)
{
    asm("MOVLW _gInPacket");
    asm("MOVFF WREG, _gInPacketIdx");
    // Initialize the device
    SYSTEM_Initialize();
    
    // Load default config
    loadsettings();
    
    // Set ADC Read channel to RA0
    ADPCH = 0x00;
    // Turn on the ADC module
    ADCON0bits.ADON = 1;
    
    // Do stick calibration if Y/X/A are all held on boot.
    if (!Y_IN_PORT && !X_IN_PORT && !A_IN_PORT)
    {
        stickcalibration = true;
        // Zero out our config options to the default center values
        zerosticks();
    }
    
    while (stickcalibration)
    {
        calibratesticks();
        
        // Pressing start saves settings
        if (!START_IN_PORT)
        {
            setstickmultipliers();
            savesettings();
            stickcalibration = false;
            break;
        }
    }
    
    // Enable interrupts
    INTCON0bits.GIEH = 1;
    
    while (1)
    {
        // check for incoming command
        commandreader();
        // check if we need to send a response
        bytepush();
        // check if we need to do a cleanup
        bytecleanup();
        
        // if the check stick bit is set, scan the sticks
        if (gInStatus & (1 << 3))
        {
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