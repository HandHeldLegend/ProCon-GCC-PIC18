#include <xc.inc>

psect   barfunc,local,class=CODE,reloc=2 ; PIC18
    
; Variables for handling incoming data
GLOBAL	_gInBitCounter	; Counts the bits down from 8
GLOBAL	_gInPacket	; This is an array for the 3 possible incoming bytes. Reserved 4th slot for overflow protecc.
GLOBAL	_gInPacketIdx	; A storage space for a pointer to the current byte in the gInPacket array

GLOBAL	_gInStatus	; A single byte which stores some important status data
GLOBAL	_gLowThreshold	; The threshold which determines a low from high pulse
GLOBAL	_gInPulseWidth	; A separate container to store the incoming pulse length for later comparison
    
GLOBAL _bitgrabber ; extern of bar function goes in the C source file
_bitgrabber:

    MOVFF   _gInPacketIdx, FSR0	; Copy the address into FSR0 for indirect addressing via INDF0
    ; Don't set the bit since we aren't putting anything there anyways
    
    BTFSC   _gInBitCounter, 3, 0
    GOTO    FIRSTBIT		; We are out of bits. Set up for the next byte
	
	RLNCF	INDF0, 1, 0	; Rotate the current byte to the left by 1.
	; This is when checking any other bit
	BCF	 INDF0, 0, 0	; Zero the rightmost bit.
	
	MOVFF   _gInPulseWidth, WREG
	; This compares gLowThreshold to the captured pulse width.
	; If the low threshold is more than the pulse width, the pulse was short
	; meaning we have a HIGH bit.
	CPFSLT  _gLowThreshold, 0   ; if the pulse is more than the low threshold, Skip.
	BSF	INDF0, 0, 0	    ; Set the rightmost bit.
	
	GOTO	BITCOMPLETE
   

    FIRSTBIT:
	MOVFF    _gInPulseWidth, WREG
	; This compares gLowThreshold to the captured pulse width.
	; If the low threshold is more than the pulse width, the pulse was short
	; meaning we have a HIGH bit.
	CPFSLT  _gLowThreshold, 0   ; if the pulse is more than the low threshold, Skip.
	GOTO	COMMANDCOMPLETE
	
	BCF	INDF0, 0, 0	    ; Zero the rightmost bit.

    BITCOMPLETE:    ; Run this after a bit is set to prepare us for the next bit
	DCFSNZ	_gInBitCounter, 1, 0	; Decrement the bit counter. 8 bits total. Skip if we're not at 0
	GOTO	NEXTBYTE
	
	RETURN				; RETURN. Not time for cleanup yet so we leave everything as-is.

    NEXTBYTE:
	INCF	_gInPacketIdx, 1, 0	; Increment our address pointer to get the next byte
	BSF	_gInBitCounter, 3, 0	; Set the bit counter to 0x8 by flagging one byte
	
	RETURN				; We're done until the next bit read. Return. No cleanup yet.

    COMMANDCOMPLETE:
	; If we made it here, our command is fully built and
	; we should mark it for the interpreter
	BSF	_gInStatus, 7, 0    ; Set the status flag for stop bit written
	
	RETURN