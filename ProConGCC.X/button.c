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
        // Zero the outgoing button status bytes
        // Since we are reading a fresh state.
        gConPollPacket[BUTTON_PORT_A] = 0x0;
        gConPollPacket[BUTTON_PORT_B] = 0x0;
        gConPollPacket[TRIGGER_PORT_L] = 0x0;
        gConPollPacket[TRIGGER_PORT_R] = 0x0;
        asm("BSF    _gInStatus, 1, 0"); // Set the status flag that we read the first button phase
    }
    
    // We or/equals the outgoing port bits against the 
    // INVERSE of incoming button bits shifted into proper positioning
    // This allows for very quick formation. Buttons pressed give a 1, depressed give 0. EZ :)
    // Port A output form: 0, 0 ,0 , Start : 1, Y : 1, X : 1, B : 1, A : 1 Credit to simple
    // Shift left one bit to get ready for setting the next bit
    gConPollPacket[BUTTON_PORT_A] |= !START_IN_PORT << 4;
    gConPollPacket[BUTTON_PORT_A] |= !Y_IN_PORT << 3;
    gConPollPacket[BUTTON_PORT_A] |= !X_IN_PORT << 2;
    gConPollPacket[BUTTON_PORT_A] |= !B_IN_PORT << 1;
    gConPollPacket[BUTTON_PORT_A] |= !A_IN_PORT;
    
    // END PORT A output bits, maximum result would be 00011111 for all port a buttons pressed
    
    // Port B Output Form: 1, L : 1 , R : 1, Z : 1, D-Up : 1, D-Dn : 1, D-Rt : 1, D-Lt : 1 Credit to simple :)
    gConPollPacket[BUTTON_PORT_B] = 1 << 7;
    gConPollPacket[BUTTON_PORT_B] |= !ZL_IN_PORT << 6;
    gConPollPacket[BUTTON_PORT_B] |= !ZR_IN_PORT << 5;
    
    // Don't shift for the first check for the Z button (L/R mapped to Z button)
    gConPollPacket[BUTTON_PORT_B] |= !L_IN_PORT << 4;
    gConPollPacket[BUTTON_PORT_B] |= !R_IN_PORT << 4;
    
    gConPollPacket[BUTTON_PORT_B] |= !DU_IN_PORT << 3;
    gConPollPacket[BUTTON_PORT_B] |= !DD_IN_PORT << 2;
    gConPollPacket[BUTTON_PORT_B] |= !DR_IN_PORT << 1;
    gConPollPacket[BUTTON_PORT_B] |= !DL_IN_PORT;

}
