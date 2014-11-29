#ifndef TEKTYPES_H
#define TEKTYPES_H

#include <string>
#include <vector>
#include "visa/visatype.h"

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

#endif // TEKTYPES_H
