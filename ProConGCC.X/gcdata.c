/*
 * File:   gcdata.c
 * Author: Mitch
 *
 * Created on December 5, 2021, 9:29 PM
 */


#include <xc.h>
#include "gcdata.h"


volatile unsigned char gPollPacket[8] = {
        0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00
    };
volatile unsigned char gProbeResponse[3] = {
        0x09, 0x00, 0x03
    };

volatile unsigned char gOriginResponse[10] = {
        0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x02, 0x02
    };

volatile unsigned char gOutByte;
volatile unsigned char gOutBytesLeft = 0;
volatile unsigned char gOutBitCounter = 8;

volatile unsigned char gInByte = 0x0;
volatile unsigned char gInRumble = 0x0;
volatile unsigned char gInBitCounter = 8;
volatile unsigned char gInBytesLeft = 1;

volatile unsigned char gPulseWidth;
volatile unsigned char gLowThreshold = 50; 
volatile unsigned char gInStatus; 

volatile unsigned char gFSR0ptr = 0x0;

void gcdatainit(void)
{   
    gLowThreshold = 50; 
}

