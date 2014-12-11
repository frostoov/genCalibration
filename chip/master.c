//===========================================================================
#include "master.h"

#define BaseAddrMaster 0x200000
#define BaseAddrCenter 0x400000

extern word h,m,s,mls,mks,dns;
//===========================================================================
longw GetBaseAddrMaster(byte CreiteNum)
{
	return ( (longw)CreiteNum<<23 | (longw)BaseAddrMaster );
}
//===========================================================================
void ResetEvent (byte CreiteNum, byte Bank)
{
#ifdef MASTER_DEBUG
	printf("ResetEvent\n");
#endif
	if ( Bank==0 )
		WriteWordExt ( GetBaseAddrMaster(CreiteNum), 0x1 );

	if ( Bank==1 )
		WriteWordExt ( GetBaseAddrMaster(CreiteNum), 0x2 );
}
//===========================================================================
int GetStatusEvent (byte CreiteNum)
{
#ifdef MASTER_DEBUG
	printf("GetStatusEvent\n");
#endif
	return ReadWordExt( GetBaseAddrMaster(CreiteNum) );
}
//===========================================================================
void SetCoinsidence (byte CreiteNum, int Number)
{
#ifdef MASTER_DEBUG
	printf("SetCoinsidence\n");
#endif
	WriteWordExt (GetBaseAddrMaster(CreiteNum)+6, Number );
}
//===========================================================================
int GetCoinsidence (byte CreiteNum)
{
#ifdef MASTER_DEBUG
	printf("GetCoinsidence\n");
#endif
	return ReadWordExt (GetBaseAddrMaster(CreiteNum)+6 );
}
//===========================================================================
void SetInitTimer (byte CreiteNum)
{
#ifdef MASTER_DEBUG
	printf("SetInitTimer\n");
#endif
	WriteWordExt (GetBaseAddrMaster(CreiteNum)+2, 0x1 );
}
//===========================================================================
void SetBeginTime (byte CreiteNum, word hour, word min, word sec,
				   word mlsec, word mksec, word dnsec)
{
	word data=0;

#ifdef MASTER_DEBUG
	printf("SetBeginTime\n");
#endif
	data = mksec << 7 | dnsec;
	WriteWordExt (GetBaseAddrMaster(CreiteNum) + 16, data );
	data = sec << 10 | mlsec;
	WriteWordExt (GetBaseAddrMaster(CreiteNum) + 18, data );
	data = hour << 6 | min;
	WriteWordExt (GetBaseAddrMaster(CreiteNum) + 20, data );
	WriteWordExt (GetBaseAddrMaster(CreiteNum) + 22, 0x0 );
}
//===========================================================================
void GetTimer (byte CreiteNum)
{
	word data=0;
#ifdef MASTER_DEBUG
	printf("GetTimer\n");
#endif
	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 16 );
	dns = ( data & 0x7f );
	mks = ( data & 0xff80 ) >> 7;
	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 18 );
	mks |= (data & 1) << 9;
	mls = ( data & 0x7fe ) >> 1;
	s   = ( data & 0xf800 ) >> 11;
	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 20 );
	s |= (data & 1) << 5;

	m  = ( data & 0x7e ) >> 1;
	h = ( data & 0xf80 ) >> 7;
	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 22 );
	if ( ( data & ( 1 << 16 ) ) != 0 )
		printf("!!!   ERROR CODE SYNCHRO :  %i  !!!\n", data);

}
//===========================================================================
void GetTimeEvent (byte CreiteNum, byte Bank)
{
	word data=0;

#ifdef MASTER_DEBUG
	printf("GetTimeEvent\n");
#endif

	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 24 + Bank*8 );
	dns = ( data & 0x7f );
	mks = ( data & 0xff80 ) >> 7;
	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 26 + Bank*8 );
	mks |= (data & 1) << 9;
	mls = ( data & 0x7fe ) >> 1;
	s   = ( data & 0xf800 ) >> 11;
	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 28 + Bank*8 );
	s |= (data & 1) << 5;
	m  = ( data & 0x7e ) >> 1;
	h = ( data & 0xf80 ) >> 7;
	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 30 + Bank*8 );
	if ( ( data & ( 1 << 16 ) ) != 0 )
		printf("!!!   ERROR CODE SYNCHRO :  %i  !!!\n", data);
}
//===========================================================================
longw GetBaseAddrCenter(byte CreiteNum)
{
	return ((longw) CreiteNum<<23 | (longw)BaseAddrCenter );
}
//===========================================================================
void StartTimer (byte CreiteNum)
{
#ifdef MASTER_DEBUG
	printf("StartTimer\n");
#endif
	WriteWordExt (GetBaseAddrCenter(CreiteNum)+4, 0x1 );
}
//===========================================================================
void StartApparatSynchro (byte CreiteNum)
{
#ifdef MASTER_DEBUG
	printf("StartApparatSynchro\n");
#endif
	WriteWordExt (GetBaseAddrCenter(CreiteNum)+4, 0x1 );
}
//===========================================================================
void ProhibiteApparatSynchro (byte CreiteNum)
{
#ifdef MASTER_DEBUG
	printf("ProhibiteApparatSynchro\n");
#endif
	WriteWordExt (GetBaseAddrCenter(CreiteNum)+4, 0x2 );
}
//===========================================================================
void ResolveApparatSynchro (byte CreiteNum)
{
#ifdef MASTER_DEBUG
	printf("ResolveApparatSynchro\n");
#endif
	WriteWordExt (GetBaseAddrCenter(CreiteNum)+4, 0x0 );
}
//===========================================================================
word GetStatusVME (byte CreiteNum)
{
	word data;

#ifdef MASTER_DEBUG
	printf("GetStatusVME\n");
#endif
	data = ReadWordExt (GetBaseAddrCenter(CreiteNum)+4 );
	printf("!!! ERROR CODE :  %i\n", ( data & 0x7 ) );
	return data;
}
//===========================================================================
word GetTimeDelay (byte CreiteNum)
{
	word data=0;

#ifdef MASTER_DEBUG
	printf("GetTimeDelay\n");
#endif
	data = ReadWordExt (GetBaseAddrCenter(CreiteNum)+10+CreiteNum*2 );
	printf("Time Delay =  %i x 10 nsec \n", data );
	return data;
}
//===========================================================================
void Timer_Init ( byte NclastReady, byte *ClastReady )
{
	byte ii, Claster;

#ifdef MASTER_DEBUG
	printf("Timer_Init\n");
#endif

	for ( ii=0; ii<NclastReady; ii++) {
		Claster = ClastReady[ii];
		SetBeginTime(Claster,0,0,0,0,0,0);
	}

	for ( ii=0; ii<NclastReady; ii++) {
		Claster = ClastReady[ii];
		ProhibiteApparatSynchro(Claster);
	}

	for ( ii=0; ii<NclastReady; ii++) {
		Claster = ClastReady[ii];
		ResolveApparatSynchro(Claster);
	}

	for ( ii=0; ii<NclastReady; ii++) {
		Claster = ClastReady[ii];
		SetInitTimer(Claster);
	}
	StartApparatSynchro(0);
	sleep(2);
}
//====================================================================
