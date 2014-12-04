//===========================================================================
#include "master.h"

//#define MASTER_DEBUG

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
 //  Bank = 0 or 1
{
//int kk=0,qq=0;

//    if ( Bank!=0 || Bank!=1 ) return;

#ifdef MASTER_DEBUG
printf("ResetEvent\n");
#endif
    if ( Bank==0 ) 
	WriteWordExt ( GetBaseAddrMaster(CreiteNum), 0x1, Std_NoPriv_Data );

    if ( Bank==1 ) 
	WriteWordExt ( GetBaseAddrMaster(CreiteNum), 0x2, Std_NoPriv_Data );



//delay(10);
/*
	kk=0;
	while ( (qq = ReadWordExt( GetBaseAddrCenter(CreiteNum)+0x4, Std_NoPriv_Data )) != 0) {

		 if ( Bank==0 )
		WriteWordExt ( GetBaseAddrMaster(CreiteNum), 0x1, Std_NoPriv_Data );

		 if ( Bank==1 )
		WriteWordExt ( GetBaseAddrMaster(CreiteNum), 0x2, Std_NoPriv_Data );

		kk++ ;
	}
	if (kk) {
	printf("RESET OLD  kk = %i   press any key\n", kk);
	getch();

	}
*/
}
//===========================================================================
/*
void ResetOldEvent (byte CreiteNum)
{
int kk=0,qq=0;
longw Addr=GetBaseAddrMaster(CreiteNum);

	 WriteWordExt ( Addr, 0x1, Std_NoPriv_Data );
//	 WriteWordExt ( GetBaseAddrMaster(CreiteNum), 0x1, Std_NoPriv_Data );
delay(10);
	kk=0;
	while ( (qq = ReadWordExt( GetBaseAddrCenter(CreiteNum)+0x4, Std_NoPriv_Data )) != 0) {
	 WriteWordExt ( GetBaseAddrMaster(CreiteNum), 0x1, Std_NoPriv_Data );
		kk++ ;
	}
	if (kk) {
	printf("RESET OLD  kk = %i   press any key\n", kk);
	getch();

	}
}
*/
//===========================================================================
/*
void ResetAllEvents (byte CreiteNum)
{
int kk=0,qq=0;
longw Addr=GetBaseAddrMaster(CreiteNum);

//	 WriteWordExt ( GetBaseAddrMaster(CreiteNum), 0x2, Std_NoPriv_Data );
	 WriteWordExt ( Addr, 0x2, Std_NoPriv_Data );
delay(10);
	kk=0;
	while ( (qq = ReadWordExt( GetBaseAddrCenter(CreiteNum)+0x4, Std_NoPriv_Data )) != 0) {
	 WriteWordExt ( GetBaseAddrMaster(CreiteNum), 0x2, Std_NoPriv_Data );
		kk++ ;
	}
	if (kk) {
	printf("RESET ALL  kk = %i press any key\n", kk);
	getch();
	}
}
*/
//===========================================================================
int GetStatusEvent (byte CreiteNum)
{
#ifdef MASTER_DEBUG
printf("GetStatusEvent\n");
#endif
	return ReadWordExt( GetBaseAddrMaster(CreiteNum), Std_NoPriv_Data );
}
//===========================================================================
void SetCoinsidence (byte CreiteNum, int Number)
{
#ifdef MASTER_DEBUG
printf("SetCoinsidence\n");
#endif
	WriteWordExt (GetBaseAddrMaster(CreiteNum)+6, Number, Std_NoPriv_Data );
}
//===========================================================================
int GetCoinsidence (byte CreiteNum)
{
//printf("GetCoinsidence  Addr=0x%x\n", GetBaseAddrMaster(CreiteNum)+6);
#ifdef MASTER_DEBUG
printf("GetCoinsidence\n");
#endif
	return ReadWordExt (GetBaseAddrMaster(CreiteNum)+6, Std_NoPriv_Data );
}
//===========================================================================
void SetInitTimer (byte CreiteNum)
{
#ifdef MASTER_DEBUG
printf("SetInitTimer\n");
#endif
	 WriteWordExt (GetBaseAddrMaster(CreiteNum)+2, 0x1, Std_NoPriv_Data );
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
//    print_in_bits(data);
	 WriteWordExt (GetBaseAddrMaster(CreiteNum) + 16, data, Std_NoPriv_Data );
//	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 16, Std_NoPriv_Data );
//	 print_in_bits(data);



	 data = sec << 10 | mlsec;
//    print_in_bits(data);
	 WriteWordExt (GetBaseAddrMaster(CreiteNum) + 18, data, Std_NoPriv_Data );
//	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 18, Std_NoPriv_Data );
//	 print_in_bits(data);


	 data = hour << 6 | min;
//    print_in_bits(data);
	 WriteWordExt (GetBaseAddrMaster(CreiteNum) + 20, data, Std_NoPriv_Data );
//	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 20, Std_NoPriv_Data );
//	 print_in_bits(data);


	 WriteWordExt (GetBaseAddrMaster(CreiteNum) + 22, 0x0, Std_NoPriv_Data );

//	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 22, Std_NoPriv_Data );
//	 print_in_bits(data);

}
//===========================================================================
void GetTimer (byte CreiteNum)
{
word data=0;
int kk=0,qq=0;
//double TIME=0L,add=0l,frac=0l;


#ifdef MASTER_DEBUG
printf("GetTimer\n");
#endif

//	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 24, Std_NoPriv_Data );
	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 16, Std_NoPriv_Data );

/*
	kk=0;
	while ( (qq = ReadWordExt( GetBaseAddrCenter(CreiteNum)+0x4, Std_NoPriv_Data )) != 0) {
//	 print_in_bits(data);

		data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 16, Std_NoPriv_Data );
		kk++ ;
		printf("GetTimeEvent(16):   Claster=%i  kk = %i data=%i\n", 
		CreiteNum+1,kk,data);
	}
	if (kk) printf("GetTimeEvent(16):   Claster=%i  kk = %i \n", CreiteNum+1,kk);
*/


//	TIME = (double)data;
//	 print_in_bits(data);
//	 print_in_bits(TIME);

//	print_in_bits(GetBaseAddrMaster(CreiteNum) + 16);
//    data = 0x3228;
	dns = ( data & 0x7f );
	mks = ( data & 0xff80 ) >> 7;
//    print_in_bits(0x7f);
//    print_in_bits(0xff80);
//	print_in_bits(data);
//    print_in_bits(*dnsec);
//    print_in_bits(*mksec);

//	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 26, Std_NoPriv_Data );
	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 18, Std_NoPriv_Data );
/*
	kk=0;
	while ( (qq = ReadWordExt( GetBaseAddrCenter(CreiteNum)+0x4, Std_NoPriv_Data )) != 0) {
//	 print_in_bits(data);
		data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 18, Std_NoPriv_Data );
		kk++ ;
		printf("GetTimeEvent(18):   Claster=%i  kk = %i data=%i\n", 
		CreiteNum+1,kk,data);
	}
	if (kk) printf("GetTimeEvent(18):   Claster=%i  kk = %i \n", CreiteNum+1,kk);
*/


//	print_in_bits(GetBaseAddrMaster(CreiteNum) + 18);

//	TIME += (double)data * pow(2,16);
//	 print_in_bits(data);
//	 print_in_bits(TIME);

//    data = 0x3c1e;
	mks |= (data & 1) << 9;
	mls = ( data & 0x7fe ) >> 1;
	s   = ( data & 0xf800 ) >> 11;
//    print_in_bits(*mksec);
//    print_in_bits(0x3ff);
//    print_in_bits(0xfc00);
//	print_in_bits(data);
//    print_in_bits(*mlsec);
//    print_in_bits(*sec);

//	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 28, Std_NoPriv_Data );
	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 20, Std_NoPriv_Data );
/*
	kk=0;
	while ( (qq = ReadWordExt( GetBaseAddrCenter(CreiteNum)+0x4, Std_NoPriv_Data )) != 0) {
//	 print_in_bits(data);
		data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 20, Std_NoPriv_Data );
		kk++ ;
		printf("GetTimeEvent(20):   Claster=%i  kk = %i data=%i\n", 
		CreiteNum+1,kk,data);
	}
	if (kk) printf("GetTimeEvent(20):   Claster=%i  kk = %i \n", CreiteNum+1,kk);
*/

//	TIME += (double)data * pow(2,16*2);
//	 print_in_bits(data);
//	 print_in_bits(TIME);

	s |= (data & 1) << 5;

//    data = 0xf294;
	m  = ( data & 0x7e ) >> 1;
	h = ( data & 0xf80 ) >> 7;
//    print_in_bits(0x3f);
//    print_in_bits(0xfc0);
//	print_in_bits(data);
//    print_in_bits(*min);
//    print_in_bits(*hour);

//	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 30, Std_NoPriv_Data );
	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 22, Std_NoPriv_Data );

/*
	kk=0;
	while ( (qq = ReadWordExt( GetBaseAddrCenter(CreiteNum)+0x4, Std_NoPriv_Data )) != 0) {
//	 print_in_bits(data);
		data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 22, Std_NoPriv_Data );
		kk++ ;
		printf("GetTimeEvent(22):   Claster=%i  kk = %i data=%i\n", 
		CreiteNum+1,kk,data);
	}
	if (kk) printf("GetTimeEvent(22):   Claster=%i  kk = %i \n", CreiteNum+1,kk);
*/

	if ( ( data & ( 1 << 16 ) ) != 0 )
		printf("!!!   ERROR CODE SYNCHRO :  %i  !!!\n", data);

//	printf ("h=%i  m=%i  s=%i  mls=%i  mks=%i  dns=%i\n", h,m,s,mls,mks,dns);

}
//===========================================================================
void GetTimeEvent (byte CreiteNum, byte Bank)
{
word data=0;
int kk=0,qq=0;
//double TIME=0L,add=0l,frac=0l;


#ifdef MASTER_DEBUG
printf("GetTimeEvent\n");
#endif

//	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 24, Std_NoPriv_Data );
	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 24 + Bank*8, Std_NoPriv_Data );

/*
	kk=0;
	while ( ((qq = ReadWordExt( GetBaseAddrCenter(CreiteNum)+0x4, Std_NoPriv_Data )) != 0) 
		&& (kk<10) ){
//	 print_in_bits(data);

		data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 24 + Bank*8, Std_NoPriv_Data );
		kk++ ;
		printf("GetTimeEvent(24):   Claster=%i  kk = %i data=%i\n", 
		CreiteNum+1,kk,data);
		if (kk>5) continue;
	}
	if (kk) printf("GetTimeEvent(24):   Claster=%i  kk = %i \n", CreiteNum+1,kk);
*/


//	TIME = (double)data;
//	 print_in_bits(data);
//	 print_in_bits(TIME);

//	print_in_bits(GetBaseAddrMaster(CreiteNum) + 16);
//    data = 0x3228;
	dns = ( data & 0x7f );
	mks = ( data & 0xff80 ) >> 7;
//    print_in_bits(0x7f);
//    print_in_bits(0xff80);
//	print_in_bits(data);
//    print_in_bits(*dnsec);
//    print_in_bits(*mksec);

//	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 26, Std_NoPriv_Data );
	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 26 + Bank*8, Std_NoPriv_Data );
/*
	kk=0;
	while ( ((qq = ReadWordExt( GetBaseAddrCenter(CreiteNum)+0x4, Std_NoPriv_Data )) != 0) 
		&& (kk<10) ) {
//	 print_in_bits(data);
		data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 26 + Bank*8, Std_NoPriv_Data );
		kk++ ;
		printf("GetTimeEvent(26):   Claster=%i  kk = %i data=%i\n", 
		CreiteNum+1,kk,data);
	}
	if (kk) printf("GetTimeEvent(26):   Claster=%i  kk = %i \n", CreiteNum+1,kk);
*/


//	print_in_bits(GetBaseAddrMaster(CreiteNum) + 18);

//	TIME += (double)data * pow(2,16);
//	 print_in_bits(data);
//	 print_in_bits(TIME);

//    data = 0x3c1e;
	mks |= (data & 1) << 9;
	mls = ( data & 0x7fe ) >> 1;
	s   = ( data & 0xf800 ) >> 11;
//    print_in_bits(*mksec);
//    print_in_bits(0x3ff);
//    print_in_bits(0xfc00);
//	print_in_bits(data);
//    print_in_bits(*mlsec);
//    print_in_bits(*sec);

//	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 28, Std_NoPriv_Data );
	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 28 + Bank*8, Std_NoPriv_Data );
/*
	kk=0;
	while ( ((qq = ReadWordExt( GetBaseAddrCenter(CreiteNum)+0x4, Std_NoPriv_Data )) != 0) 
		&& (kk<10) ) {
//	 print_in_bits(data);
		data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 28 + Bank*8, Std_NoPriv_Data );
		kk++ ;
		printf("GetTimeEvent(28):   Claster=%i  kk = %i data=%i\n", 
		CreiteNum+1,kk,data);
	}
	if (kk) printf("GetTimeEvent(28):   Claster=%i  kk = %i \n", CreiteNum+1,kk);
*/

//	TIME += (double)data * pow(2,16*2);
//	 print_in_bits(data);
//	 print_in_bits(TIME);

	s |= (data & 1) << 5;

//    data = 0xf294;
	m  = ( data & 0x7e ) >> 1;
	h = ( data & 0xf80 ) >> 7;
//    print_in_bits(0x3f);
//    print_in_bits(0xfc0);
//	print_in_bits(data);
//    print_in_bits(*min);
//    print_in_bits(*hour);

//	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 30, Std_NoPriv_Data );
	data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 30 + Bank*8, Std_NoPriv_Data );
/*

	kk=0;
	while ( ((qq = ReadWordExt( GetBaseAddrCenter(CreiteNum)+0x4, Std_NoPriv_Data )) != 0) 
		&& (kk<10) ) {
//	 print_in_bits(data);
		data = ReadWordExt (GetBaseAddrMaster(CreiteNum) + 30 + Bank*8, Std_NoPriv_Data );
		kk++ ;
		printf("GetTimeEvent(30):   Claster=%i  kk = %i data=%i\n", 
		CreiteNum+1,kk,data);
	}
	if (kk) printf("GetTimeEvent(30):   Claster=%i  kk = %i \n", CreiteNum+1,kk);
*/

	if ( ( data & ( 1 << 16 ) ) != 0 )
		printf("!!!   ERROR CODE SYNCHRO :  %i  !!!\n", data);

//	printf ("h=%i  m=%i  s=%i  mls=%i  mks=%i  dns=%i\n", h,m,s,mls,mks,dns);

}
//===========================================================================
//===========================================================================
//===========================================================================
//===========================================================================
longw GetBaseAddrCenter(byte CreiteNum)
{
return ((longw) CreiteNum<<23 | (longw)BaseAddrCenter );
//return ( (longw)BaseAddrCenter );
}
//===========================================================================
void StartTimer (byte CreiteNum)
{
#ifdef MASTER_DEBUG
printf("StartTimer\n");
#endif
	WriteWordExt (GetBaseAddrCenter(CreiteNum)+4, 0x1, Std_NoPriv_Data );
}
//===========================================================================
void StartApparatSynchro (byte CreiteNum)
{
#ifdef MASTER_DEBUG
printf("StartApparatSynchro\n");
#endif
    WriteWordExt (GetBaseAddrCenter(CreiteNum)+4, 0x1, Std_NoPriv_Data );
}
//===========================================================================
void ProhibiteApparatSynchro (byte CreiteNum)
{
#ifdef MASTER_DEBUG
printf("ProhibiteApparatSynchro\n");
#endif
    WriteWordExt (GetBaseAddrCenter(CreiteNum)+4, 0x2, Std_NoPriv_Data );
}
//===========================================================================
void ResolveApparatSynchro (byte CreiteNum)
{
#ifdef MASTER_DEBUG
printf("ResolveApparatSynchro\n");
#endif
    WriteWordExt (GetBaseAddrCenter(CreiteNum)+4, 0x0, Std_NoPriv_Data );
}
//===========================================================================
word GetStatusVME (byte CreiteNum)
{
word data;

#ifdef MASTER_DEBUG
printf("GetStatusVME\n");
#endif
    data = ReadWordExt (GetBaseAddrCenter(CreiteNum)+4, Std_NoPriv_Data );
	 printf("!!! ERROR CODE :  %i\n", ( data & 0x7 ) );
return data;
}
//===========================================================================
word GetTimeDelay (byte CreiteNum)
   // in 10 nsec
{
word data=0;

#ifdef MASTER_DEBUG
printf("GetTimeDelay\n");
#endif
    data = ReadWordExt (GetBaseAddrCenter(CreiteNum)+10+CreiteNum*2, Std_NoPriv_Data );
//    print_in_bits(data);
    printf("Time Delay =  %i x 10 nsec \n", data );
return data;
}
//===========================================================================
void Timer_Init ( byte NclastReady, byte *ClastReady )
//void Timer_Init ( void )
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


//	for ( ii=0; ii<NclastReady; ii++) {
//		Claster = ClastReady[ii];
//		GetTimeEvent (Claster);
//	}


	StartApparatSynchro(0);

	sleep(2);
//	delay(500);



/*
	for ( ii=0; ii<NclastReady; ii++) {
		Claster = ClastReady[ii];
		printf("Claster=%i  Bank0  ",Claster);
		GetTimeEvent (Claster, 0);
//		delay(500);
		printf("Claster=%i  Bank1  ",Claster);
		GetTimeEvent (Claster, 1);
	}
*/

}
//====================================================================
