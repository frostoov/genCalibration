#ifndef MAINBLCK_H
#define MAINBLCK_H

#include "defines.h"
#include "flash_adc.h"
#include "master.h"
#include "thresholds.h"
#include "savedata.h"
#include "stoprun.h"

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
} exchangeBlock;

int MainBlock (unsigned long nnn, int *Threshold, int flag );
exchangeBlock startMainBlock (unsigned long nnn, int *Threshold, int flag );
void setThresOfPart(int *Threshold);
void endMainBlock();
std::vector<int> sniffMainBlock(const exchangeBlock &inputVars);

#endif // MAINBLCK_H
