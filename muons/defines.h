#ifndef DEFINES_H
#define DEFINES_H

#include <sys/types.h>
#include <signal.h>
#include <sys/io.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <time.h>
#include <math.h>
#include <sys/timeb.h>
#include "globals_clT.h"
#include "common.h"

#define NPIN     4  // Number of pins in plate
#define NCLAST   4
#define NPLAT    2
#define NCHAN    2
#define NDET     NPLAT*NCHAN

#define  EventsPerFile  100
#define  TimePerHistogramm 1000  // in sec

void nrerror( char error_text[] );

extern int COINS[NCLAST];
extern int Thresh[NCLAST*NDET];
extern int HV[NCLAST*NDET];
extern int LED[NCLAST*NDET];
extern char DEFECT[NCLAST][NPLAT][NCHAN];
extern unsigned long nnn;
extern unsigned long EventsPerEndMeasure;

extern FILE *ferr, *ftm, *ffsum;

extern word h,m,s,mls,mks,dns;
extern byte NCLASTREADY;
extern byte ClasterReady[NCLAST];
extern word h0,m0,s0,mls0;
extern long tclast0,tclast1,tcomp0,tcomp1;

extern long dat1,dat2;
extern int mass[2000];
extern FILE *fmass;

extern char filename[NCLAST][120],txtname[NCLAST][120],histname[NCLAST][120];
extern char filetime[120];
extern char summname[120];

extern char host[];
extern int SIZE;

#endif // DEFINES_H
