#ifndef READCONF_H
#define READCONF_H

#include "defines.h"

int  ReadConfiguration(byte Claster, int *TH, int *HV, int flag);
void ReadConfigFile(int *Thres, int *HV);
void SaveConfigFile(int *Thres, int *HV);

#endif // READCONF_H
