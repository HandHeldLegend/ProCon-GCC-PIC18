#include <xc.inc>
    
psect   barfunc,local,class=CODE,reloc=2 ; PIC18

; Variables we are going to work with
GLOBAL _gInStatus
GLOBAL _gInPacket
GLOBAL _gInPacketIdx
    
; Stuff that we should clean up
GLOBAL _gInBitCounter
GLOBAL _gConOutByte
GLOBAL _gConOutIdx
GLOBAL _gConByteCount
GLOBAL _gInCommandByte
GLOBAL _gInPulseWidth
    
GLOBAL _bytecleanup ; extern of bar function goes in the C source file
_bytecleanup:
    
    BANKSEL(_gInStatus)
    ; Check that we finished a packet and need cleanup
    BTFSS	BANKMASK(_gInStatus), 5, 1	; Bit 5 cleanup request flag. Skip if we sent one
    RETURN			; Return if we have nothing to clean up yet.
    
    BANKSEL(_gInPacket)
    ; We gotta reset some variables for our next command read
    MOVLW	_gInPacket	    ; Reset the byte packet index pointer
    MOVFF	WREG, _gInPacketIdx

    CLRF	BANKMASK(_gInBitCounter), 1   ; Clear the bit counter
    BSF		BANKMASK(_gInBitCounter), 3, 1; Set the bit counter to 8 (bit 3)

    CLRF	BANKMASK(_gInStatus), 1	    ; Clear the communication status byte
    CLRF	BANKMASK(_gConOutByte), 1	    ; Clear the outgoing byte container
    CLRF	BANKMASK(_gConOutIdx), 1	    ; Clear the outgoing byte index pointer
    MOVLW	0x5				; Put 5 in the wreg
    MOVWF	BANKMASK(_gConByteCount), 1	; Set byte count to 5 so we can count down
    CLRF	BANKMASK(_gInCommandByte), 1  ; Clear the incoming command bit
    CLRF	BANKMASK(_gInPulseWidth), 1   ; Clear the pulse width counter

    BSF		BANKMASK(_gInStatus), 3, 1    ; Enable stick scan bit in status

    BSF		TRISB, 2, 0	; Clear the data pin from us driving so we can receive
    
    //MOVLB	0x39
    //BCF		0xA1, 7, 1	    ; Clear Pulse width interrupt
    
    MOVLB	0x3F
    //CLRF	0x18, 1		    ; Clear the pulse width capture
    BSF		0xD2, 7, 1	    ; Start global interrupts
    BSF		0x1F, 7, 1	    ; Start SMT

    RETURN	
