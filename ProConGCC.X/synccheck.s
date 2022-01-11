#include <xc.inc>

; We should perform checks to ensure we are in sync.
; If we read a '1' bit when we are not at an in bit count of 8 (beginning of a new packet)
; then we are out of sync. We should perform a cleanup so we can re-sync.

psect   barfunc,local,class=CODE,reloc=2 ; PIC18

GLOBAL _gInBitCounter	; Incoming bit counter.
GLOBAL _gInStatus	; Device communication status
    
GLOBAL _gInCommandByte	; Incoming command byte array (points to first value)
    
GLOBAL _bytecleanup	; External function for cleanup
    
global _synccheck ; extern of bar function goes in the C source file
_synccheck:
    
    ; Check that we are NOT on bit 8.
    BTFSC   _gInBitCounter, 3, 0    ; Bit 3 will be 1 if we are at 8.
    RETURN			    ; Return if bit IS 8.
    
    BTFSC   _gInStatus, 1, 0	    ; Bit 1, skip if we are not marked as synced yet
    RETURN
    
    ; If we got here, we are not yet in sync and
    ; we should verify that we are not getting any '1' bits.
    BTFSS   _gInCommandByte, 0, 0   ; Check the rightmost bit for the most recent bit
    RETURN			    ; If we see a 1 or HIGH bit, skip this return
    
    ; If we got here, we should reset the device synced flag
    BCF	    _gInStatus, 0, 0	    ; bit 0 for the desync
    
    RETURN
