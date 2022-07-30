#include <xc.inc>

psect   barfunc,local,class=CODE,reloc=2 ; PIC18
    
; Variables for handling incoming data
GLOBAL	_gInBitCounter	; Counts the bits down from 8
GLOBAL	_gInPacket	; This is an array for the 3 possible incoming bytes. Reserved 4th slot for overflow protecc.
GLOBAL	_gInPacketIdx	; A storage space for a pointer to the current byte in the gInPacket array

GLOBAL	_gInStatus	; A single byte which stores some important status data
GLOBAL	_gLowThreshold	; The threshold which determines a low from high pulse
GLOBAL	_gInPulseWidth	; A separate container to store the incoming pulse length for later comparison
GLOBAL	_gSamplesLeft  ; How many samples have been taken so far  
GLOBAL	_gHighestSample	; Keep track of highest sample taken

GLOBAL _syncsetter ; extern of bar function goes in the C source file
_syncsetter:
    
    BANKSEL(_gInPacketIdx)
    BTFSS   BANKMASK(_gInStatus), 0, 1	; Check to see if we have synced. Skip if set.
    GOTO    BUTTONTASK
    
    MOVF    BANKMASK(_gInPulseWidth), 0, 1    ; Move Pulse width measurement into WREG
    CPFSLT  BANKMASK(_gHighestSample), 1    ; If pulse is more than the highest sample, skip.
    GOTO STOPCHECK
    
	; If we are here, we need to store the pulse width as the highest sample
	; and we need to set the Low threshold
	MOVFF	_gInPulseWidth, _gLowThreshold	; Copy pulse width to low threshold
	MOVLW	0x8				; Put 8 into WREG
	SUBWF	BANKMASK(_gLowThreshold), 1, 1	; Subtract WREG from low threshold.
	
	MOVFF	_gInPulseWidth, _gHighestSample ; Copy pulse length to highest sample
    
    STOPCHECK:
    
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
    
    BUTTONTASK:
	
	BTFSS	BANKMASK(_gInStatus), 1, 1  ; Check if we've run our first button loop Skip if yes.
	RETURN
	    
	    BCF _gInStatus, 2, 0    ; Clear the button read flag
	    BCF _gInStatus, 1, 0    ; Clear the button first phase flag
	    
    RETURN

