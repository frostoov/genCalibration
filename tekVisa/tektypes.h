#ifndef TEKTYPES_H
#define TEKTYPES_H

#include <string>
#include <vector>
#include <array>
#include "visa/visatype.h"

using namespace std;

namespace tekVisa {

class genModule;

struct genSettings_t
{
	int leftFront;
	int rightFront;
	int interval;
	int width;
	int highLevel;
	int lowLevel;
	int ping;
	int activeChannel;
	int countSignals;
	int frequency;
};

struct calibrSettings_t
{
	int ampStart;
	int ampEnd;
	int ampStep;
};

enum class genActions
{
	init,
	close,
	setWidth,
	setPing,
	setLeftFront,
	setCountSignals,
	setFrequency,
	setAmplitudeLow,
	setAmplitudeHigh,
	setRightFront,
	setInterval,
	setChannel,
	setDefaultSettings,
	activateChannel,
	getWidth,
	getAmplitudeHigh,
	getAmplitudeLow,
	getLeftFront,
	getRightFront,
	getInterval
};

struct actionInfo_s
{
	actionInfo_s(){}
	actionInfo_s(genActions act,ViStatus err)
		: actCode(act),errCode(err){}
	genActions	actCode;
	ViStatus	errCode;
};

}

struct dataChip
{
	array<int, 4>	transformationADC;
	array<int, 4>	errorADC;
	array<int, 4>	nonlinearity;
	array<int, 2>	transformationDAC;
	array<int, 2>	errorDAC;
};

#endif // TEKTYPES_H
