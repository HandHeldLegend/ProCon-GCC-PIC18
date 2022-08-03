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

psect   barfunc,local,class=CODE,reloc=2 ; PIC10/12/16
; psect   barfunc,local,class=CODE,reloc=2 ; PIC18

global _gInStatus
global _gInByte
global _gPulseWidth
global _gOutBytesLeft
global _gLowThreshold
global _gInBitCounter
global _gProbeResponse
global _gOriginResponse
global _gInBytesLeft
global _gPollPacket
global _gInRumble
global _gOutByte
global _gOutBitCounter
global _gFSR0ptr
    
global _joybus ; extern of joybus function goes in the C source file
_joybus:
    
    BANKSEL(PIE1)
    BTFSS   PIE1, 7, 1	 ; Check if interrupt flag on. Skip if it is
    RETURN
    
    BTFSS   PIR1, 7, 1	 ; Check if we got a pulse width acquisition. Skip if yes
    RETURN
    
    BANKSEL(_gInStatus)
    
    ; First we want to check if we recognize a command
    ; This will tell us that this pulse is a 'stop' or 'send data' pulse
    ; If the bit is clear, we have not recognized a command and we should
    ; Skip sending a message
    BTFSC   BANKMASK(_gInStatus), 7, 1
    GOTO    SENDRESPONSE
    
    ; Clear interrupt flag for pulse interrupt
    BCF	    PIR1, 7, 1
    
    ; Check if a sync cycle is needed. Return if it's needed.
    BTFSC   BANKMASK(_gInStatus), 0, 1
    RETURN
    
    ; Copy pulse width to our wreg
    MOVFF   SMT1CPWL, WREG
    
    ; Check if we're getting a poll 2nd byte.
    ; Skip if we aren't.
    BTFSC   BANKMASK(_gInStatus), 6, 1
    GOTO    POLLSECOND
    
    ; Rotate the current byte to the left by 1.
    ; Clear the rightmost bit to zero
    RLNCF   BANKMASK(_gInByte), 1, 1	
    BCF	    BANKMASK(_gInByte), 0, 1	
    
    ; Check if we are on our first bit
    ; skip if we ain't and we can keep measuring
    ; per normal.
    BTFSC   BANKMASK(_gInStatus), 1, 1
    GOTO    FIRSTBIT
    
    ; This compares gLowThreshold to the captured pulse width.
    ; If the low threshold is more than the pulse width, the pulse was short
    ; meaning we have a HIGH bit.
    CPFSLT  BANKMASK(_gLowThreshold), 1	; if the pulse is more than the low threshold, Skip, keeping the bit LOW.
    BSF	    BANKMASK(_gInByte), 0, 1	; Set the rightmost bit HIGH
    
    ; Decrement the bit counter. 8 bits total. Skip if 0
    DECFSZ  BANKMASK(_gInBitCounter), 1, 1	
    ; Return and we can just wait for the next bit :)
    RETURN
    
	; If we got here, check the command and go to the next byte if we need to
	; Test if byte is 0x00. Skip if yes
	TSTFSZ	BANKMASK(_gInByte), 1
	; We need to do additional processing if it's not 0x00
	GOTO	PROCESSCMD		
	    
	    ; If we got here, we have a probe command. Toggle bit and set outgoing data.
	    BSF	    BANKMASK(_gInStatus), 7, 1	
	    
	    ; Get address to probe response first byte.
	    MOVLW   _gProbeResponse		
	    ; Move address to FSR0 so we can access with INDF0.
	    MOVWF   BANKMASK(_gFSR0ptr), 1
	    ; Set byte length for outgoing command to 3.
	    MOVLW   0x3
	    MOVWF   BANKMASK(_gOutBytesLeft), 1
	    
	    ; Return
	    RETURN				

    ; This is for our first bit and we can keep
    ; track of the pulse width nonsense
    FIRSTBIT:
	; At this point, WREG is
	; still loaded with the actual
	; pulse width. Subtract 8 from WREG.
	
	; First copy the pulse width to gLowThreshold
	MOVWF	BANKMASK(_gLowThreshold), 1
	; Move literal 8 to WREG
	MOVLW	0x8
	; Subtract 8 from the pulse and store it
	; as the new 'low' threshold.
	SUBWF	BANKMASK(_gLowThreshold), 1, 1
	
	; Clear the status flag indicating
	; that we have our first sync bit.
	BCF	BANKMASK(_gInStatus), 1, 1
	
	RETURN
	    
    POLLSECOND:
	; Check if we're getting a poll 3rd (final) byte.
	; Skip if we aren't.
	BTFSS   BANKMASK(_gInStatus), 5, 1
	GOTO	POLLTHIRD
	
	; Subtract incoming bits. Skip if zero.
	DECFSZ	BANKMASK(_gInBitCounter), 1
	RETURN
	
	    ; If we're here, we just got the second byte of poll
	    ; and we need to set up for the next byte
	    ; Set the status bit
	    ; indicating we are now polling our third poll byte.
	    BSF	    BANKMASK(_gInStatus), 5, 1
	    ; Reset bit counter to 8
	    BSF	    BANKMASK(_gInBitCounter), 3, 1
	    RETURN
	
    POLLTHIRD:
	; Set the incoming rumble data
	RLNCF   BANKMASK(_gInRumble), 1, 1	; Rotate the current byte to the left by 1.
	BCF	BANKMASK(_gInRumble), 0, 1	; Clear the rightmost bit to zero
	
	; This compares gLowThreshold to the captured pulse width.
	; If the low threshold is more than the pulse width, the pulse was short
	; meaning we have a HIGH bit.
	CPFSLT  BANKMASK(_gLowThreshold), 1	; if the pulse is more than the low threshold, Skip, keeping the bit LOW.
	BSF	BANKMASK(_gInRumble), 0, 1	; Set the rightmost bit HIGH
	
	; Subtract incoming bits. Skip if not zero.
	DCFSNZ	BANKMASK(_gInBitCounter), 1
	; When we're out of bits. Set the status bit
	; indicating we have completed an incoming command.
	BSF	BANKMASK(_gInStatus), 7, 1
	RETURN
	
    PROCESSCMD:
	
	; Test if 0x40 poll request (6th bit is set, skip).
	BTFSS	BANKMASK(_gInByte), 6, 1;
	; If we're on the first byte and we don't even have
	; 0x40, there's an erroneous command!
	GOTO	ERRCOMMAND
	
	; If we got here, we're at least 0x40.
	; Are we also 0x41? Let's check (bit 0). Skip if set.
	BTFSS	BANKMASK(_gInByte), 0, 1;
	; This means we're 0x40 only, go to poll setup.
	GOTO	POLLSETUP
	
	    ; This is com 0x41. Set command as done.
	    BSF	    BANKMASK(_gInStatus), 7, 1

	    ; Get address to origin response first byte.
	    MOVLW   _gOriginResponse		
	    ; Move address to FSR0 so we can access with INDF0.
	    MOVWF   BANKMASK(_gFSR0ptr), 1
	    ; Set byte length for outgoing command to 10.
	    MOVLW   0xA
	    MOVWF   BANKMASK(_gOutBytesLeft), 1
	    RETURN
	
    ERRCOMMAND:
    
	RETURN
    
    POLLSETUP:	
    
	; Set up that we want to read the second
	; byte of the poll packet
	BSF	BANKMASK(_gInStatus), 6, 1
    
	; Get address to poll response first byte.
	MOVLW   _gPollPacket		
	; Move address to FSR0 so we can access with INDF0.
	MOVWF   BANKMASK(_gFSR0ptr), 1
	; Set byte length for outgoing command to 8.
	MOVLW   0x8
	MOVWF   BANKMASK(_gOutBytesLeft), 1
	
	; Set in bytes left to 2.
	MOVLW	0x2
	MOVWF	BANKMASK(_gInBytesLeft), 1
	RETURN
	

    SENDRESPONSE:
	; Disable pin interrupts
	BANKSEL(INTCON0)
	BCF	INTCON0, 7, 1
	NOP
	BANKSEL(_gOutByte)
	MOVFF	BANKMASK(_gFSR0ptr), FSR0
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
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
	    NOP
	    NOP
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

	    ; Perform some cleanup
	    ; Reset gInStatus
	    CLRF    BANKMASK(_gInStatus), 1
	    MOVLW   0x8
	    MOVWF   BANKMASK(_gInBitCounter), 1
	    MOVWF   BANKMASK(_gOutBitCounter), 1
	    ; Clear interrupt flag for pulse interrupt
	    BCF	    PIR1, 7, 1
	    BANKSEL(INTCON0)
	    ; Reenable interrupts
	    BSF	    INTCON0, 7, 1
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
	    NOP
	    NOP
	    NOP
	    NOP
	    BSF	    PORTB, 2	; Set port to HIGH					
	    GOTO    getNextByte

	