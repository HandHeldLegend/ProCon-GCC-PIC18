#include <xc.inc>

; When assembly code is placed in a psect, it can be manipulated as a
; whole by the linker and placed in memory.  
;
; In this example, barfunc is the program section (psect) name, 'local' means
; that the section will not be combined with other sections even if they have
; the same name.  class=CODE means the barfunc must go in the CODE container.
; PIC18's should have a delta (addressible unit size) of 1 (default) since they
; are byte addressible.  PIC10/12/16's have a delta of 2 since they are word
; addressible.  PIC18's should have a reloc (alignment) flag of 2 for any
; psect which contains executable code.  PIC10/12/16's can use the default
; reloc value of 1.  Use one of the psects below for the device you use:

psect   barfunc,local,class=CODE,reloc=2 

global _gPollPacket
global _gOutBytesLeft
global _gOutByte
global _gOutBitCounter
global _gOutBytesLeft
    
global _sendpoll ; extern of bar function goes in the C source file
_sendpoll:
    
    BANKSEL(_gPollPacket)
    MOVLW   BANKMASK(_gPollPacket)
    MOVWF   FSR0, 0
    MOVLW   0x8
    MOVWF   BANKMASK(_gOutBytesLeft), 1
    
    ; Disable pin interrupts
	BANKSEL(INTCON0)
	BCF	INTCON0, 7, 1
	NOP
	BANKSEL(_gOutByte)
	
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	BCF	TRISB, 2	; Turn the data pin around so we can drive
	
	; Start writing a byte
	newByte:
	    MOVFF   INDF0, _gOutByte	    ; Set the outgoing byte with the first byte.
	    CLRF    BANKMASK(_gOutBitCounter), 1	    ; Set bit counter to 0 (clear)
	    BSF	    BANKMASK(_gOutBitCounter), 3, 1   ; Set bit counter to 8.

	; Start writing our bit
	writeBit:
	    BCF	    PORTB, 2		; Set port to LOW
	    BTFSS   BANKMASK(_gOutByte), 7, 1	; Check the leftmost bit, skip if one.
	    GOTO    lowBitWrite		; Bit is 0, write a low bit out.

	; Write a high bit
	highBitWrite:		;
	    DCFSNZ  BANKMASK(_gOutBitCounter), 1, 1   ; Decrement the BIT counter. Skip next when NOT 0 to go to next bit
	    GOTO    endHighWrite
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    BSF	    PORTB, 2	; Set port to HIGH
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP			
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP			
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP			
	    NOP			
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP			
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    GOTO    getNextBit

	lowBitWrite:		
	    DCFSNZ  BANKMASK(_gOutBitCounter), 1, 1   ; Decrement the BIT counter. Skip next when NOT 0 to go to next bit
	    GOTO    endLowWrite
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP			
	    NOP			
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP			
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP			
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    BSF	    PORTB, 2	; Set port to HIGH
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP			
	    NOP
	    NOP
	    NOP			
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP

	; Load up our next bit
	getNextBit:			
	    RLCF    BANKMASK(_gOutByte), 1, 1	; Rotate the byte left so we can read the next bit
	    GOTO    writeBit		; We still have bits left, so go back up to writeBit.

	getNextByte:
	    INCF    FSR0, 1, 0		; Increment our pointer value
	    DECFSZ  BANKMASK(_gOutBytesLeft), 1, 1; Decrement our Byte counter, skip when no bytes left to write our stop bit.
	    GOTO    newByte			

	stopBitWrite:
	    NOP
	    NOP
	    NOP
	    NOP
	    BCF	    PORTB, 2	; Set port to LOW
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP	
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP	
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP	
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP	
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    BSF	    PORTB, 2	; Set port to HIGH

	    BANKSEL(INTCON0)
	    ; Reenable interrupts
	    BSF	    INTCON0, 7, 1
	    BSF	TRISB, 2	; Turn the data pin around so we can listen
	    RETURN

	endHighWrite:	
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    BSF	    PORTB, 2	; Set port to HIGH
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP			
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP			
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP			
	    NOP			
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP			
	    GOTO    getNextByte

	endLowWrite:
	    NOP			
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP			
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP			
	    NOP			
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP			
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP			
	    NOP
	    NOP
	    NOP
	    NOP
	    BSF	    PORTB, 2	; Set port to HIGH
	    NOP
	    NOP
	    NOP
	    NOP
	    GOTO    getNextByte

RETURN