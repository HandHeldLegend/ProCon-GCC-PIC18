/*
 * File:   button.c
 * Author: Mitch
 *
 * Created on December 22, 2021, 8:11 PM
 */


#include <xc.h>
#include "main.h"

void checkbuttons(void)
{
    // DEBUG
    //INTCON0bits.GIEH = 0;
    // END DEBUG
    
    if (!(gInStatus & (1 << 1))) // Check if first button has not been read
    {
        gConPollPacket[BUTTON_PORT_A] = 0x0;
        gConPollPacket[BUTTON_PORT_B] = 0x0;
        gConPollPacket[TRIGGER_PORT_L] = 0x0;
        gConPollPacket[TRIGGER_PORT_R] = 0x0;
        asm("BSF    _gInStatus, 1, 0"); // Set the status flag that we read the first button phase
    }
    
    gConPollPacket[BUTTON_PORT_A] ^= A_IN_PORT << 1;
    gConPollPacket[BUTTON_PORT_A] ^= B_IN_PORT << 1;
    gConPollPacket[BUTTON_PORT_A] ^= X_IN_PORT << 1;
    gConPollPacket[BUTTON_PORT_A] ^= Y_IN_PORT << 1;
    gConPollPacket[BUTTON_PORT_A] ^= START_IN_PORT;

}
