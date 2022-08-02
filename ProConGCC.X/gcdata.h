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


// This is the packet we use when we are
// sending button and stick value updates
// to the Gamecube/Wii/USB Adapter
// each byte is explained below with bits for each item.
volatile unsigned char gConPollPacket[8];
//byte 0 :: 0, 0, 0, START, Y, X, B, A

//byte 1 :: 1, L-Digital, R-Digital, Z D-up, D-down, D-right, Dleft

//byte 2 :: Analog stick x

//byte 3 :: Analog stick y

//byte 4 :: C stick x

//byte 5 :: C stick y

//byte 6 :: L trigger ANALOG

//byte 7 :: R trigger ANALOG

// This is the canned response we 
volatile unsigned char gConProbeResponse[3];

volatile unsigned char gConOriginResponse[10];

volatile unsigned char gConOutByte;
volatile unsigned char gConOutIdx;

// Use this byte to count down how many bits left
volatile unsigned char gConBitCounter;
// Use this byte to count down how many bytes left
volatile unsigned char gConByteCount;


// Use this as a way to count down how many
// bits until we have a full byte.
volatile unsigned char gInBitCounter;
 
// Byte array for storing incoming bytes.
volatile unsigned char gInPacket[6];

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

// Byte for various status features
// 0 means not happened yet
// 1 means this happened
// Default is cleanup requested to initialize everything.
volatile unsigned char gInStatus; 
// 7 6 5 4 3 2 1 0
// a b c d e f g h
//
// a - stop bit received flag (command received)
#define STATUS_STOP_RECEIVED    (1 << 7)
// b - command interpreted flag
#define STATUS_CMD_INTERP       (1 << 6)
// c - request cleanup flag
#define STATUS_REQUEST_CLEANUP  (1 << 5)

// d - sync bit received (tells us the frequency of incoming bits)
#define STATUS_SYNC_BIT_GOT     (1 << 4)

// e - sticks read enable flag
#define STATUS_READ_STICKS      (1 << 3)
// f - buttons read enable flag
#define STATUS_READ_BUTTON      (1 << 2)
// g - button read first loop flag
#define STATUS_FIRST_LOOP_OK    (1 << 1)
// h - desync detected flag
#define STATUS_DESYNCED         (1)

// This is the first byte that comes in.
// It tells the controller which command to respond to.
volatile unsigned char gInCommandByte;

// Set command bits for our reference.
volatile unsigned char gCommandOriginMask; // Actual command 0x41 for asking for origin/calibration
volatile unsigned char gCommandPollMask; // Actual command 0x40 for asking for a button/stick updates

// Used to keep track of polls so we can ensure
// we've sampled enough data to ensure
// that we have a valid pulse length to 
// differentiate between high/low pulses
volatile unsigned char gSamplesLeft;

// Keep track of highest value sample taken.
volatile unsigned char gHighestSample;

void gcdatainit(void);

void desyncfix(void);

#endif	/* GC_DATA_H*/

