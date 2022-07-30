/*
 * File:   gcdata.c
 * Author: Mitch
 *
 * Created on December 5, 2021, 9:29 PM
 */


#include <xc.h>
#include "gcdata.h"



// This is the packet we use when we are
// sending button and stick value updates
// to the Gamecube/Wii/USB Adapter
// each byte is explained below with bits for each item.
volatile unsigned char gConPollPacket[8] = {
        0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00
    };
//byte 0 :: 0, 0, 0, START, Y, X, B, A

//byte 1 :: 1, L-Digital, R-Digital, Z D-up, D-down, D-right, Dleft

//byte 2 :: Analog stick x

//byte 3 :: Analog stick y

//byte 4 :: C stick x

//byte 5 :: C stick y

//byte 6 :: L trigger ANALOG

//byte 7 :: R trigger ANALOG

// This is the canned response we 
volatile unsigned char gConProbeResponse[3] = {
        0x09, 0x00, 0x03
    };

volatile unsigned char gConOriginResponse[10] = {
        0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x02, 0x02
    };

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
volatile unsigned char gInPacket[4] = {0,0,0,0};

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
// b - command interpreted flag
// c - request cleanup flag
// d - sync bit received (tells us the frequency of incoming bits)
// e - sticks read enable flag
// f - buttons read enable flag
// g - button read first loop flag
// h - desync detected flag

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
volatile unsigned char gSamplesLeft = 128;

// Keep track of highest value sample taken.
volatile unsigned char gHighestSample = 0x0;

void gcdatainit(void)
{   
    gConBitCounter = 0x8;
    gInBitCounter = 0x8;
    gInStatus = 0x0;
    gInStatus |= STATUS_DESYNCED;
    
    gInPacketIdx = 0x0;
    
    gCommandOriginMask = 0x41; // Actual command 0x41 for asking for origin/calibration
    gCommandPollMask = 0x40;
    gLowThreshold = 0; 
}

void desyncfix(void) {
    __delay_us(450);
    asm("BSF _gInStatus, 5, 0");
}
