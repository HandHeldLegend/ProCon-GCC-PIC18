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

; psect   barfunc,local,class=CODE,delta=2 ; PIC10/12/16
psect   barfunc,local,class=CODE,reloc=2 ; PIC18
; define external vars for linker
GLOBAL _gConBitCounter
GLOBAL _gConByteCount

GLOBAL _bytepush ; extern of bar function goes in the C source file
    
GLOBAL _gConOutByte ; Byte we can store value in that's outgoing
    
GLOBAL _gConOutIdx  ; Pointer for outgoing bytes

GLOBAL _gInStatus
    
GLOBAL _bytecleanup
    
_bytepush:
    
    BANKSEL(_gInStatus)
    ; Check if we have a command to push out
    BTFSS   BANKMASK(_gInStatus), 6, 1	; Byte 6 tells us if a command has been interpreted
    RETURN			; Return if there's no command to push out
    
    BCF	    TRISB, 2		    ; Turn the data pin around so we can drive
    
    ; Start writing a byte
    newByte:
	MOVFF   BANKMASK(_gConOutIdx), FSR0	; Get the address for our first byte, place into FSR0
	MOVFF   INDF0, BANKMASK(_gConOutByte)	; Set the outgoing byte with the first byte.
	CLRF	BANKMASK(_gConBitCounter), 1	; Set bit counter to 0 (clear)
	BSF	BANKMASK(_gConBitCounter), 3, 1	; Set bit counter to 8.
	
    ; Start writing our bit
    writeBit:
	BCF	PORTB, 2	; Set port to LOW
        BTFSS	BANKMASK(_gConOutByte), 7, 1  ; Check the leftmost bit, skip if one.
	GOTO	lowBitWrite	    ; Bit is 0, write a low bit out.
    
    ; Write a high bit
    highBitWrite:		;
        DCFSNZ	 BANKMASK(_gConBitCounter), 1, 1	; Decrement the BIT counter. Skip next when NOT 0 to go to next bit
	GOTO	endHighWrite
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
        BSF	PORTB, 2	; Set port to HIGH
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
	GOTO	getNextBit
	
    lowBitWrite:		
	DCFSNZ	 BANKMASK(_gConBitCounter), 1, 1	; Decrement the BIT counter. Skip next when NOT 0 to go to next bit
	GOTO	endLowWrite
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
	BSF	PORTB, 2	; Set port to HIGH
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
        RLCF	 BANKMASK(_gConOutByte), 1, 1	; Rotate the byte left so we can read the next bit
	GOTO	writeBit		; We still have bits left, so go back up to writeBit.
        
    getNextByte:
        INCF	BANKMASK(_gConOutIdx), 1, 1	; Increment our pointer value
        DECFSZ	BANKMASK(_gConByteCount), 1, 1	; Decrement our Byte counter, skip when no bytes left to write our stop bit.
        GOTO	newByte			
	
    stopBitWrite:
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
	BCF	PORTB, 2    ; Set port to LOW
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
        BSF	PORTB, 2    ; Set port to HIGH
    
	BANKSEL(SMT1CON1)
	BSF	SMT1CON1, 7, 1
	BANKSEL(_gInStatus)
	BSF	BANKMASK(_gInStatus), 5, 1    ; Set byte 5 of our status flag
	; Bit 5 says that we have sent out our response
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
	NOP
        BSF	PORTB, 2	; Set port to HIGH
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
	GOTO	getNextByte
    
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
	NOP
	BSF	PORTB, 2	; Set port to HIGH					
	GOTO	getNextByte
