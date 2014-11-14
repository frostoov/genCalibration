#ifndef TEKTYPES_H
#define TEKTYPES_H

#include <string>
#include "visatype.h"

using namespace std;

struct tekSettings_t
{
	int leftFront;
	int rightFront;
	int interval;
	int width;
	int amplitude;
	int ampEnd;
	int ampStep;
	int ping;
	int countSignals;
	int frequency;
	int activeChannels;
};

enum class tekActions
{
	init,
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
	activateChannel
};

struct actionInfo_s
{
	actionInfo_s(){}
	actionInfo_s(tekActions act,ViStatus err)
		: actCode(act),errCode(err){}
	tekActions	actCode;
	ViStatus	errCode;
};

#endif // TEKTYPES_H
