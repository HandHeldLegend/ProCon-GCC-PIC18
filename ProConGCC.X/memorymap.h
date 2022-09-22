// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MEMORYMAP_H
#define	MEMORYMAP_H

#include <xc.h> // include processor files - each processor file is guarded.  

// Size of GC Data block items
#define SIZE_POLLPACKET     8U
#define SIZE_PROBEPACKET    3U
#define SIZE_ORIGPACKET     10U
#define SIZE_INPACKET       4U

// Start of GC Data block
#define MEM_STARTDATABLOCK  0x700
#define MEM_BANKSELDATA     MEM_STARTDATABLOCK >> 8

#define MEM_POLLPACKET      MEM_STARTDATABLOCK
#define MEM_PROBEPACKET     MEM_POLLPACKET      + SIZE_POLLPACKET
#define MEM_ORIGINPACKET    MEM_PROBEPACKET     + SIZE_PROBEPACKET
#define MEM_OUTPACKET       MEM_ORIGINPACKET    + SIZE_ORIGPACKET
#define MEM_OUTPACKETIDX    MEM_OUTPACKET       + 1
#define MEM_OUTBITCOUNT     MEM_OUTPACKETIDX    + 1
#define MEM_OUTBYTECOUNT    MEM_OUTBITCOUNT     + 1
#define MEM_INBITCOUNT      MEM_OUTBYTECOUNT    + 1
#define MEM_INPACKET        MEM_INBITCOUNT      + 1
#define MEM_INPACKETIDX     MEM_INPACKET        + SIZE_INPACKET
#define MEM_PULSEW          MEM_INPACKETIDX     + 1
#define MEM_PULSELOW        MEM_PULSEW          + 1
#define MEM_GSTATUS         MEM_PULSELOW        + 1
#define MEM_COMMANDBYTE     MEM_GSTATUS         + 1
#define MEM_ORIGMASK        MEM_COMMANDBYTE     + 1
#define MEM_POLLMASK        MEM_ORIGMASK        + 1

// End of GC Data block
#define MEM_ENDDATABLOCK    MEM_POLLMASK + 1

// Size of general items
#define SIZE_SETTINGS       50

#define MEM_STARTGENERALBLOCK   0x800

#define MEM_SETTINGS        MEM_STARTGENERALBLOCK
#define MEM_CALIBRATE_EN    MEM_SETTINGS        + SIZE_SETTINGS
#define MEM_XBOXMODE        MEM_CALIBRATE_EN    + 1
#define MEM_STICKIDX        MEM_XBOXMODE        + 1
#define MEM_STICKINIT       MEM_STICKIDX        + 1
#define MEM_STICKCLR        MEM_STICKINIT       + 1
#define MEM_ADC_READ        MEM_STICKCLR        + 1
#define MEM_TMPHIGH         MEM_ADC_READ        + 2
#define MEM_TMPLOW          MEM_TMPHIGH         + 2
#define MEM_TMPCENTER       MEM_TMPLOW          + 2
#define MEM_TMPHIGHM        MEM_TMPCENTER       + 2
#define MEM_TMPLOWM         MEM_TMPHIGHM        + 2
#define MEM_ADC_CONV        MEM_TMPLOWM         + 2

#define MEM_SB_TIME         MEM_ADC_CONV        + 2
#define MEM_SB_TIMEINT      MEM_SB_TIME         + 2
#define MEM_SB_TOFLOW       MEM_SB_TIMEINT      + 4
#define MEM_SB_TOFLIGHT     MEM_SB_TOFLOW       + 1
#define MEM_SB_XPM          MEM_SB_TOFLIGHT     + 4
#define MEM_SB_XNM          MEM_SB_XPM          + 1

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* MEMORYMAP_H */

