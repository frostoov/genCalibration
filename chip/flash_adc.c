//===========================================================================
#include "flash_adc.h"

//#define FADC_DEBUG

#define BaseAddrFADC 0x80000
//===========================================================================
longw GetBaseAddrFADC(byte CreiteNum, byte PlataNum)
{
return (((longw)CreiteNum<<23) | (longw)BaseAddrFADC | (((longw)PlataNum*2)<<16));
//return ( (longw)BaseAddrFADC | (((longw)PlataNum*2)<<16));
/*
	if ( PlataNum == 0 ) return (BaseAddrFADC);
	else if ( PlataNum == 1 ) return (BaseAddrFADC | 0x20000);
	else if ( PlataNum == 2 ) return (BaseAddrFADC | 0x40000);
	else if ( PlataNum == 3 ) return (BaseAddrFADC | 0x60000);
	else return 0;
*/
}
//===========================================================================
/*
void ResetADC (byte CreiteNum, byte PlataNum)
{
int kk=0,qq=0;
longw Addr = GetBaseAddrFADC(CreiteNum,PlataNum)+0x10000+28;

//	WriteWordExt ( GetBaseAddrFADC(CreiteNum,PlataNum)+0x4000+28, 1, Std_NoPriv_Data );
	WriteWordExt ( Addr, 1, Std_NoPriv_Data );
*/
/*
	delay(100);

	kk=0;
	while ( (qq = ReadWordExt( GetBaseAddrCenter(CreiteNum)+0x4, Std_NoPriv_Data )) != 0) {
		WriteWordExt ( GetBaseAddrFADC(CreiteNum,PlataNum)+0x4000+28, 1, Std_NoPriv_Data );
		kk++ ;
	}
	if (kk) {
	printf("RESET FADC:    Claster=%i  kk = %i press any key\n", CreiteNum+1,kk);
	getchar();
	}
*/
/*
}
*/
//===========================================================================
void ResetHIST (byte CreiteNum, byte PlataNum, byte ChannelNum)
{
//longw Addr = GetBaseAddrFADC(CreiteNum,PlataNum)+0x4000;
longw Addr = GetBaseAddrFADC(CreiteNum,PlataNum)+0x10000;

    if (ChannelNum == 0) 
	WriteWordExt (Addr+24, 1, Std_NoPriv_Data );

	 if (ChannelNum == 1 )
        WriteWordExt ( Addr+26, 1, Std_NoPriv_Data );

}
//===========================================================================
void SetProgramTrigger (byte CreiteNum, byte PlataNum)
{
#ifdef FADC_DEBUG
printf("SetProgramTrigger\n");
#endif
//	WriteWordExt ( GetBaseAddrFADC(CreiteNum,PlataNum)+0x4000+28, 0x2, Std_NoPriv_Data );
	WriteWordExt ( GetBaseAddrFADC(CreiteNum,PlataNum)+0x10000+28, 0x2, Std_NoPriv_Data );
}
//===========================================================================
void WriteStatusRegister ( byte CreiteNum, byte PlataNum,
				byte BlockRequest0,
			   byte TriggerRoutine0,
			   byte RequestAsTrigger0,
			   byte BlockRequest1,
			   byte TriggerRoutine1,
			   byte RequestAsTrigger1 )
{

word Data=0, Data0=0, Data1=0;
//longw Addr = GetBaseAddrFADC(CreiteNum,PlataNum)+0x4000;
longw Addr = GetBaseAddrFADC(CreiteNum,PlataNum)+0x10000;


#ifdef FADC_DEBUG
printf("WriteStatusRegister\n");
#endif
	Data0 = (4*BlockRequest0) | (8*TriggerRoutine0) | (16*RequestAsTrigger0);
	Data1 = ( (4*BlockRequest1) | (8*TriggerRoutine1) | (16*RequestAsTrigger1) ) << 8;
	Data = Data0 | Data1;
	WriteWordExt ( Addr, Data, Std_NoPriv_Data );

}
//===========================================================================
void SetTriggerExternal ( byte CreiteNum, byte PlataNum )
{
#ifdef FADC_DEBUG
printf("SetTriggerExternal\n");
#endif
	WriteStatusRegister ( CreiteNum, PlataNum, 0, 0, 0, 0, 0, 0 );
}
//===========================================================================
void SetTriggerProgramm ( byte CreiteNum, byte PlataNum )
{

#ifdef FADC_DEBUG
printf("SetTriggerProgramm\n");
#endif
	WriteStatusRegister ( CreiteNum, PlataNum, 1, 1, 0, 1, 1, 0 );
}
//===========================================================================
void SetTriggerSelf ( byte CreiteNum, byte PlataNum )
{
#ifdef FADC_DEBUG
printf("SetTriggerSelf\n");
#endif
	WriteStatusRegister ( CreiteNum, PlataNum, 1, 0, 1, 1, 0, 1 );
}
//===========================================================================
void BlockedChannel ( byte CreiteNum, byte PlataNum, byte ChannelNum )
{
word Data=0, Data0=0, Data1=0;
longw Addr = GetBaseAddrFADC(CreiteNum,PlataNum)+0x10000;

#ifdef FADC_DEBUG
printf("BlockedChannel\n");
#endif
	Data0 = ReadWordExt ( Addr, Std_NoPriv_Data );
	Data1 = (4*1) << 8*ChannelNum;
	Data = Data0 | Data1;
	WriteWordExt ( Addr, Data, Std_NoPriv_Data );
}
//===========================================================================
void SetThreshold ( byte CreiteNum, byte PlataNum, byte ChannelNum, word Threshold )
{
//longw Addr = GetBaseAddrFADC(CreiteNum,PlataNum)+0x4000;
longw Addr = GetBaseAddrFADC(CreiteNum,PlataNum)+0x10000;

#ifdef FADC_DEBUG
printf("SetThreshold\n");
#endif
	if ( ChannelNum == 0 )
		{ WriteWordExt ( Addr + 16, (2048+Threshold), Std_NoPriv_Data ); }

	if ( ChannelNum == 1 )
		{ WriteWordExt ( Addr + 20, (2048+Threshold), Std_NoPriv_Data ); }
}
//===========================================================================
void SetDelayRequest ( byte CreiteNum, byte PlataNum, byte ChannelNum, word DelayRequest )
{
//longw Addr = GetBaseAddrFADC(CreiteNum,PlataNum)+0x4000;
longw Addr = GetBaseAddrFADC(CreiteNum,PlataNum)+0x10000;

#ifdef FADC_DEBUG
printf("SetDelayRequest\n");
#endif
	if ( ChannelNum == 0 )
		{ WriteWordExt ( Addr | 18, DelayRequest, Std_NoPriv_Data ); }

	if ( ChannelNum == 1 )
		{ WriteWordExt ( Addr | 22, DelayRequest, Std_NoPriv_Data ); }
}
//===========================================================================
word GetStatusRegister ( byte CreiteNum, byte PlataNum, 
                         byte ChannelNum, byte Bank, byte Flag)
//if Flag != 0 then print status register to display
{
word dat, Ask;
int kk=0,qq=0;
//longw Addr = GetBaseAddrFADC(CreiteNum,PlataNum)+0x4000;
longw Addr = GetBaseAddrFADC(CreiteNum,PlataNum)+0x10000;


#ifdef FADC_DEBUG
printf("GetStatusRegister\n");
#endif

	Ask = ReadWordExt ( Addr, Std_NoPriv_Data );

/*
	kk=0;
	while ( (qq = ReadWordExt( GetBaseAddrCenter(CreiteNum)+0x4, Std_NoPriv_Data )) != 0) {
//		print_in_bits ( Ask );
		Ask = ReadWordExt ( Addr, Std_NoPriv_Data );
		kk++ ;
	}
	if (kk) {
		printf("GetStatusRegister:   Claster=%i   kk = %i    press any key\n", CreiteNum+1, kk);
//		getchar();
	}
*/

	if ( Bank==0 ) dat = (word) (Ask >> (8*ChannelNum));

	if ( Bank==1 ) dat = (word) ( (Ask >> 6) >> (8*ChannelNum));
	



if (Flag != 0)
{
	printf( "Status register: \n" );
	print_in_bits ( Ask );

/*
	printf( "For Channel Num = %i      Status Regist is :\n", ChannelNum );

	if ( dat & 1 )  printf("Ready is 1\n");
	else            printf("Ready is 0\n");

	if ( dat & 2 )  printf("HIST Overflow is 1\n");
	else            printf("HIST Overflow is 0\n");

	if ( dat & 4 )  printf("BlockRequest is 1\n");
	else            printf("BlockRequest is 0\n");

	if ( dat & 8 )  printf("TriggerRoutine is 1\n");
	else            printf("TriggerRoutine is 0\n");

	if ( dat & 16 ) printf("RequestAsTrigger is 1\n");
	else            printf("RequestAsTrigger is 0\n");
*/
}
return dat;
}
//===========================================================================
/*
word Read_ADC_Word ( byte CreiteNum, byte PlataNum, longw AddrData )
{
word dat;
longw Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + AddrData;

	dat = ReadWordExt (Addr, Std_NoPriv_Data );
	dat += 0xF800;
	printf( "In Addres %lX   ADC Word is:   ", Addr );
	print_in_bits ( dat );

return dat;
}
*/
//===========================================================================
void BlockRead_ADC(byte CreiteNum, byte PlataNum,
 byte ChannelNum,
                   longw StartAddr, word WordsForRead,

	int *DataMas)
//	int DataMas[1048])
{
word j=0L;
FILE *fsub, *fff;
int kk=0;
int qq=0;
longw Addr=0L;

#ifdef FADC_DEBUG
printf("BlockRead_ADC\n");
printf("Addr = %lX  CreiteNum=%i,  PlataNum=%i,  ChannelNum=%i\n ", 
Addr,CreiteNum,PlataNum,ChannelNum);
#endif

	 if ( ChannelNum == 0 ) {
	Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + StartAddr;
//	print_in_bits ( Addr );
	}


	 if ( ChannelNum == 1 ) {
	Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + 0x4000 + StartAddr;
//	print_in_bits ( Addr );
	}




	printf("Addr = %lX  WordsForRead=%ld  j=%ld\n ", Addr,WordsForRead,j);


//	ferr = fopen("err.dat","w");
//	fclose(ferr);
//	ferr = fopen("err.dat","a");
//	fprintf(ferr,"Event = %9ld\n",NEV);


	if (WordsForRead == 0) return;
//	while ( j < 3 )
	while ( j < WordsForRead )
	{
//		delay (10);
/*
		if ( Addr>=9310340 ) {
		printf("Claster=%d   Plata=%d  j=%i  Addr=%ld(%lX)\n",CreiteNum,PlataNum,j,Addr,Addr);
		}

		if ( Addr>=9310342 ) {
//		printf("Claster=%d   Plata=%d  j=%i  Addr=%ld(%lX)\n",CreiteNum,PlataNum,j,Addr,Addr);
			DataMas[j] = 2048;
			j++;
			Addr+=2;
			continue;
		}
*/
//	printf("Addr = %lX   ", Addr);
		DataMas[j] = ReadWordExt ( Addr, Std_NoPriv_Data );

//	printf("Addr = %lx   Data = %i\n", Addr, DataMas[j]);



//		if ( Addr>=9310340 ) {
//		printf("Claster=%d   Plata=%d  j=%i  Addr=%ld(%lX)  :=%i\n",CreiteNum,PlataNum,j,Addr,Addr,DataMas[j]);
//		}

//		printf("Claster=%d   Plata=%d  j=%i  Addr=%ld(%lX)\n",CreiteNum,PlataNum,j,Addr,Addr);
//		getchar();
//	printf("Addr = %lx   kk = %i   data = %i\n", Addr,kk, DataMas[j]);

/*
		kk=0;
//		while (DataMas[j] == -1) {
//		while ( (qq = ReadWordExt( 0xc00004, Std_NoPriv_Data )) != 0) {

//		printf("Claster=%d   Plata=%d  j=%i  Addr=%ld(%lX)\n",CreiteNum,PlataNum,j,Addr,Addr);

		while ( (qq = ReadWordExt( GetBaseAddrCenter(CreiteNum)+0x4, Std_NoPriv_Data )) != 0) {
//		   qq = ReadWordExt(0x400004, Std_NoPriv_Data );
//			while ( (  (word)DataMas[j] = ReadWordExt ( Addr, Std_NoPriv_Data )) == 0xffff);
			DataMas[j] = ReadWordExt ( Addr, Std_NoPriv_Data );
//           getchar();
			kk++ ;
			kerr++;
	printf("Addr = %lx   kk = %i   data = %i\n", Addr,kk, DataMas[j]);
//	fprintf(ferr,"Addr = %lx   kk = %i   qq = %i\n", Addr,kk,qq);

		}
*/


		if (j > 0) {
			if ((word)DataMas[j] == 0xffff) {
//		printf("%i   %i   qq=%i\n", DataMas[j], DataMas[j-1], qq);
				DataMas[j] = DataMas[j-1];
			}
		}


		DataMas[j] &= 4095;

//		if(kk) 	fprintf(ferr,"Addr = %lx   kk = %i\n", Addr,kk);
//		if(kerr) 	printf("!!!   ERROR   !!!   Addr = %lx   kk = %i\n", Addr,kerr);
		j++;
		Addr+=2;
//	printf("Addr = %lX\n", Addr);
	}

/*
	fsub = fopen("block.dat","w");
	for( j=0; j<WordsForRead; j++ ) {
		fprintf ( fsub, "  %ld   %i\n", (longw)(j*2)+StartAddr, DataMas[j] );
//		printf ( "Addr = %ld   Data = %i\n", (longw)(j*2)+StartAddr, DataMas[j] );
	}
	fclose(fsub);
*/
//	fclose(ferr);
}
//===========================================================================
/*
void GetOneChannelData ( byte CreiteNum, byte PlataNum, byte ChannelNum, int *DataMas )
{
word j;

	if ( ChannelNum == 0 ) BlockRead_ADC(CreiteNum,PlataNum, 0x0000, 1024, DataMas);
	if ( ChannelNum == 1 ) BlockRead_ADC(CreiteNum,PlataNum, 0x0800, 1024, DataMas);
	if ( ChannelNum == 2 ) BlockRead_ADC(CreiteNum,PlataNum, 0x1000, 1024, DataMas);
	if ( ChannelNum == 3 ) BlockRead_ADC(CreiteNum,PlataNum, 0x1800, 1024, DataMas);
	if ( ChannelNum >= 4 ) return;

	for (j=0; j<Aperture; j++)  {
		DataMas[j] = 2048 - DataMas[j];
	}

//	fsub = fopen("data.dat","w");
//	for (j=0; j<Aperture; j++)
// {
//		fprintf (fsub, "%4i  %5i  %5i\n",
// j,
// DataMas[j], data[j]);
//	}
//	fclose(fsub);

}
*/
//===========================================================================
//void GetPulseForm ( byte CreiteNum, byte PlataNum, int DataMas[4][1024] )
void GetPulseForm ( byte CreiteNum, byte PlataNum, byte Bank, int *DataMas )
{
word j;
//int data[4][Aperture];
//FILE *fsub;

//	BlockRead_ADC(CreiteNum,PlataNum, 0x0000, 1024, data[0]);
//	BlockRead_ADC(CreiteNum,PlataNum, 0x0800, 1024, data[1]);
//	BlockRead_ADC(CreiteNum,PlataNum, 0x1000, 1024, data[2]);
//	BlockRead_ADC(CreiteNum,PlataNum, 0x1800, 1024, data[3]);

#ifdef FADC_DEBUG
printf("GetPulseForm\n");
#endif

    if ( Bank==0 ) {
	BlockRead_ADC(CreiteNum,PlataNum, 0, 0x0000, 1024, DataMas);
	BlockRead_ADC(CreiteNum,PlataNum, 0, 0x0800, 1024, DataMas+1024);
//	print_in_bits ( 0x0C00 );

	BlockRead_ADC(CreiteNum,PlataNum, 1, 0x0000, 1024, DataMas+2048);
	BlockRead_ADC(CreiteNum,PlataNum, 1, 0x0800, 1024, DataMas+3072);
	 }
	 if ( Bank==1 ) {
	BlockRead_ADC(CreiteNum,PlataNum, 0, 0x1000, 1024, DataMas);
	BlockRead_ADC(CreiteNum,PlataNum, 0, 0x1800, 1024, DataMas+1024);

	BlockRead_ADC(CreiteNum,PlataNum, 1, 0x1000, 1024, DataMas+2048);
	BlockRead_ADC(CreiteNum,PlataNum, 1, 0x1800, 1024, DataMas+3072);
    }

	for (j=0; j<Aperture*4; j++) {
		DataMas[j] = 2048 - DataMas[j];
	}


//	for (j=0; j<Aperture; j++) {
//		DataMas[0][j] = 2048 - data [0][j];
//		DataMas[1][j] = 2048 - data [1][j];
//		DataMas[2][j] = 2048 - data [2][j];
//		DataMas[3][j] = 2048 - data [3][j];
//	}

//	fsub = fopen("data.dat","w");
//	for (j=0; j<Aperture; j++) {
//		fprintf (fsub, "%4i  %5i  %5i  %5i  %5i  %5i  %5i  %5i  %5i\n",
// j,
//			DataMas[0][j], DataMas[1][j], DataMas[2][j], DataMas[3][j],
//			data[0][j], data[1][j], data[2][j], data[3][j] );
//	}
//	fclose(fsub);

}
//===========================================================================
//void GetPulseForm ( byte CreiteNum, byte PlataNum, int DataMas[4][1024] )
void GetPulseFormDim ( byte CreiteNum, byte PlataNum, byte Bank, short int *DataMas )
{
word j;
longw Addr=0L;
//int data[4][Aperture];
//FILE *fsub;

//	BlockRead_ADC(CreiteNum,PlataNum, 0x0000, 1024, data[0]);
//	BlockRead_ADC(CreiteNum,PlataNum, 0x0800, 1024, data[1]);
//	BlockRead_ADC(CreiteNum,PlataNum, 0x1000, 1024, data[2]);
//	BlockRead_ADC(CreiteNum,PlataNum, 0x1800, 1024, data[3]);

#ifdef FADC_DEBUG
printf("GetPulseFormDim\n");
#endif
    if ( Bank==0 ) {
	Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + 0x0000;
#ifdef FADC_DEBUG
	printf("Addr = %lX\n ", Addr);
#endif
//	ReadDimExt12 ( Addr, 1024, DataMas );

	ReadDimExt12 ( Addr, 512, DataMas );
	ReadDimExt12 ( Addr+1024, 512, DataMas+512 );

//	ReadDimExt12 ( Addr, 256, DataMas );
//	ReadDimExt12 ( Addr, 256, DataMas+256 );
//	ReadDimExt12 ( Addr, 256, DataMas+512 );
//	ReadDimExt12 ( Addr, 256, DataMas+768 );

	Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + 0x0800;
#ifdef FADC_DEBUG
	printf("Addr = %lX\n ", Addr);
#endif
//	ReadDimExt12 ( Addr, 1024, DataMas+1024 );

	ReadDimExt12 ( Addr, 512, DataMas+1024 );
	ReadDimExt12 ( Addr+1024, 512, DataMas+(1024+512) );

//	ReadDimExt12 ( Addr, 256, DataMas+1024 );
//	ReadDimExt12 ( Addr, 256, DataMas+1024+256 );
//	ReadDimExt12 ( Addr, 256, DataMas+1024+512 );
//	ReadDimExt12 ( Addr, 256, DataMas+1024+768 );

	Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + 0x4000 + 0x0000;
#ifdef FADC_DEBUG
	printf("Addr = %lX\n ", Addr);
#endif
//	ReadDimExt12 ( Addr, 1024, DataMas+2048 );

	ReadDimExt12 ( Addr, 512, DataMas+2048 );
	ReadDimExt12 ( Addr+1024, 512, DataMas+(2048+512) );

//	ReadDimExt12 ( Addr, 256, DataMas+2048 );
//	ReadDimExt12 ( Addr, 256, DataMas+2048+256 );
//	ReadDimExt12 ( Addr, 256, DataMas+2048+512 );
//	ReadDimExt12 ( Addr, 256, DataMas+2048+768 );

	Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + 0x4000 + 0x0800;
#ifdef FADC_DEBUG
	printf("Addr = %lX\n ", Addr);
#endif
//	ReadDimExt12 ( Addr, 1024, DataMas+3072 );

	ReadDimExt12 ( Addr, 512, DataMas+3072 );
	ReadDimExt12 ( Addr+1024, 512, DataMas+(3072+512) );

//	ReadDimExt12 ( Addr, 256, DataMas+3072 );
//	ReadDimExt12 ( Addr, 256, DataMas+3072+256 );
//	ReadDimExt12 ( Addr, 256, DataMas+3072+512 );
//	ReadDimExt12 ( Addr, 256, DataMas+3072+768 );
    }
    if ( Bank==1 ) {

	Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + 0x1000;
#ifdef FADC_DEBUG
	printf("Addr = %lX\n ", Addr);
#endif
//	ReadDimExt12 ( Addr, 1024, DataMas );

	ReadDimExt12 ( Addr, 512, DataMas );
	ReadDimExt12 ( Addr+1024, 512, DataMas+512 );

//	ReadDimExt12 ( Addr, 256, DataMas );
//	ReadDimExt12 ( Addr, 256, DataMas+256 );
//	ReadDimExt12 ( Addr, 256, DataMas+512 );
//	ReadDimExt12 ( Addr, 256, DataMas+768 );

	Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + 0x1800;
#ifdef FADC_DEBUG
	printf("Addr = %lX\n ", Addr);
#endif
//	ReadDimExt12 ( Addr, 1024, DataMas+1024 );

	ReadDimExt12 ( Addr, 512, DataMas+1024 );
	ReadDimExt12 ( Addr+1024, 512, DataMas+(1024+512) );

//	ReadDimExt12 ( Addr, 256, DataMas+1024 );
//	ReadDimExt12 ( Addr, 256, DataMas+1024+256 );
//	ReadDimExt12 ( Addr, 256, DataMas+1024+512 );
//	ReadDimExt12 ( Addr, 256, DataMas+1024+768 );

	Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + 0x4000 + 0x1000;
#ifdef FADC_DEBUG
	printf("Addr = %lX\n ", Addr);
#endif
//	ReadDimExt12 ( Addr, 1024, DataMas+2048 );

	ReadDimExt12 ( Addr, 512, DataMas+2048 );
	ReadDimExt12 ( Addr+1024, 512, DataMas+(2048+512) );

//	ReadDimExt12 ( Addr, 256, DataMas+2048 );
//	ReadDimExt12 ( Addr, 256, DataMas+2048+256 );
//	ReadDimExt12 ( Addr, 256, DataMas+2048+512 );
//	ReadDimExt12 ( Addr, 256, DataMas+2048+768 );

	Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + 0x4000 + 0x1800;
#ifdef FADC_DEBUG
	printf("Addr = %lX\n ", Addr);
#endif
//	ReadDimExt12 ( Addr, 1024, DataMas+3072 );

	ReadDimExt12 ( Addr, 512, DataMas+3072 );
	ReadDimExt12 ( Addr+1024, 512, DataMas+(3072+512) );

//	ReadDimExt12 ( Addr, 256, DataMas+3072 );
//	ReadDimExt12 ( Addr, 256, DataMas+3072+256 );
//	ReadDimExt12 ( Addr, 256, DataMas+3072+512 );
//	ReadDimExt12 ( Addr, 256, DataMas+3072+768 );
    }

	for (j=0; j<Aperture*4; j++) {
		DataMas[j] = 2048 - DataMas[j];
	}

/*
	for (j=0; j<Aperture; j++) {
		DataMas[0][j] = 2048 - data [0][j];
		DataMas[1][j] = 2048 - data [1][j];
		DataMas[2][j] = 2048 - data [2][j];
		DataMas[3][j] = 2048 - data [3][j];
	}

	fsub = fopen("data.dat","w");
	for (j=0; j<Aperture; j++) {
		fprintf (fsub, "%4i  %5i  %5i  %5i  %5i  %5i  %5i  %5i  %5i\n",
 j,
			DataMas[0][j], DataMas[1][j], DataMas[2][j], DataMas[3][j],
			data[0][j], data[1][j], data[2][j], data[3][j] );
	}
	fclose(fsub);
*/
}
//===========================================================================
void GetHistogram ( byte CreiteNum, byte PlataNum, int *DataMas )
//void GetHistogram ( byte CreiteNum, byte PlataNum, int DataMas[][Aperture] )
{
word j;
FILE *fsub;

#ifdef FADC_DEBUG
printf("GetHistogram\n");
#endif
//	BlockRead_ADC(CreiteNum,PlataNum, 0x2000, 1024, DataMas[0]);
//	BlockRead_ADC(CreiteNum,PlataNum, 0x2800, 1024, DataMas[1]);
	BlockRead_ADC(CreiteNum,PlataNum, 0, 0x2000, 1024, DataMas);
	BlockRead_ADC(CreiteNum,PlataNum, 1, 0x2000, 1024, DataMas+1024);

/*
	fsub = fopen("hist.dat","w");
	for (j=0; j<Aperture; j++)
 {
		fprintf (fsub, "%4i  %5i  %5i\n", j-512, DataMas[0][j], DataMas[1][j] );
	}
	fclose(fsub);
*/
}
//===========================================================================
void GetHistogramDim ( byte CreiteNum, byte PlataNum, int *DataMas )
//void GetHistogram ( byte CreiteNum, byte PlataNum, int DataMas[][Aperture] )
{
word j;
FILE *fsub;
longw Addr=0L;

#ifdef FADC_DEBUG
printf("GetHistogram\n");
#endif
	Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + 0x2000;
#ifdef FADC_DEBUG
	printf("Addr = %lX\n ", Addr);
#endif
//	ReadDimExt ( Addr, 1024, DataMas );

	ReadDimExt ( Addr, 512, DataMas );
	ReadDimExt ( Addr+1024, 512, DataMas+512 );

	Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + 0x4000 + 0x2000;
#ifdef FADC_DEBUG
	printf("Addr = %lX\n ", Addr);
#endif
//	ReadDimExt ( Addr, 1024, DataMas+1024 );

	ReadDimExt ( Addr, 512, DataMas+1024 );
	ReadDimExt ( Addr+1024, 512, DataMas+(1024+512) );

/*
	for (j=0; j<Aperture; j++)
 {
		printf ("%4i  %5i  %5i\n", j-512, DataMas[j], DataMas[j+1024] );
	}
*/



/*
	fsub = fopen("h.dat","w");
	for (j=512; j<Aperture; j++)
 {
		fprintf (fsub, "%4i  %5i  %5i\n", j-512, DataMas[j], DataMas[j+1024] );
	}
	fclose(fsub);
*/

}
//===========================================================================
