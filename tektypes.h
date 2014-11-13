#ifndef TEKTYPES_H
#define TEKTYPES_H

#include "visatype.h"

enum class tekActions
{
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
	actionInfo_s(tekActions act,ViStatus err)
		: actCode(act),errCode(err){}
	tekActions	actCode;
	ViStatus	errCode;
};

#endif // TEKTYPES_H
