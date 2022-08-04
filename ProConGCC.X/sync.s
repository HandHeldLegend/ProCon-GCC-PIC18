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

global _gInStatus
global _gInBytes
global _gFSR0ptr
 
global _sync ; extern of bar function goes in the C source file
_sync:
    
    ; When this interrupt triggers, it indicates that
    ; no activity has occurred on the joybus line in
    ; a period of time that would indicate we are in
    ; between packets. This is when we should start
    ; the communication process. 
    
    BANKSEL(PIR1)
    ; Check if overflow interrupt is enabled.
    ; for SMT
    BTFSS   PIE1, 5, 1
    ; Skip if not enabled.
    RETURN
    
    BANKSEL(PIR1)
    ; Check if overflow flag was set
    BTFSS   PIR1, 5, 1
    ; Skip if interrupt flag not set.
    RETURN
    
    ; Clear the interrupt flag for width acquire
    BCF	    PIR1, 7, 1
    
    BANKSEL(PIE1)
    ; Clear the interrupt enable
    BCF	    PIE1, 5, 1
    BANKSEL(PIR1)
    ; Clear the interrupt flag
    
    BCF	    PIR1, 5, 1
    
    BANKSEL(_gInStatus)
    ; Set the status flag indicating
    ; that we have synced up. We can 
    ; safely assume the next pulse will
    ; be the beginning of a new packet.
    BSF	    BANKMASK(_gInStatus), 0, 1
    MOVLW   BANKMASK(_gInBytes)
    MOVWF   BANKMASK(_gFSR0ptr), 1
    
    BANKSEL(PIE1)
    
    ; Enable pulse width interrupt
    BSF	    PIE1, 7, 1
    BCF	    PIR1, 7, 1
    
    
    
    RETURN

    
