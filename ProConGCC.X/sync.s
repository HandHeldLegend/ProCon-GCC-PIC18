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

global _gSyncCycles
global _gLowThreshold
global _gLargestPulse
global _gInStatus
 
global _sync ; extern of bar function goes in the C source file
_sync:
    BANKSEL(gSyncCycles)
    
    
    
    BANKSEL(PIE1)
    BTFSS   PIE1, 7, 1	 ; Check if interrupt flag on. Skip if it is
    RETURN
    
    BTFSS   PIR1, 7, 1	 ; Check if we got a pulse width acquisition. Skip if yes
    RETURN
    
    
