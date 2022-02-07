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
    0x00, 0x00, 0x83, 0x83, 0x7D, 0x7D, 0x00, 0x00
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

volatile unsigned char gConOutByte = 0x0;

// Use this byte to count down how many bits left
volatile unsigned char gConBitCounter = 0x8;
// Use this byte to count down how many bytes left
volatile unsigned char gConByteCount = 0;
volatile unsigned char gConOutIdx = 0x0;

// --------------------
// --------------------

// This section is for variables for incoming bits/bytes
volatile unsigned char gInBitBuffer[60];
volatile unsigned char gInBitBufferIdx;

// Use this as a way to count down how many
// bits until we have a full byte.
volatile unsigned char gInBitCounter = 0x8;

// A way to store the incoming pulse width
// reliably. Otherwise it can get overwritten.
volatile unsigned char gInPulseWidth = 0x0;
 
// Byte array for storing incoming bytes.
volatile unsigned char gInPacket[4] = {
    0x0, 0x0, 0x0, 0x0
};

// This is the first byte that comes in.
// It tells the controller which command to respond to.
volatile unsigned char gInCommandByte = 0x0;

// This holds the pointer to the current byte that is coming in
// Using permanently is unreliable, so we should store it in
// between interrupts.
volatile unsigned char gInPacketIdx = 0x0;


// Byte for various status features
// 0 means not happened yet
// 1 means this happened
// Default is cleanup requested to initialize everything.
volatile unsigned char gInStatus = 0x20; 
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


// The low threshold to discern between a
// LOW and a HIGH bit (0 or 1).
// Another interesting tidbit; there will never be a '1'
// in the leftmost position coming back from the Gamecube
// or Wii console. I suspect this is to avoid getting
// HIGH bits mixed up with LOW bits since they share a 1us
// low pulse.

// The low threshold is updated at the beginning of every
// incoming packet to handle differences in incoming transmission speed.
volatile unsigned char gLowThreshold = 0x2D; 

// Set command bits for our reference.
volatile unsigned char gCommandOriginMask = 0x41; // Actual command 0x41 for asking for origin/calibration
volatile unsigned char gCommandPollMask = 0x40; // Actual command 0x40 for asking for a button/stick update

void desyncfix(void) {
    __delay_us(450);
    asm("BSF _gInStatus, 5, 0");
}
