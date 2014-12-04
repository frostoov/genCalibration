#ifndef BLOCKS_H
#define BLOCKS_H

#include "defines.h"

exchangeBlock startMainBlock (unsigned long nnn, int *Threshold, int flag );
void setThresOfPart(int *Threshold);
void endMainBlock();
std::vector<int> sniffMainBlock(const exchangeBlock &inputVars);
std::vector<int> SaveDataResult(byte Claster, byte Bank, int event, int por);
std::vector<int> sniffMainBlockSignals(unsigned long nnn, int *Threshold, int flag , int countSignals);
std::vector<int> sniffMainBlockThree(unsigned long nnn, int flag, int countSignals);

#endif // BLOCKS_H
