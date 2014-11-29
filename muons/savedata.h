#ifndef SAVEDATA_H
#define SAVEDATA_H

#include <vector>

#include "getsumpls.h"
#include "common.h"
#include "master.h"
#include "create_dir.h"
#include "flash_adc.h"

int OpenNewFile(unsigned long nnn, int por, int flag );
void SaveData ( byte Claster, byte Bank, int event, int por );
void SaveData (unsigned char Claster, unsigned char Bank, int event, int por);
int ReadAmplAnalizator ( byte Claster );
void SaveTime ( byte Claster, int event );
std::vector<int> SaveDataResult(byte Claster, byte Bank, int event, int por);


#endif // SAVEDATA_H
