/**
  Generated Interrupt Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    interrupt_manager.h

  @Summary:
    This is the Interrupt Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description:
    This header file provides implementations for global interrupt handling.
    For individual peripheral handlers please see the peripheral driver for
    all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18LF26K83
        Driver Version    :  2.12
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.31 and above or later
        MPLAB 	          :  MPLAB X 5.45
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "interrupt_manager.h"
#include "mcc.h"
#include "../gcdata.h"

void  INTERRUPT_Initialize (void)
{
    // Enable Interrupt Priority Vectors
    INTCON0bits.IPEN = 1;

    // Assign peripheral interrupt priority vectors

    // TMRI - high priority
    IPR9bits.TMR6IP = 1;

    // SMTPWAI - high priority
    IPR1bits.SMT1PWAIP = 1;


}

void __interrupt() INTERRUPT_InterruptManagerHigh (void)
{
    asm("MOVFF   SMT1CPWL, _gPulseWidth");
   // interrupt handler
    if(PIE1bits.SMT1PWAIE == 1 && PIR1bits.SMT1PWAIF == 1)
    {
        PIR1bits.SMT1PWAIF = 0;
        TMR6 = 0;
        
        if (gSynced)
        {
            asm("BANKSEL(_gPulseWidth)");
            switch(gInBitCounter)
            {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                    asm("RLNCF _gInStatus, 1, 1");
                    asm("MOVF   _gPulseWidth, 0, 1");
                    asm("CPFSLT  _gLowThreshold, 1");
                    asm("BSF _gInStatus, 0, 1");
                    break;
                case 23:
                    asm("MOVF   _gPulseWidth, 0, 1");
                    asm("CPFSLT  _gLowThreshold, 1");
                    asm("BSF _gRumbleStatus, 0, 1");
                    break;
                case 26:
                    // overflow handle
                    asm("NOP");
                    SMT1CON1bits.SMT1GO = 0;
                    PIR1bits.SMT1PWAIF = 0;
                    SMT1STATbits.CPRUP = 0x1;
                    SMT1STATbits.CPWUP = 0x1;
                    asm("NOP");
                    PIE1bits.SMT1PWAIE = 1;

                    T6CONbits.TMR6ON = 0;

                    gInBitCounter = 0;
                    gInStatus = 0;
                    gPollStatus = POLL_STATUS_NULL;
                    gRumbleStatus = RUMBLE_STATUS_OFF;
                    gSynced = 0;

                    TMR6_Initialize();
                    INTCON0bits.GIEH = 1;
                    SMT1CON1bits.SMT1GO = 1;
                    T6CONbits.TMR6ON = 1;
                    break;
                default:
                    break;
            }
            
            asm("BANKSEL(_gInBitCounter)");
            asm("INCF _gInBitCounter, 1, 1");
            asm("NOP");
        }

    }
    if(PIE9bits.TMR6IE == 1 && PIR9bits.TMR6IF == 1)
    {
        PIR9bits.TMR6IF = 0;
        if (!gSynced)
        {
            SMT1CON1bits.SMT1GO = 0;
            asm("NOP");
            PIR1bits.SMT1PWAIF = 0;
            SMT1STATbits.CPRUP = 0x1;
            SMT1STATbits.CPWUP = 0x1;
            asm("NOP");
            gSynced = 0x1;
            PIE9bits.TMR6IE = 0;
            SMT1CON1bits.SMT1GO = 1;
        }
        
    }
}

/**
 End of File
*/
