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

#define NPIN     4  // Number of pins in plate
#define NCLAST   4
#define NPLAT    2
#define NCHAN    2
#define NDET     NPLAT*NCHAN

int COINS[NCLAST];
int Thresh[NCLAST*NDET];
int HV[NCLAST*NDET];
int LED[NCLAST*NDET];
char DEFECT[NCLAST][NPLAT][NCHAN];
unsigned long nnn=0L;
unsigned long EventsPerEndMeasure=100;


#define  EventsPerFile  100
#define  TimePerHistogramm 1000  // in sec


#include "bitslib.h"

void nrerror( char error_text[] ) {

	fprintf( stderr, "%s\n",error_text );
	fprintf( stderr, "... now exiting to system ...\n" );
	exit( 1 );
}


FILE *ferr, *ftm, *ffsum;
#include "create_dir.c"
#include "common.h"
#include "globals_clT.h"
#include "vme.h"
#include "vme.cpp"
#include "vme-lib.c"
#include "flash_adc.c"

word h,m,s,mls,mks,dns;
byte NCLASTREADY=0;
byte ClasterReady[NCLAST];
word h0,m0,s0,mls0;
long tclast0,tclast1,tcomp0,tcomp1;


#include "master.c"

long dat1,dat2;
int mass[2000];
FILE *fmass;
char filename[NCLAST][120],txtname[NCLAST][120],histname[NCLAST][120];
char filetime[120];
char summname[120];

#include "stoprun.c"
#include "thresholds.c"

#include "savedata.c"
#include "mainblck.c"
#include "readconf.c"

char host[]="192.168.1.89";
int SIZE=0;
