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


// This is the packet we use when we are
// sending button and stick value updates
// to the Gamecube/Wii/USB Adapter
// each byte is explained below with bits for each item.
volatile unsigned char gConPollPacket[8];

// This is the canned response we 
volatile unsigned char gConProbeResponse[3];

volatile unsigned char gConOriginResponse[10];

volatile unsigned char gConOutByte;

// Use this byte to count down how many bits left
volatile unsigned char gConBitCounter;
// Use this byte to count down how many bytes left
volatile unsigned char gConByteCount;
volatile unsigned char gConOutIdx;

// --------------------
// --------------------

// This section is for variables for incoming bits/bytes
volatile unsigned char gInBitBuffer[60];
volatile unsigned char gInBitBufferIdx = 0x0;

// Use this as a way to count down how many
// bits until we have a full byte.
volatile unsigned char gInBitCounter;

// A way to store the incoming pulse width
// reliably. Otherwise it can get overwritten.
volatile unsigned char gInPulseWidth;
 
// Byte array for storing incoming bytes.
volatile unsigned char gInPacket[4];

// This is the first byte that comes in.
// It tells the controller which command to respond to.
volatile unsigned char gInCommandByte;

// This holds the pointer to the current byte that is coming in
// Using permanently is unreliable, so we should store it in
// between interrupts.
volatile unsigned char gInPacketIdx;


// Byte for various status features
// 0 means not happened yet
// 1 means this happened
volatile unsigned char gInStatus;

// The low threshold to discern between a
// LOW and a HIGH bit (0 or 1).
// Another interesting tidbit; there will never be a '1'
// in the leftmost position coming back from the Gamecube
// or Wii console. I suspect this is to avoid getting
// HIGH bits mixed up with LOW bits since they share a 1us
// low pulse.
volatile unsigned char gLowThreshold;


// We use a bitmask to do XOR operation, increment one, and check for 0 (overflow to 0))
// A ZERO result confirms our command!
// Set command bits for our convenience.
volatile unsigned char gCommandOriginMask; // Actual command 0x41 for asking for origin/calibration
volatile unsigned char gCommandPollMask; // Actual command 0x40 for asking for a button/stick update

void desyncfix(void);

#endif	/* GC_DATA_H*/

