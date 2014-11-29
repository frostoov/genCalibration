//===========================================================================
#ifndef __MASTER_H
#define __MASTER_H

#include "defines.h"
#include "vme-lib.h"

//===========================================================================
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
//====================================================================
#endif
