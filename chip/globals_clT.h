#ifndef _globals_clT_
#define _globals_clT_

//#define Version          	"0.0.1 - August 2007"

#include <sys/types.h>

//#define VME_DEBUG



typedef unsigned char    	byte;
//typedef u_int16_t        	word;
typedef short unsigned int		word; 
//typedef int        	word;
typedef unsigned int     	longint;

#define Std_NoPriv_Data		2
#define Std_Priv_Data		1024
#define HD				0xFFFF//header
#define CMD_RD			0x80
#define CMD_WR			0x00
#define STAT_OK                 0x80

//word h,m,s,mls,mks,dns;
//unsigned int  h,m,s,mls,mks,dns;

//#define NPLAT	4

#endif
