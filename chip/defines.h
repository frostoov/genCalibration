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
#include <vector>
#include "common.h"
#include "globals_clT.h"
#include "vme.h"

#define NPIN     4  // Number of pins in plate
#define NCLAST   4
#define NPLAT    2
#define NCHAN    2
#define NDET     NPLAT*NCHAN
#define  TimePerHistogramm 1000  // in sec
#define  EventsPerFile  100

typedef unsigned long ULONG;
typedef unsigned int UWORD;

typedef struct
{
	int TRIGGER;
	int ii;
	byte Claster;
	byte PlataNum;
	int flag_STOP;
	int coin;
	int flag;
	int nnn;
	time_t this_moment;
	time_t last_moment;
	unsigned long Nev[NCLAST];
} exchangeBlock;

extern int COINS[NCLAST];
extern int Thresh[NCLAST*NDET];
extern int HV[NCLAST*NDET];
extern int LED[NCLAST*NDET];
extern char DEFECT[NCLAST][NPLAT][NCHAN];
extern unsigned long nnn;
extern unsigned long EventsPerEndMeasure;
extern FILE *ferr, *ftm, *ffsum;

extern struct timeb Now,Stop;
extern struct tm t;
extern FILE *ffstat, *fftime, *fferr, *ffdat;
extern char pathfile[120];
extern char statfile[120];
extern char timefile[120];
extern char summfile[120];
extern char errfile[120];
extern char datafile[120];
extern VME cl;
extern VME* clA;

extern word h,m,s,mls,mks,dns;
extern byte NCLASTREADY;
extern byte ClasterReady[NCLAST];
extern word h0,m0,s0,mls0;
extern long tclast0,tclast1,tcomp0,tcomp1;

extern char filename[NCLAST][120],txtname[NCLAST][120],histname[NCLAST][120];
extern char filetime[120];
extern char summname[120];

unsigned char get_single_bit (unsigned char sym, int bit);

void Two_char (	unsigned int twobit,
		unsigned char *bitHigh, unsigned char *bitLow );

void Four_char (unsigned long fourbit,
		unsigned char *bitHH, unsigned char *bitHL,
		unsigned char *bitLH, unsigned char *bitLL );

void print_in_bits (unsigned long sym);

unsigned long Create_Dir_Files(void);

longw GetBaseAddrFADC(byte CreiteNum, byte PlataNum);
void ResetHIST (byte CreiteNum, byte PlataNum, byte ChannelNum);
void SetProgramTrigger (byte CreiteNum, byte PlataNum );
void WriteStatusRegister ( byte CreiteNum, byte PlataNum,
						   byte BlockRequest0,
						   byte TriggerRoutine0,
						   byte RequestAsTrigger0,
						   byte BlockRequest1,
						   byte TriggerRoutine1,
			   byte RequestAsTrigger1 );
void SetTriggerExternal ( byte CreiteNum, byte PlanaNum );
void SetTriggerProgramm ( byte CreiteNum, byte PlataNum );
void SetTriggerSelf ( byte CreiteNum, byte PlataNum );
void BlockedChannel ( byte CreiteNum, byte PlataNum, byte ChannelNum );
void SetThreshold ( byte CreiteNum, byte PlataNum, byte ChannelNum,
					word Threshold );
void SetDelayRequest ( byte CreiteNum, byte PlataNum, byte ChannelNum,
					   word DelayRequest );
word GetStatusRegister ( byte CreiteNum, byte PlataNum,
						 byte ChannelNum, byte Bank, byte Flag);
word Read_ADC_Word ( byte CreiteNum, byte PlataNum, longw AddrData );
void BlockRead_ADC(byte CreiteNum, byte PlataNum, byte ChannelNum,
				   longw StartAddr, word WordsForRead,	int *DataMas);
void GetOneChannelData ( byte CreiteNum, byte PlataNum, byte ChannelNum,
						 int *DataMas );
void GetPulseForm ( byte CreiteNum, byte PlataNum, byte Bank,
					int *DataMas );
void GetPulseFormDim ( byte CreiteNum, byte PlataNum, byte Bank,
					   short int *DataMas );
void GetHistogram ( byte CreiteNum, byte PlataNum, int *DataMas );
void GetHistogramDim ( byte CreiteNum, byte PlataNum, int *DataMas );

int GetSummaPulse(int clast, short int (*DATA)[1024*4], float *PLS );

int MainBlock (unsigned long nnn, int *Threshold, int flag );

longw GetBaseAddrMaster(byte CreiteNum);
void ResetEvent (byte CreiteNum, byte Bank);
int GetStatusEvent (byte CreiteNum);
void SetCoinsidence (byte CreiteNum, int Number);
int GetCoinsidence (byte CreiteNum);
void SetInitTimer (byte CreiteNum);
void SetBeginTime (byte CreiteNum, word hour, word min, word sec,
								   word mlsec, word mksec, word dnsec);
void GetTimer (byte CreiteNum);
void GetTimeEvent (byte CreiteNum, byte Bank);
//===========================================================================
//===========================================================================
//===========================================================================
//===========================================================================
longw GetBaseAddrCenter(byte CreiteNum);
void StartTimer (byte CreiteNum);
void StartApparatSynchro (byte CreiteNum);
void ProhibiteApparatSynchro (byte CreiteNum);
void ResolveApparatSynchro (byte CreiteNum);
word GetStatusVME (byte CreiteNum);
word GetTimeDelay (byte CreiteNum);
void Timer_Init ( byte Nclast, byte *ClastReady );
int  ReadConfiguration(byte Claster, int *TH, int *HV, int flag);
void ReadConfigFile(int *Thres, int *HV);
void SaveConfigFile(int *Thres, int *HV);
int OpenNewFile(unsigned long nnn, int por, int flag );
void SaveData ( byte Claster, byte Bank, int event, int por );
void SaveData (unsigned char Claster, unsigned char Bank, int event, int por);
int ReadAmplAnalizator ( byte Claster );
void SaveTime ( byte Claster, int event );
class sock;
void ReadStopTime (void);
int GetStopRun  ( void );
void GetBaseLine (byte Claster, byte PlataNum, int *BaseLine, int flag );
void SetThresholdWithBaseLine (byte Claster, int *Threshold, int flag );
void UnConnect( void);
void Connect(char* host,int size);
int ReadWordExt12(ULONG adr,UWORD AModifier);     /* get word from arbitrary address */
int ReadWordExt(ULONG adr,UWORD AModifier);     /* get word from arbitrary address */
int ReadDimExt12(ULONG adr, word nwords, short int *Data);     /* get word from arbitrary address */
int ReadDimExt(ULONG adr, word nwords, int *Data);     /* get word from arbitrary address */
void WriteWordExt(ULONG adr, int Data,UWORD AModifier);


#endif // DEFINES_H
