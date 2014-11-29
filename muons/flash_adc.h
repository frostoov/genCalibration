//===========================================================================
#ifndef __FLASH_ADC_H
#define __FLASH_ADC_H

#include "defines.h"
#include "vme-lib.h"
#include "bitslib.h"
#include <stdio.h>

//===========================================================================
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
//===========================================================================
#endif
