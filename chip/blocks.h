#ifndef BLOCKS_H
#define BLOCKS_H

#include "defines.h"

void setThresOfPart(int *Threshold);
void endMainBlock();
std::vector<int> SaveDataResult(byte Claster, byte Bank, int event);
std::vector<int> sniffMainBlockThree(unsigned long nnn, int countSignals);

#endif // BLOCKS_H
