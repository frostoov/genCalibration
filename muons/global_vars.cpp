#include "defines.h"

int COINS[NCLAST];
int Thresh[NCLAST*NDET];
int HV[NCLAST*NDET];
int LED[NCLAST*NDET];
char DEFECT[NCLAST][NPLAT][NCHAN];
unsigned long nnn=0L;
unsigned long EventsPerEndMeasure=100;

FILE *ferr, *ftm, *ffsum;

word h,m,s,mls,mks,dns;
byte NCLASTREADY=0;
byte ClasterReady[NCLAST];
word h0,m0,s0,mls0;
long tclast0,tclast1,tcomp0,tcomp1;

long dat1,dat2;
int mass[2000];
FILE *fmass;

char filename[NCLAST][120],txtname[NCLAST][120],histname[NCLAST][120];
char filetime[120];
char summname[120];

char host[]="192.168.1.89";
int SIZE=0;

void nrerror( char error_text[] ) {

	fprintf( stderr, "%s\n",error_text );
	fprintf( stderr, "... now exiting to system ...\n" );
	exit( 1 );
}

