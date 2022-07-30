#include <xc.inc>

; Variables relating to bitgrabber incoming data
GLOBAL _gInStatus
GLOBAL _gInPacket
GLOBAL _gInPacketIdx

; Variables relating to checking commands
GLOBAL _gCommandPollMask
GLOBAL _gCommandOriginMask

; Variables used to set up our bytepusher
GLOBAL _gConOutIdx
GLOBAL _gConByteCount
    
; Variables for outgoing responses, canned and assembled
GLOBAL _gConPollPacket	    ; The packet we use to store our button and stick data
GLOBAL _gConOriginResponse  ; Used to respond to origin command
GLOBAL _gConProbeResponse   ; Used to respond to probe command
    
GLOBAL	_gSamplesLeft  ; How many samples have been taken so far 
    
; External function
GLOBAL _bytepush
GLOBAL _bytecleanup
GLOBAL _main

psect   barfunc,local,class=CODE,reloc=2 ; PIC18

global _commandreader ; extern of bar function goes in the C source file
_commandreader:
    
    ; This function should run in the main loop.
    ; When it detects that a stop bit has been written,
    ; we can take action to quickly interpret the message
    ; and send out the corresponding packet.
    
    BANKSEL(_gInStatus)
    ; First let's check if the stop bit write flag is set
    BTFSS   _gInStatus, 7, 1	; Byte 7 in the status is the stop bit flag
    RETURN			; Return if there's no command to read

    ; WE WILL CLEAN UP LATER, JUST GET ON TO SENDING OUT THE RIGHT PACKS ASAP
    ; Since the most high priority (performance-wise) command will be polling
    ; which is where our button inputs are sent back, we should 100% check for this
    ; first and foremost. We want as little delay as possible in polling as we have about
    ; 600 MICROSECONDS to grab our button and stick updates before the next poll.
    ; Less delay between poll command and our response means... more time for button updates =)

    MOVLB   0x3F	    ; Switch to bank 0x3F
    BCF	    0xD2, 7, 1	    ; Stop global interrupts
    BCF	    0x1F, 7, 1	    ; Stop SMT
    
    BANKSEL(_gInStatus)
    ; Check to see if we have a poll command coming in
    MOVFF   BANKMASK(_gInPacket), WREG	    ; Get the first byte of the IN packet
    XORWF   BANKMASK(_gCommandPollMask), 0, 1 ; Perform exclusive OR on the command byte against our mask

    ; Anything that ISN'T a match will fail the check zero/skip command.
    TSTFSZ  WREG, 0		    ; skip if it's zero meaning a match.
    GOTO    ORIGINPARSE		    ; We did not match, so now we check if it's an origin command.

	; If we made it to here, we are working with a poll command.
	; Lets set up the outgoing byte as a pointer for the bytepush command.
	MOVLW   BANKMASK(_gConPollPacket)		; Get the pointer for the poll packet
	MOVWF   BANKMASK(_gConOutIdx), 1		; Move the pointer into the outgoing index.
	
	BSF	BANKMASK(_gConByteCount), 3, 1	; Set our outgoing Byte count to 8 (bit set 3) 0000 1000

	BSF	BANKMASK(_gInStatus), 6, 1	; Mark that we have a valid command interpreted
	RETURN


; Here we check if we got a origin command to send over our calibration info.
ORIGINPARSE:

    MOVFF   BANKMASK(_gInPacket), WREG		; Get the first byte of the IN packet
    XORWF   BANKMASK(_gCommandOriginMask), 0, 1   ; Perform exclusive OR on the command byte against our mask

    ; Anything that ISN'T a match will fail the check zero/skip command.
    TSTFSZ  WREG, 0			; skip if it's zero meaning a match.
    GOTO    PROBEPARSE			; We did not match, so now we check if it's a probe command.

	; If we made it to here, we are working with a origin command.
	; Lets set up the outgoing byte as a pointer for the bytepush command.
	MOVLW   BANKMASK(_gConOriginResponse)	; Get the pointer for the poll packet
	MOVWF   BANKMASK(_gConOutIdx), 1		; Move the pointer into the outgoing index.

	BSF	BANKMASK(_gConByteCount), 3, 1    ; Set our outgoing Byte count to 8 (bit set 3) 0000 1000
	BSF	BANKMASK(_gConByteCount), 1, 1    ; Set the bit 1 so we have 10 (8+2) 0000 1010

	BSF	BANKMASK(_gInStatus), 6, 1	; Mark that we have a valid command interpreted
	RETURN


; Here we check if we got a probe command for first contact.
PROBEPARSE:

    ; First we can check if the command is 0x0 (0000 0000) meaning it's being probed.
    MOVFF   BANKMASK(_gInPacket), WREG	; Get the first byte of the IN packet
    TSTFSZ  WREG, 0		; If it's zero, skip checking any further
    GOTO    NOCOMMAND		; No command match

    ; We need to make sure our byte counter for outgoing is set

    BSF	    BANKMASK(_gConByteCount), 0, 1	; Set our outgoing Byte count to 1 (bit set 0) 0000 0001
    BSF	    BANKMASK(_gConByteCount), 1, 1	; Set the bit 1 so we have 3 (1+2)  0000 0011

    MOVLW   BANKMASK(_gConProbeResponse)	; Place the address for our first byte into WREG
    MOVWF   BANKMASK(_gConOutIdx), 1	;
    
    ; Mark our status bit that we are synced
    BSF	    BANKMASK(_gInStatus), 1, 1	; Bit 1 is sync bit
    BSF	    BANKMASK(_gInStatus), 6, 1	; Mark that we have a valid command interpreted 
    RETURN
    
NOCOMMAND:
    
    BSF	    BANKMASK(_gSamplesLeft), 7, 1   ; reset our sample counter
    CLRF    BANKMASK(_gInStatus), 1	    ; Clear the communication status byte
    BSF	    BANKMASK(_gInStatus), 0, 1    ; Set bit 0 of our status flag meaning we need to resync
    RETURN
