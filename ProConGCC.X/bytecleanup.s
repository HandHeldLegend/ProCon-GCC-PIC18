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
    
    ; Check if we are out of sync and need cleanup
    BTFSC   _gInStatus, 0, 0	; Byte 0 is desync. Skip if in sync
    GOTO    MAINCLEANUP		; Perform a cleanup
    
    ; Check that we finished a packet and need cleanup
    BTFSS   _gInStatus, 5, 0	; Byte 5 response sent flag. Skip if we sent one
    RETURN			; Return if we have nothing to clean up yet.
    
    MAINCLEANUP:
    
	; We gotta reset some variables for our next command read
	MOVLW	_gInPacket	    ; Reset the byte packet index pointer
	MOVFF	WREG, _gInPacketIdx

	BCF	_gInStatus, 7, 0    ; Clear the stop bit written flag
	BCF	_gInStatus, 6, 0    ; Clear the command interpreted flag
	BCF	_gInStatus, 5, 0    ; response sent flag
	BCF	_gInStatus, 0, 0    ; Clear any desync flag

	CLRF	_gInBitCounter, 0   ; Clear the bit counter
	BSF	_gInBitCounter, 3, 0; Set the bit counter to 8 (bit 3)

	CLRF	_gConOutByte, 0	    ; Clear the outgoing byte container
	CLRF	_gConOutIdx, 0	    ; Clear the outgoing byte index pointer
	CLRF	_gConByteCount, 0   ; Clear the outgoing byte counter
	CLRF	_gInCommandByte, 0  ; Clear the incoming command bit
	CLRF	_gInPulseWidth, 0   ; Clear the pulse width counter
	
	BSF	_gInStatus, 4, 0    ; Set the flag that we did cleanup
	
	
	BSF	_gInStatus, 3, 0    ; Enable stick scan

	BSF	TRISB, 2	; Clear the data pin from us driving so we can receive
	
	MOVLB	0x3F
	BSF	0xD2, 7, 1	    ; Start global interrupts
	BSF	0x1F, 7, 1	    ; Start SMT
	
	RETURN
