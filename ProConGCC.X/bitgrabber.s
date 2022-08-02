#include <xc.inc>

psect   barfunc,local,class=CODE,reloc=2 ; PIC18
    
; Variables for handling incoming data
GLOBAL	_gInBitCounter	; Counts the bits down from 8
GLOBAL	_gInPacket	; This is an array for the 3 possible incoming bytes. Reserved 4th slot for overflow protecc.
GLOBAL	_gInPacketIdx	; A storage space for a pointer to the current byte in the gInPacket array
GLOBAL	_gConByteCount	; Keep track of byte count

GLOBAL	_gInStatus	; A single byte which stores some important status data
GLOBAL	_gLowThreshold	; The threshold which determines a low from high pulse
GLOBAL	_gInPulseWidth	; A separate container to store the incoming pulse length for later comparison
    
GLOBAL	_gSamplesLeft  ; How many samples have been taken so far  
GLOBAL	_gHighestSample	; Keep track of highest sample taken
    
GLOBAL _bitgrabber ; extern of bar function goes in the C source file
_bitgrabber:
    
    BANKSEL(PIE1)
    BTFSS   PIE1, 7, 1	 ; Check if interrupt flag on. Skip if it is
    RETURN
    
    BTFSS   PIR1, 7, 1	 ; Check if we got a pulse width acquisition. Skip if yes
    RETURN
    BCF	    PIR1, 7, 1
    
    BANKSEL(_gInPacketIdx)
    
    MOVLW   0x5		 ; Move 5 into WREG
    CPFSLT  _gInPacketIdx
    
    
    
    MOVFF   BANKMASK(_gInPacketIdx), FSR0	; Copy the address into FSR0 for indirect addressing via INDF0
    
    BTFSC   BANKMASK(_gInStatus), 0, 1			; Check to see if we have synced. Skip if clear.
    GOTO    SYNCSET
	
	BTFSC   BANKMASK(_gInBitCounter), 3, 1	; Check if the bit counter is 8 (start of new byte). Skip if 8 bit is not on.
	GOTO    STOPCHECK		; We are checking a beginning bit. Check if stop bit
	
	    RLNCF   INDF0, 1, 0			    ; Rotate the current byte to the left by 1.
	    ; Here we check one of the body bits making up the byte. going to be HIGH or LOW
	    BCF	    INDF0, 0, 0			    ; Zero the rightmost bit.
	    MOVF    BANKMASK(_gInPulseWidth), 0, 1  ; Copy pulse width to WREG
	    
	    ; This compares gLowThreshold to the captured pulse width.
	    ; If the low threshold is more than the pulse width, the pulse was short
	    ; meaning we have a HIGH bit.
	    CPFSLT  BANKMASK(_gLowThreshold), 1		; if the pulse is more than the low threshold, Skip, keeping the bit LOW.
	    BSF	    INDF0, 0, 0			; Set the rightmost bit HIGH
	    
	    DECFSZ	BANKMASK(_gInBitCounter), 1, 1	; Decrement the bit counter. 8 bits total. Skip if 0
	    RETURN

	; Set up the next byte
	INCF	BANKMASK(_gInPacketIdx), 1, 1	; Increment our address pointer to get the next byte
	DCFSNZ	BANKMASK(_gConByteCount, 1, 1	; Decrement byte counter. Skip if NOT zero
	GOTO	COMMANDCOMPLETE			; If we do this, we have 5 bytes which is an overflow!
	
	BSF	BANKMASK(_gInBitCounter), 3, 1	; Set the bit counter to 0x8 by flagging one byte
	
	RETURN				; We're done until the next bit read. Return. No cleanup yet.
	
    STOPCHECK:

	MOVF	BANKMASK(_gInPulseWidth), 0, 1		; Copy pulse width to WREG
	    
	; This compares gLowThreshold to the captured pulse width.
	; If the low threshold is more than the pulse width, the pulse was short
	; meaning we have a STOP bit.
	CPFSLT  BANKMASK(_gLowThreshold), 1		; if the pulse is more than the low threshold, Skip, keeping the bit LOW.
	GOTO	COMMANDCOMPLETE			; We got a short pulse, to go complete command.
	
	; If we got here, Cleanup for next bit
	BCF	 INDF0, 0, 0			; Zero the rightmost bit since it will always be zero.
	DECF	BANKMASK(_gInBitCounter), 1, 1		; Decrement the bit counter
	RETURN

    COMMANDCOMPLETE:
	; If we made it here, our command is fully built and
	; we should mark it for the interpreter
	BSF	BANKMASK(_gInStatus), 7, 1    ; Set the status flag for stop bit written
	
	BTFSS	BANKMASK(_gInStatus), 1, 1  ; Check if we've run our first button loop Skip if yes.
	RETURN  
	    BCF _gInStatus, 2, 0    ; Clear the button read flag
	    BCF _gInStatus, 1, 0    ; Clear the button first phase flag
	
	RETURN
	
    SYNCSET:
    
    MOVF    BANKMASK(_gInPulseWidth), 0, 1    ; Move Pulse width measurement into WREG
    CPFSLT  BANKMASK(_gHighestSample), 1    ; If pulse is more than the highest sample, skip.
    GOTO STOPWAIT
    
	; If we are here, we need to store the pulse width as the highest sample
	; and we need to set the Low threshold
	MOVFF	_gInPulseWidth, _gLowThreshold	; Copy pulse width to low threshold
	MOVLW	0x8				; Put 8 into WREG
	SUBWF	BANKMASK(_gLowThreshold), 1, 1	; Subtract WREG from low threshold.
	
	MOVFF	_gInPulseWidth, _gHighestSample ; Copy pulse length to highest sample
    
    STOPWAIT:
    
	TSTFSZ  BANKMASK(_gSamplesLeft), 1		; Skip if samples left is 0
	DECF    BANKMASK(_gSamplesLeft), 1, 1		; Subtract one from samples
    
	TSTFSZ  BANKMASK(_gSamplesLeft), 1		; Skip if samples left is 0
	RETURN

	    ; If we're here, we're waiting for a short pulse.
	    MOVF    BANKMASK(_gLowThreshold), 0, 1    ; Move Pulse width measurement into WREG
	    CPFSLT	BANKMASK(_gInPulseWidth), 1 ; If pulse is less than the low threshold, skip!
	    RETURN

		; If we're here, we've set up our low threshold and we
		; have our sync bit. Time to clean up!
		BCF	    BANKMASK(_gInStatus), 0, 1	; Clear the desync flag in status byte
		//BSF	    BANKMASK(_gInStatus), 5, 1  ; Set request cleanup bit in status byte
		RETURN
		
    RETURN
    