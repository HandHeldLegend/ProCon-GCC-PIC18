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
    
    if ( gPollStatus == POLL_STATUS_BUTTONS_FIRST ) // Check if first button has not been read
    {
        // Zero the outgoing button status bytes
        // Since we are reading a fresh state.
        gPollPacket[BUTTON_PORT_A] = 0x0;
        gPollPacket[BUTTON_PORT_B] = 0x80; // leftmost bit always 1
        gPollPacket[TRIGGER_PORT_L] = 0x0;
        gPollPacket[TRIGGER_PORT_R] = 0x0;
        gPollStatus = POLL_STATUS_BUTTONS_LOOP;
    }
    
    // We or/equals the outgoing port bits against the 
    // INVERSE of incoming button bits shifted into proper positioning
    // This allows for very quick formation. Buttons pressed give a 1, depressed give 0. EZ :)
    // Port A output form: 0, 0 ,0 , Start : 1, Y : 1, X : 1, B : 1, A : 1 Credit to simple
    // Shift left one bit to get ready for setting the next bit
    gPollPacket[BUTTON_PORT_A] |= !START_IN_PORT << 4;
    
    if (!xboxmode)
    {
        gPollPacket[BUTTON_PORT_A] |= !Y_IN_PORT << 3;
        gPollPacket[BUTTON_PORT_A] |= !X_IN_PORT << 2;
        gPollPacket[BUTTON_PORT_A] |= !B_IN_PORT << 1;
        gPollPacket[BUTTON_PORT_A] |= !A_IN_PORT;
    }
    // Xbox layout mode
    else
    {
        gPollPacket[BUTTON_PORT_A] |= !X_IN_PORT << 3;
        gPollPacket[BUTTON_PORT_A] |= !Y_IN_PORT << 2;
        gPollPacket[BUTTON_PORT_A] |= !A_IN_PORT << 1;
        gPollPacket[BUTTON_PORT_A] |= !B_IN_PORT;
    }
    
    
    // END PORT A output bits, maximum result would be 00011111 for all port a buttons pressed
    
    // Port B Output Form: 1, L : 1 , R : 1, Z : 1, D-Up : 1, D-Dn : 1, D-Rt : 1, D-Lt : 1 Credit to simple :)
    
    if (SettingData.modeData == 0)
    {
        // ZL/ZR act as digital press and half analog press
        gPollPacket[BUTTON_PORT_B]   |= !L_IN_PORT << 6;
        gPollPacket[TRIGGER_PORT_L]  |= (!L_IN_PORT) ? 0x80 : 0x00;

        gPollPacket[BUTTON_PORT_B]   |= !R_IN_PORT << 5;
        gPollPacket[TRIGGER_PORT_R]  |= (!R_IN_PORT) ? 0x80 : 0x00;

        // Don't shift for the first check for the Z button (L/R mapped to Z button)
        gPollPacket[BUTTON_PORT_B]   |= !ZL_IN_PORT << 4;
        gPollPacket[BUTTON_PORT_B]   |= !ZR_IN_PORT << 4;
    }
    else if (SettingData.modeData == 1)
    {
        //ZL acts as digital and half analog press
        gPollPacket[BUTTON_PORT_B]   |= !L_IN_PORT << 6;
        gPollPacket[TRIGGER_PORT_L]  |= (!L_IN_PORT) ? 0x80 : 0x00;

        // ZR is digital press+analog, R is half analog press
        gPollPacket[BUTTON_PORT_B]   |= !R_IN_PORT << 5;
        gPollPacket[TRIGGER_PORT_R]  |= (!R_IN_PORT) ? 0x80 : 0x00;
        
        gPollPacket[TRIGGER_PORT_R]  |= (!ZR_IN_PORT) ? 0x80 : 0x00;

        // L mapped to Z
        gPollPacket[BUTTON_PORT_B]   |= !ZL_IN_PORT << 4;
    }
    else if (SettingData.modeData == 2)
    {
        //ZL acts as digital+analog, L acts as analog half press
        gPollPacket[BUTTON_PORT_B]   |= !L_IN_PORT << 6;
        gPollPacket[TRIGGER_PORT_L]  |= (!L_IN_PORT) ? 0x80 : 0x00;
        
        gPollPacket[TRIGGER_PORT_L]  |= (!ZL_IN_PORT) ? 0x80 : 0x00;

        // ZR is digital press and half analog press
        gPollPacket[BUTTON_PORT_B]   |= !R_IN_PORT << 5;
        gPollPacket[TRIGGER_PORT_R]  |= (!R_IN_PORT) ? 0x80 : 0x00;

        // R mapped to Z
        gPollPacket[BUTTON_PORT_B]   |= !ZR_IN_PORT << 4;
    }
    else if (SettingData.modeData == 3)
    {
        //ZL acts as digital+analog, L acts as analog half press
        gPollPacket[BUTTON_PORT_B]   |= !L_IN_PORT << 6;
        gPollPacket[TRIGGER_PORT_L]  |= (!L_IN_PORT) ? 0x80 : 0x00;
        
        gPollPacket[TRIGGER_PORT_L]  |= (!ZL_IN_PORT) ? 0x80 : 0x00;

        // ZR acts as digital+analog, R acts as analog half press
        gPollPacket[BUTTON_PORT_B]   |= !R_IN_PORT << 5;
        gPollPacket[TRIGGER_PORT_R]  |= (!R_IN_PORT) ? 0x80 : 0x00;
        
        gPollPacket[TRIGGER_PORT_R]  |= (!ZR_IN_PORT) ? 0x80 : 0x00;
    }
    
    else if (SettingData.modeData == 4)
    {
        // ZL/ZR act as digital press and half analog press
        gPollPacket[BUTTON_PORT_B]   |= !L_IN_PORT << 6;
        gPollPacket[TRIGGER_PORT_L]  |= (!L_IN_PORT) ? 0x80 : 0x00;

        gPollPacket[BUTTON_PORT_B]   |= !R_IN_PORT << 5;
        gPollPacket[TRIGGER_PORT_R]  |= (!R_IN_PORT) ? 0x80 : 0x00;

        // Don't shift for the first check for the Z button (L/R mapped to Z button)
        gPollPacket[BUTTON_PORT_B]   |= !DL_IN_PORT << 4;
        // L mapped to Left Dpad
        gPollPacket[BUTTON_PORT_B]   |= !ZL_IN_PORT;
        gPollPacket[BUTTON_PORT_B]   |= !ZR_IN_PORT << 4;
    }
    
    gPollPacket[BUTTON_PORT_B] |= !DU_IN_PORT << 3;
    gPollPacket[BUTTON_PORT_B] |= !DD_IN_PORT << 2;
    
    gPollPacket[BUTTON_PORT_B] |= !DR_IN_PORT << 1;
    
    if (SettingData.modeData != 4)
    {
        gPollPacket[BUTTON_PORT_B] |= !DL_IN_PORT;
    }

}
