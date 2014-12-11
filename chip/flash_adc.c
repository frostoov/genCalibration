//===========================================================================
#include "flash_adc.h"

#define BaseAddrFADC 0x80000
//===========================================================================
longw GetBaseAddrFADC(byte CreiteNum, byte PlataNum)
{
	return (((longw)CreiteNum<<23) | (longw)BaseAddrFADC | (((longw)PlataNum*2)<<16));
}
//===========================================================================
void ResetHIST (byte CreiteNum, byte PlataNum, byte ChannelNum)
{
	longw Addr = GetBaseAddrFADC(CreiteNum,PlataNum)+0x10000;

	if (ChannelNum == 0)
		WriteWordExt (Addr+24, 1);

	if (ChannelNum == 1 )
		WriteWordExt ( Addr+26, 1);

}
//===========================================================================
void SetProgramTrigger (byte CreiteNum, byte PlataNum)
{
#ifdef FADC_DEBUG
	printf("SetProgramTrigger\n");
#endif
	WriteWordExt ( GetBaseAddrFADC(CreiteNum,PlataNum)+0x10000+28, 0x2);
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
	longw Addr = GetBaseAddrFADC(CreiteNum,PlataNum)+0x10000;


#ifdef FADC_DEBUG
	printf("WriteStatusRegister\n");
#endif
	Data0 = (4*BlockRequest0) | (8*TriggerRoutine0) | (16*RequestAsTrigger0);
	Data1 = ( (4*BlockRequest1) | (8*TriggerRoutine1) | (16*RequestAsTrigger1) ) << 8;
	Data = Data0 | Data1;
	WriteWordExt ( Addr, Data);

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
	Data0 = ReadWordExt ( Addr);
	Data1 = (4*1) << 8*ChannelNum;
	Data = Data0 | Data1;
	WriteWordExt ( Addr, Data);
}
//===========================================================================
void SetThreshold ( byte CreiteNum, byte PlataNum, byte ChannelNum, word Threshold )
{
	longw Addr = GetBaseAddrFADC(CreiteNum,PlataNum)+0x10000;

#ifdef FADC_DEBUG
	printf("SetThreshold\n");
#endif
	if ( ChannelNum == 0 )
	{ WriteWordExt ( Addr + 16, (2048+Threshold)); }

	if ( ChannelNum == 1 )
	{ WriteWordExt ( Addr + 20, (2048+Threshold)); }
}
//===========================================================================
void SetDelayRequest ( byte CreiteNum, byte PlataNum, byte ChannelNum, word DelayRequest )
{
	longw Addr = GetBaseAddrFADC(CreiteNum,PlataNum)+0x10000;

#ifdef FADC_DEBUG
	printf("SetDelayRequest\n");
#endif
	if ( ChannelNum == 0 )
	{ WriteWordExt ( Addr | 18, DelayRequest); }

	if ( ChannelNum == 1 )
	{ WriteWordExt ( Addr | 22, DelayRequest); }
}
//===========================================================================
word GetStatusRegister ( byte CreiteNum, byte PlataNum, 
						 byte ChannelNum, byte Bank, byte Flag)
{
	word dat, Ask;
	longw Addr = GetBaseAddrFADC(CreiteNum,PlataNum)+0x10000;


#ifdef FADC_DEBUG
	printf("GetStatusRegister\n");
#endif

	Ask = ReadWordExt ( Addr);

	if ( Bank==0 ) dat = (word) (Ask >> (8*ChannelNum));

	if ( Bank==1 ) dat = (word) ( (Ask >> 6) >> (8*ChannelNum));
	
	if (Flag != 0)
	{
		printf( "Status register: \n" );
		print_in_bits ( Ask );
	}
	return dat;
}
//===========================================================================
void BlockRead_ADC(byte CreiteNum, byte PlataNum,
				   byte ChannelNum,
				   longw StartAddr, word WordsForRead,

				   int *DataMas)
{
	word j=0L;
	longw Addr=0L;

#ifdef FADC_DEBUG
	printf("BlockRead_ADC\n");
	printf("Addr = %lX  CreiteNum=%i,  PlataNum=%i,  ChannelNum=%i\n ",
		   Addr,CreiteNum,PlataNum,ChannelNum);
#endif

	if ( ChannelNum == 0 ) {
		Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + StartAddr;
	}


	if ( ChannelNum == 1 ) {
		Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + 0x4000 + StartAddr;
	}
	printf("Addr = %lX  WordsForRead=%ld  j=%ld\n ", Addr, (long)WordsForRead, (long)j);
	if (WordsForRead == 0) return;
	while ( j < WordsForRead )
	{
		DataMas[j] = ReadWordExt ( Addr );
		if (j > 0) {
			if ((word)DataMas[j] == 0xffff) {
				DataMas[j] = DataMas[j-1];
			}
		}
		DataMas[j] &= 4095;
		j++;
		Addr+=2;
	}
}
//===========================================================================
void GetPulseForm ( byte CreiteNum, byte PlataNum, byte Bank, int *DataMas )
{
	word j;
#ifdef FADC_DEBUG
	printf("GetPulseForm\n");
#endif

	if ( Bank==0 ) {
		BlockRead_ADC(CreiteNum,PlataNum, 0, 0x0000, 1024, DataMas);
		BlockRead_ADC(CreiteNum,PlataNum, 0, 0x0800, 1024, DataMas+1024);

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
}
//===========================================================================
void GetPulseFormDim ( byte CreiteNum, byte PlataNum, byte Bank, short int *DataMas )
{
	word j;
	longw Addr=0L;
#ifdef FADC_DEBUG
	printf("GetPulseFormDim\n");
#endif
	if ( Bank==0 ) {
		Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + 0x0000;
#ifdef FADC_DEBUG
		printf("Addr = %lX\n ", Addr);
#endif
		ReadDimExt12 ( Addr, 512, DataMas );
		ReadDimExt12 ( Addr+1024, 512, DataMas+512 );
		Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + 0x0800;
#ifdef FADC_DEBUG
		printf("Addr = %lX\n ", Addr);
#endif
		ReadDimExt12 ( Addr, 512, DataMas+1024 );
		ReadDimExt12 ( Addr+1024, 512, DataMas+(1024+512) );
		Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + 0x4000 + 0x0000;
#ifdef FADC_DEBUG
		printf("Addr = %lX\n ", Addr);
#endif
		ReadDimExt12 ( Addr, 512, DataMas+2048 );
		ReadDimExt12 ( Addr+1024, 512, DataMas+(2048+512) );
		Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + 0x4000 + 0x0800;
#ifdef FADC_DEBUG
		printf("Addr = %lX\n ", Addr);
#endif
		ReadDimExt12 ( Addr, 512, DataMas+3072 );
		ReadDimExt12 ( Addr+1024, 512, DataMas+(3072+512) );
	}
	if ( Bank==1 ) {

		Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + 0x1000;
#ifdef FADC_DEBUG
		printf("Addr = %lX\n ", Addr);
#endif
		ReadDimExt12 ( Addr, 512, DataMas );
		ReadDimExt12 ( Addr+1024, 512, DataMas+512 );
		Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + 0x1800;
#ifdef FADC_DEBUG
		printf("Addr = %lX\n ", Addr);
#endif
		ReadDimExt12 ( Addr, 512, DataMas+1024 );
		ReadDimExt12 ( Addr+1024, 512, DataMas+(1024+512) );
		Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + 0x4000 + 0x1000;
#ifdef FADC_DEBUG
		printf("Addr = %lX\n ", Addr);
#endif
		ReadDimExt12 ( Addr, 512, DataMas+2048 );
		ReadDimExt12 ( Addr+1024, 512, DataMas+(2048+512) );
		Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + 0x4000 + 0x1800;
#ifdef FADC_DEBUG
		printf("Addr = %lX\n ", Addr);
#endif
		ReadDimExt12 ( Addr, 512, DataMas+3072 );
		ReadDimExt12 ( Addr+1024, 512, DataMas+(3072+512) );
	}

	for (j=0; j<Aperture*4; j++) {
		DataMas[j] = 2048 - DataMas[j];
	}
}
//===========================================================================
void GetHistogram ( byte CreiteNum, byte PlataNum, int *DataMas )
{
#ifdef FADC_DEBUG
	printf("GetHistogram\n");
#endif
	BlockRead_ADC(CreiteNum,PlataNum, 0, 0x2000, 1024, DataMas);
	BlockRead_ADC(CreiteNum,PlataNum, 1, 0x2000, 1024, DataMas+1024);
}
//===========================================================================
void GetHistogramDim ( byte CreiteNum, byte PlataNum, int *DataMas )
{
	longw Addr=0L;

#ifdef FADC_DEBUG
	printf("GetHistogram\n");
#endif
	Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + 0x2000;
#ifdef FADC_DEBUG
	printf("Addr = %lX\n ", Addr);
#endif
	ReadDimExt ( Addr, 512, DataMas );
	ReadDimExt ( Addr+1024, 512, DataMas+512 );

	Addr = GetBaseAddrFADC(CreiteNum,PlataNum) + 0x4000 + 0x2000;
#ifdef FADC_DEBUG
	printf("Addr = %lX\n ", Addr);
#endif
	ReadDimExt ( Addr, 512, DataMas+1024 );
	ReadDimExt ( Addr+1024, 512, DataMas+(1024+512) );
}
