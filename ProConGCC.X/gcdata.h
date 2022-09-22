/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef GC_DATA_H
#define	GC_DATA_H

#include <xc.h> // include processor files - each processor file is guarded.
#include "main.h"
#include "memorymap.h"

#define STICK_SX_PACKET 2
#define STICK_SY_PACKET 3

#define STICK_CX_PACKET 4
#define STICK_CY_PACKET 5

#define POLL_STATUS_NULL            0x0
#define POLL_STATUS_STICKS          0x1
#define POLL_STATUS_BUTTONS_FIRST   0x2
#define POLL_STATUS_BUTTONS_LOOP    0x3

#define RUMBLE_STATUS_OFF           0x0
#define RUMBLE_STATUS_EN            0x1
#define RUMBLE_STATUS_BRAKE         0x2

// This is the packet we use when we are
// sending button and stick value updates
// to the Gamecube/Wii/USB Adapter
// each byte is explained below with bits for each item.
volatile unsigned char gPollPacket[8];
//byte 0 :: 0, 0, 0, START, Y, X, B, A
//byte 1 :: 1, L-Digital, R-Digital, Z D-up, D-down, D-right, Dleft
//byte 2 :: Analog stick x
//byte 3 :: Analog stick y
//byte 4 :: C stick x
//byte 5 :: C stick y
//byte 6 :: L trigger ANALOG
//byte 7 :: R trigger ANALOG

// This is the canned response we use for probe
volatile unsigned char gProbeResponse[3];

// This is the canned response we use for origin
volatile unsigned char gOriginResponse[10];

// Used as a placeholder to store the byte
// we are pushing
volatile unsigned char gOutByte;

// Count down how many bytes left we have
volatile unsigned char gOutBytesLeft;

// Use this as a way to count down how many
// bits until we have a full byte sending data.
volatile unsigned char gOutBitCounter;

// store incoming rumble data
volatile unsigned char gInRumble;
// Use this byte to count down how many bits left
// coming IN.
volatile unsigned char gInBitCounter;
// Use this byte to count down how many bytes left
// coming IN
volatile unsigned char gInBytesLeft;


// This holds the pointer to the current byte that is coming in
// Using permanently is unreliable, so we should store it in
// between interrupts.
volatile unsigned char gInPacketIdx;


// The low threshold to discern between a
// LOW and a HIGH bit (0 or 1).
// Another interesting tidbit; there will never be a '1'
// in the leftmost position coming back from the Gamecube
// or Wii console. I suspect this is to avoid getting
// HIGH bits mixed up with LOW bits since they share a 1us
// low pulse.

// A way to store the incoming pulse width
// reliably. Otherwise it can get overwritten.
volatile unsigned char gInPulseWidth;

// The low threshold is updated at the beginning of every
// incoming packet to handle differences in incoming transmission speed.
volatile unsigned char gLowThreshold; 

// Byte for keeping track of
// which command is received.
volatile unsigned char gInStatus; 

// Keep track of rumble being on.
volatile unsigned char gRumbleStatus;

// Keep track of current polling cycle.
volatile unsigned char gPollStatus;

volatile unsigned char gSynced;

void gcdatainit(void);

void desyncfix(void);

#endif	/* GC_DATA_H*/

