#ifndef THRESHOLDS_H
#define THRESHOLDS_H

#include "defines.h"
#include "flash_adc.h"
#include "create_dir.h"

void GetBaseLine (byte Claster, byte PlataNum, int *BaseLine, int flag );
void SetThresholdWithBaseLine (byte Claster, int *Threshold, int flag );

#endif // THRESHOLDS_H
