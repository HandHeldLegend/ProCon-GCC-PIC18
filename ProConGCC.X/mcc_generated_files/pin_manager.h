/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18F25K42
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.31 and above
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

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set SX_A aliases
#define SX_A_TRIS                 TRISAbits.TRISA0
#define SX_A_LAT                  LATAbits.LATA0
#define SX_A_PORT                 PORTAbits.RA0
#define SX_A_WPU                  WPUAbits.WPUA0
#define SX_A_OD                   ODCONAbits.ODCA0
#define SX_A_ANS                  ANSELAbits.ANSELA0
#define SX_A_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define SX_A_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define SX_A_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define SX_A_GetValue()           PORTAbits.RA0
#define SX_A_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define SX_A_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define SX_A_SetPullup()          do { WPUAbits.WPUA0 = 1; } while(0)
#define SX_A_ResetPullup()        do { WPUAbits.WPUA0 = 0; } while(0)
#define SX_A_SetPushPull()        do { ODCONAbits.ODCA0 = 0; } while(0)
#define SX_A_SetOpenDrain()       do { ODCONAbits.ODCA0 = 1; } while(0)
#define SX_A_SetAnalogMode()      do { ANSELAbits.ANSELA0 = 1; } while(0)
#define SX_A_SetDigitalMode()     do { ANSELAbits.ANSELA0 = 0; } while(0)

// get/set SY_A aliases
#define SY_A_TRIS                 TRISAbits.TRISA1
#define SY_A_LAT                  LATAbits.LATA1
#define SY_A_PORT                 PORTAbits.RA1
#define SY_A_WPU                  WPUAbits.WPUA1
#define SY_A_OD                   ODCONAbits.ODCA1
#define SY_A_ANS                  ANSELAbits.ANSELA1
#define SY_A_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define SY_A_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define SY_A_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define SY_A_GetValue()           PORTAbits.RA1
#define SY_A_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define SY_A_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define SY_A_SetPullup()          do { WPUAbits.WPUA1 = 1; } while(0)
#define SY_A_ResetPullup()        do { WPUAbits.WPUA1 = 0; } while(0)
#define SY_A_SetPushPull()        do { ODCONAbits.ODCA1 = 0; } while(0)
#define SY_A_SetOpenDrain()       do { ODCONAbits.ODCA1 = 1; } while(0)
#define SY_A_SetAnalogMode()      do { ANSELAbits.ANSELA1 = 1; } while(0)
#define SY_A_SetDigitalMode()     do { ANSELAbits.ANSELA1 = 0; } while(0)

// get/set CX_A aliases
#define CX_A_TRIS                 TRISAbits.TRISA2
#define CX_A_LAT                  LATAbits.LATA2
#define CX_A_PORT                 PORTAbits.RA2
#define CX_A_WPU                  WPUAbits.WPUA2
#define CX_A_OD                   ODCONAbits.ODCA2
#define CX_A_ANS                  ANSELAbits.ANSELA2
#define CX_A_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define CX_A_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define CX_A_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define CX_A_GetValue()           PORTAbits.RA2
#define CX_A_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define CX_A_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define CX_A_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define CX_A_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define CX_A_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define CX_A_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define CX_A_SetAnalogMode()      do { ANSELAbits.ANSELA2 = 1; } while(0)
#define CX_A_SetDigitalMode()     do { ANSELAbits.ANSELA2 = 0; } while(0)

// get/set CY_A aliases
#define CY_A_TRIS                 TRISAbits.TRISA3
#define CY_A_LAT                  LATAbits.LATA3
#define CY_A_PORT                 PORTAbits.RA3
#define CY_A_WPU                  WPUAbits.WPUA3
#define CY_A_OD                   ODCONAbits.ODCA3
#define CY_A_ANS                  ANSELAbits.ANSELA3
#define CY_A_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define CY_A_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define CY_A_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define CY_A_GetValue()           PORTAbits.RA3
#define CY_A_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define CY_A_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define CY_A_SetPullup()          do { WPUAbits.WPUA3 = 1; } while(0)
#define CY_A_ResetPullup()        do { WPUAbits.WPUA3 = 0; } while(0)
#define CY_A_SetPushPull()        do { ODCONAbits.ODCA3 = 0; } while(0)
#define CY_A_SetOpenDrain()       do { ODCONAbits.ODCA3 = 1; } while(0)
#define CY_A_SetAnalogMode()      do { ANSELAbits.ANSELA3 = 1; } while(0)
#define CY_A_SetDigitalMode()     do { ANSELAbits.ANSELA3 = 0; } while(0)

// get/set A_IN aliases
#define A_IN_TRIS                 TRISAbits.TRISA4
#define A_IN_LAT                  LATAbits.LATA4
#define A_IN_PORT                 PORTAbits.RA4
#define A_IN_WPU                  WPUAbits.WPUA4
#define A_IN_OD                   ODCONAbits.ODCA4
#define A_IN_ANS                  ANSELAbits.ANSELA4
#define A_IN_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define A_IN_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define A_IN_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define A_IN_GetValue()           PORTAbits.RA4
#define A_IN_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define A_IN_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define A_IN_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define A_IN_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define A_IN_SetPushPull()        do { ODCONAbits.ODCA4 = 0; } while(0)
#define A_IN_SetOpenDrain()       do { ODCONAbits.ODCA4 = 1; } while(0)
#define A_IN_SetAnalogMode()      do { ANSELAbits.ANSELA4 = 1; } while(0)
#define A_IN_SetDigitalMode()     do { ANSELAbits.ANSELA4 = 0; } while(0)

// get/set B_IN aliases
#define B_IN_TRIS                 TRISAbits.TRISA5
#define B_IN_LAT                  LATAbits.LATA5
#define B_IN_PORT                 PORTAbits.RA5
#define B_IN_WPU                  WPUAbits.WPUA5
#define B_IN_OD                   ODCONAbits.ODCA5
#define B_IN_ANS                  ANSELAbits.ANSELA5
#define B_IN_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define B_IN_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define B_IN_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define B_IN_GetValue()           PORTAbits.RA5
#define B_IN_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define B_IN_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define B_IN_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define B_IN_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)
#define B_IN_SetPushPull()        do { ODCONAbits.ODCA5 = 0; } while(0)
#define B_IN_SetOpenDrain()       do { ODCONAbits.ODCA5 = 1; } while(0)
#define B_IN_SetAnalogMode()      do { ANSELAbits.ANSELA5 = 1; } while(0)
#define B_IN_SetDigitalMode()     do { ANSELAbits.ANSELA5 = 0; } while(0)

// get/set Y_IN aliases
#define Y_IN_TRIS                 TRISAbits.TRISA6
#define Y_IN_LAT                  LATAbits.LATA6
#define Y_IN_PORT                 PORTAbits.RA6
#define Y_IN_WPU                  WPUAbits.WPUA6
#define Y_IN_OD                   ODCONAbits.ODCA6
#define Y_IN_ANS                  ANSELAbits.ANSELA6
#define Y_IN_SetHigh()            do { LATAbits.LATA6 = 1; } while(0)
#define Y_IN_SetLow()             do { LATAbits.LATA6 = 0; } while(0)
#define Y_IN_Toggle()             do { LATAbits.LATA6 = ~LATAbits.LATA6; } while(0)
#define Y_IN_GetValue()           PORTAbits.RA6
#define Y_IN_SetDigitalInput()    do { TRISAbits.TRISA6 = 1; } while(0)
#define Y_IN_SetDigitalOutput()   do { TRISAbits.TRISA6 = 0; } while(0)
#define Y_IN_SetPullup()          do { WPUAbits.WPUA6 = 1; } while(0)
#define Y_IN_ResetPullup()        do { WPUAbits.WPUA6 = 0; } while(0)
#define Y_IN_SetPushPull()        do { ODCONAbits.ODCA6 = 0; } while(0)
#define Y_IN_SetOpenDrain()       do { ODCONAbits.ODCA6 = 1; } while(0)
#define Y_IN_SetAnalogMode()      do { ANSELAbits.ANSELA6 = 1; } while(0)
#define Y_IN_SetDigitalMode()     do { ANSELAbits.ANSELA6 = 0; } while(0)

// get/set X_IN aliases
#define X_IN_TRIS                 TRISAbits.TRISA7
#define X_IN_LAT                  LATAbits.LATA7
#define X_IN_PORT                 PORTAbits.RA7
#define X_IN_WPU                  WPUAbits.WPUA7
#define X_IN_OD                   ODCONAbits.ODCA7
#define X_IN_ANS                  ANSELAbits.ANSELA7
#define X_IN_SetHigh()            do { LATAbits.LATA7 = 1; } while(0)
#define X_IN_SetLow()             do { LATAbits.LATA7 = 0; } while(0)
#define X_IN_Toggle()             do { LATAbits.LATA7 = ~LATAbits.LATA7; } while(0)
#define X_IN_GetValue()           PORTAbits.RA7
#define X_IN_SetDigitalInput()    do { TRISAbits.TRISA7 = 1; } while(0)
#define X_IN_SetDigitalOutput()   do { TRISAbits.TRISA7 = 0; } while(0)
#define X_IN_SetPullup()          do { WPUAbits.WPUA7 = 1; } while(0)
#define X_IN_ResetPullup()        do { WPUAbits.WPUA7 = 0; } while(0)
#define X_IN_SetPushPull()        do { ODCONAbits.ODCA7 = 0; } while(0)
#define X_IN_SetOpenDrain()       do { ODCONAbits.ODCA7 = 1; } while(0)
#define X_IN_SetAnalogMode()      do { ANSELAbits.ANSELA7 = 1; } while(0)
#define X_IN_SetDigitalMode()     do { ANSELAbits.ANSELA7 = 0; } while(0)

// get/set DR_IN aliases
#define DR_IN_TRIS                 TRISBbits.TRISB0
#define DR_IN_LAT                  LATBbits.LATB0
#define DR_IN_PORT                 PORTBbits.RB0
#define DR_IN_WPU                  WPUBbits.WPUB0
#define DR_IN_OD                   ODCONBbits.ODCB0
#define DR_IN_ANS                  ANSELBbits.ANSELB0
#define DR_IN_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define DR_IN_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define DR_IN_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define DR_IN_GetValue()           PORTBbits.RB0
#define DR_IN_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define DR_IN_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define DR_IN_SetPullup()          do { WPUBbits.WPUB0 = 1; } while(0)
#define DR_IN_ResetPullup()        do { WPUBbits.WPUB0 = 0; } while(0)
#define DR_IN_SetPushPull()        do { ODCONBbits.ODCB0 = 0; } while(0)
#define DR_IN_SetOpenDrain()       do { ODCONBbits.ODCB0 = 1; } while(0)
#define DR_IN_SetAnalogMode()      do { ANSELBbits.ANSELB0 = 1; } while(0)
#define DR_IN_SetDigitalMode()     do { ANSELBbits.ANSELB0 = 0; } while(0)

// get/set DL_IN aliases
#define DL_IN_TRIS                 TRISBbits.TRISB1
#define DL_IN_LAT                  LATBbits.LATB1
#define DL_IN_PORT                 PORTBbits.RB1
#define DL_IN_WPU                  WPUBbits.WPUB1
#define DL_IN_OD                   ODCONBbits.ODCB1
#define DL_IN_ANS                  ANSELBbits.ANSELB1
#define DL_IN_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define DL_IN_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define DL_IN_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define DL_IN_GetValue()           PORTBbits.RB1
#define DL_IN_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define DL_IN_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define DL_IN_SetPullup()          do { WPUBbits.WPUB1 = 1; } while(0)
#define DL_IN_ResetPullup()        do { WPUBbits.WPUB1 = 0; } while(0)
#define DL_IN_SetPushPull()        do { ODCONBbits.ODCB1 = 0; } while(0)
#define DL_IN_SetOpenDrain()       do { ODCONBbits.ODCB1 = 1; } while(0)
#define DL_IN_SetAnalogMode()      do { ANSELBbits.ANSELB1 = 1; } while(0)
#define DL_IN_SetDigitalMode()     do { ANSELBbits.ANSELB1 = 0; } while(0)

// get/set GC_DATA aliases
#define GC_DATA_TRIS                 TRISBbits.TRISB2
#define GC_DATA_LAT                  LATBbits.LATB2
#define GC_DATA_PORT                 PORTBbits.RB2
#define GC_DATA_WPU                  WPUBbits.WPUB2
#define GC_DATA_OD                   ODCONBbits.ODCB2
#define GC_DATA_ANS                  ANSELBbits.ANSELB2
#define GC_DATA_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define GC_DATA_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define GC_DATA_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define GC_DATA_GetValue()           PORTBbits.RB2
#define GC_DATA_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define GC_DATA_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define GC_DATA_SetPullup()          do { WPUBbits.WPUB2 = 1; } while(0)
#define GC_DATA_ResetPullup()        do { WPUBbits.WPUB2 = 0; } while(0)
#define GC_DATA_SetPushPull()        do { ODCONBbits.ODCB2 = 0; } while(0)
#define GC_DATA_SetOpenDrain()       do { ODCONBbits.ODCB2 = 1; } while(0)
#define GC_DATA_SetAnalogMode()      do { ANSELBbits.ANSELB2 = 1; } while(0)
#define GC_DATA_SetDigitalMode()     do { ANSELBbits.ANSELB2 = 0; } while(0)

// get/set L_IN aliases
#define L_IN_TRIS                 TRISBbits.TRISB5
#define L_IN_LAT                  LATBbits.LATB5
#define L_IN_PORT                 PORTBbits.RB5
#define L_IN_WPU                  WPUBbits.WPUB5
#define L_IN_OD                   ODCONBbits.ODCB5
#define L_IN_ANS                  ANSELBbits.ANSELB5
#define L_IN_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define L_IN_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define L_IN_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define L_IN_GetValue()           PORTBbits.RB5
#define L_IN_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define L_IN_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define L_IN_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define L_IN_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define L_IN_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define L_IN_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define L_IN_SetAnalogMode()      do { ANSELBbits.ANSELB5 = 1; } while(0)
#define L_IN_SetDigitalMode()     do { ANSELBbits.ANSELB5 = 0; } while(0)

// get/set R_IN aliases
#define R_IN_TRIS                 TRISCbits.TRISC0
#define R_IN_LAT                  LATCbits.LATC0
#define R_IN_PORT                 PORTCbits.RC0
#define R_IN_WPU                  WPUCbits.WPUC0
#define R_IN_OD                   ODCONCbits.ODCC0
#define R_IN_ANS                  ANSELCbits.ANSELC0
#define R_IN_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define R_IN_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define R_IN_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define R_IN_GetValue()           PORTCbits.RC0
#define R_IN_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define R_IN_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define R_IN_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define R_IN_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define R_IN_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
#define R_IN_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
#define R_IN_SetAnalogMode()      do { ANSELCbits.ANSELC0 = 1; } while(0)
#define R_IN_SetDigitalMode()     do { ANSELCbits.ANSELC0 = 0; } while(0)

// get/set START_IN aliases
#define START_IN_TRIS                 TRISCbits.TRISC1
#define START_IN_LAT                  LATCbits.LATC1
#define START_IN_PORT                 PORTCbits.RC1
#define START_IN_WPU                  WPUCbits.WPUC1
#define START_IN_OD                   ODCONCbits.ODCC1
#define START_IN_ANS                  ANSELCbits.ANSELC1
#define START_IN_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define START_IN_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define START_IN_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define START_IN_GetValue()           PORTCbits.RC1
#define START_IN_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define START_IN_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define START_IN_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define START_IN_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define START_IN_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define START_IN_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define START_IN_SetAnalogMode()      do { ANSELCbits.ANSELC1 = 1; } while(0)
#define START_IN_SetDigitalMode()     do { ANSELCbits.ANSELC1 = 0; } while(0)

// get/set ZR_IN aliases
#define ZR_IN_TRIS                 TRISCbits.TRISC2
#define ZR_IN_LAT                  LATCbits.LATC2
#define ZR_IN_PORT                 PORTCbits.RC2
#define ZR_IN_WPU                  WPUCbits.WPUC2
#define ZR_IN_OD                   ODCONCbits.ODCC2
#define ZR_IN_ANS                  ANSELCbits.ANSELC2
#define ZR_IN_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define ZR_IN_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define ZR_IN_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define ZR_IN_GetValue()           PORTCbits.RC2
#define ZR_IN_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define ZR_IN_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define ZR_IN_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define ZR_IN_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define ZR_IN_SetPushPull()        do { ODCONCbits.ODCC2 = 0; } while(0)
#define ZR_IN_SetOpenDrain()       do { ODCONCbits.ODCC2 = 1; } while(0)
#define ZR_IN_SetAnalogMode()      do { ANSELCbits.ANSELC2 = 1; } while(0)
#define ZR_IN_SetDigitalMode()     do { ANSELCbits.ANSELC2 = 0; } while(0)

// get/set IO_RC3 aliases
#define IO_RC3_TRIS                 TRISCbits.TRISC3
#define IO_RC3_LAT                  LATCbits.LATC3
#define IO_RC3_PORT                 PORTCbits.RC3
#define IO_RC3_WPU                  WPUCbits.WPUC3
#define IO_RC3_OD                   ODCONCbits.ODCC3
#define IO_RC3_ANS                  ANSELCbits.ANSELC3
#define IO_RC3_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define IO_RC3_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define IO_RC3_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define IO_RC3_GetValue()           PORTCbits.RC3
#define IO_RC3_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define IO_RC3_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define IO_RC3_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define IO_RC3_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define IO_RC3_SetPushPull()        do { ODCONCbits.ODCC3 = 0; } while(0)
#define IO_RC3_SetOpenDrain()       do { ODCONCbits.ODCC3 = 1; } while(0)
#define IO_RC3_SetAnalogMode()      do { ANSELCbits.ANSELC3 = 1; } while(0)
#define IO_RC3_SetDigitalMode()     do { ANSELCbits.ANSELC3 = 0; } while(0)

// get/set IO_RC4 aliases
#define IO_RC4_TRIS                 TRISCbits.TRISC4
#define IO_RC4_LAT                  LATCbits.LATC4
#define IO_RC4_PORT                 PORTCbits.RC4
#define IO_RC4_WPU                  WPUCbits.WPUC4
#define IO_RC4_OD                   ODCONCbits.ODCC4
#define IO_RC4_ANS                  ANSELCbits.ANSELC4
#define IO_RC4_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define IO_RC4_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define IO_RC4_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define IO_RC4_GetValue()           PORTCbits.RC4
#define IO_RC4_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define IO_RC4_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define IO_RC4_SetPullup()          do { WPUCbits.WPUC4 = 1; } while(0)
#define IO_RC4_ResetPullup()        do { WPUCbits.WPUC4 = 0; } while(0)
#define IO_RC4_SetPushPull()        do { ODCONCbits.ODCC4 = 0; } while(0)
#define IO_RC4_SetOpenDrain()       do { ODCONCbits.ODCC4 = 1; } while(0)
#define IO_RC4_SetAnalogMode()      do { ANSELCbits.ANSELC4 = 1; } while(0)
#define IO_RC4_SetDigitalMode()     do { ANSELCbits.ANSELC4 = 0; } while(0)

// get/set ZL_IN aliases
#define ZL_IN_TRIS                 TRISCbits.TRISC5
#define ZL_IN_LAT                  LATCbits.LATC5
#define ZL_IN_PORT                 PORTCbits.RC5
#define ZL_IN_WPU                  WPUCbits.WPUC5
#define ZL_IN_OD                   ODCONCbits.ODCC5
#define ZL_IN_ANS                  ANSELCbits.ANSELC5
#define ZL_IN_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define ZL_IN_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define ZL_IN_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define ZL_IN_GetValue()           PORTCbits.RC5
#define ZL_IN_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define ZL_IN_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define ZL_IN_SetPullup()          do { WPUCbits.WPUC5 = 1; } while(0)
#define ZL_IN_ResetPullup()        do { WPUCbits.WPUC5 = 0; } while(0)
#define ZL_IN_SetPushPull()        do { ODCONCbits.ODCC5 = 0; } while(0)
#define ZL_IN_SetOpenDrain()       do { ODCONCbits.ODCC5 = 1; } while(0)
#define ZL_IN_SetAnalogMode()      do { ANSELCbits.ANSELC5 = 1; } while(0)
#define ZL_IN_SetDigitalMode()     do { ANSELCbits.ANSELC5 = 0; } while(0)

// get/set DU_IN aliases
#define DU_IN_TRIS                 TRISCbits.TRISC6
#define DU_IN_LAT                  LATCbits.LATC6
#define DU_IN_PORT                 PORTCbits.RC6
#define DU_IN_WPU                  WPUCbits.WPUC6
#define DU_IN_OD                   ODCONCbits.ODCC6
#define DU_IN_ANS                  ANSELCbits.ANSELC6
#define DU_IN_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define DU_IN_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define DU_IN_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define DU_IN_GetValue()           PORTCbits.RC6
#define DU_IN_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define DU_IN_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define DU_IN_SetPullup()          do { WPUCbits.WPUC6 = 1; } while(0)
#define DU_IN_ResetPullup()        do { WPUCbits.WPUC6 = 0; } while(0)
#define DU_IN_SetPushPull()        do { ODCONCbits.ODCC6 = 0; } while(0)
#define DU_IN_SetOpenDrain()       do { ODCONCbits.ODCC6 = 1; } while(0)
#define DU_IN_SetAnalogMode()      do { ANSELCbits.ANSELC6 = 1; } while(0)
#define DU_IN_SetDigitalMode()     do { ANSELCbits.ANSELC6 = 0; } while(0)

// get/set DD_IN aliases
#define DD_IN_TRIS                 TRISCbits.TRISC7
#define DD_IN_LAT                  LATCbits.LATC7
#define DD_IN_PORT                 PORTCbits.RC7
#define DD_IN_WPU                  WPUCbits.WPUC7
#define DD_IN_OD                   ODCONCbits.ODCC7
#define DD_IN_ANS                  ANSELCbits.ANSELC7
#define DD_IN_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define DD_IN_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define DD_IN_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define DD_IN_GetValue()           PORTCbits.RC7
#define DD_IN_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define DD_IN_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define DD_IN_SetPullup()          do { WPUCbits.WPUC7 = 1; } while(0)
#define DD_IN_ResetPullup()        do { WPUCbits.WPUC7 = 0; } while(0)
#define DD_IN_SetPushPull()        do { ODCONCbits.ODCC7 = 0; } while(0)
#define DD_IN_SetOpenDrain()       do { ODCONCbits.ODCC7 = 1; } while(0)
#define DD_IN_SetAnalogMode()      do { ANSELCbits.ANSELC7 = 1; } while(0)
#define DD_IN_SetDigitalMode()     do { ANSELCbits.ANSELC7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCBF2 pin functionality
 * @Example
    IOCBF2_ISR();
 */
void IOCBF2_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCBF2 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCBF2 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF2_SetInterruptHandler(MyInterruptHandler);

*/
void IOCBF2_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCBF2 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCBF2_SetInterruptHandler() method.
    This handler is called every time the IOCBF2 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF2_SetInterruptHandler(IOCBF2_InterruptHandler);

*/
extern void (*IOCBF2_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCBF2 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCBF2_SetInterruptHandler() method.
    This handler is called every time the IOCBF2 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF2_SetInterruptHandler(IOCBF2_DefaultInterruptHandler);

*/
void IOCBF2_DefaultInterruptHandler(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/