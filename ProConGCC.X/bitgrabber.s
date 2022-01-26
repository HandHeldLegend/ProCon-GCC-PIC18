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
    
    BTFSS   _gInStatus, 4, 0			; Check to see if we have set our sync bit. Skip if set.
    GOTO    SYNCBIT
    
	BTFSC   _gInBitCounter, 3, 0	; Check if the bit counter is 8 (start of new byte). Skip if 8 bit is not on.
	GOTO    STOPCHECK		; We are checking a beginning bit. LOW or STOP
	    
	    RLNCF   INDF0, 1, 0			; Rotate the current byte to the left by 1.
	    ; Here we check one of the body bits making up the byte. going to be HIGH or LOW
	    BCF	    INDF0, 0, 0			; Zero the rightmost bit.
	    MOVFF   _gInPulseWidth, WREG	; Copy pulse width to WREG
	    
	    ; This compares gLowThreshold to the captured pulse width.
	    ; If the low threshold is more than the pulse width, the pulse was short
	    ; meaning we have a HIGH bit.
	    CPFSLT  _gLowThreshold, 0		; if the pulse is more than the low threshold, Skip, keeping the bit LOW.
	    BSF	    INDF0, 0, 0			; Set the rightmost bit HIGH
	    
	    DECFSZ	_gInBitCounter, 1, 0	; Decrement the bit counter. 8 bits total. Skip if 0
	    RETURN

	; Set up the next byte
	INCF	_gInPacketIdx, 1, 0	; Increment our address pointer to get the next byte
	BSF	_gInBitCounter, 3, 0	; Set the bit counter to 0x8 by flagging one byte
	
	RETURN				; We're done until the next bit read. Return. No cleanup yet.

    SYNCBIT:
	; This is the sync process
	; We get the pulse width, copy it to the low threshold variable
	; then we subtract 8 to represent a threshold of X - 0.5 microseconds;
	; This should be sufficient enough to account for any possible errors.
	; Bit pulse speed can change slightly based on a variety of compounding factors
	; so this allows us to address it quickly and at the start of every incoming
	; packet.
	
	MOVLW	0x08			; Copy the number 8 into WREG
	SUBWF	_gInPulseWidth, 0, 0	; Subtract 8 from Pulse width
	MOVWF	_gLowThreshold, 0	; Move result to threshold
	
	BSF	_gInStatus, 4, 0		; Set the sync bit in the status
	BCF	INDF0, 0, 0			; Zero the rightmost bit since it will always be zero.
	DECF	_gInBitCounter, 1, 0		; Decrement the bit counter
	RETURN
	
    STOPCHECK:
	
	MOVFF   _gInPulseWidth, WREG	; Copy pulse width to WREG
	    
	; This compares gLowThreshold to the captured pulse width.
	; If the low threshold is more than the pulse width, the pulse was short
	; meaning we have a STOP bit.
	CPFSLT  _gLowThreshold, 0		; if the pulse is more than the low threshold, Skip, keeping the bit LOW.
	GOTO	COMMANDCOMPLETE			; We got a short pulse, to go complete command.
	
	; If we got here, Cleanup for next bit
	BCF	 INDF0, 0, 0			; Zero the rightmost bit since it will always be zero.
	DECF	_gInBitCounter, 1, 0		; Decrement the bit counter
	RETURN

    COMMANDCOMPLETE:
	; If we made it here, our command is fully built and
	; we should mark it for the interpreter
	BSF	_gInStatus, 7, 0    ; Set the status flag for stop bit written
	
	RETURN