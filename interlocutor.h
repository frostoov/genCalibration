#ifndef INTERLOCUTOR_H
#define INTERLOCUTOR_H

#include <string>
#include "visa.h"
#include "visatype.h"

using namespace std;

class ViInterlocutor
{
public:
	bool		openSession();
//	string&		whoYou();
	bool		closeSession();
	bool		setPing(const int& mSec);
	bool		setWidth(const int& mSec);
	bool		setFrequency(const int& freq);
	bool		setAmplitude(const int& amp);
	bool		setCountSignals(const int& count);
	bool		setLeftFront(const int& leftFront);
	bool		setRightFront(const int& rightFront);
	bool		setInterval(const int& interval);
	void		setChannel(const int& channel);
	bool		setDefaultSettings(const int &channel);
	bool		activateChannel(const bool& statusChannel);

	string		getWidth() const;
	string		getAmplitude() const;
	string		getLeftFront() const;
	string		getRightFront() const;
	string		getInterval() const;
private:
	ViSession	_mainSession, _thisSession;
	ViFindList	_mainFList;
	int			_channel;
};

#endif // INTERLOCUTOR_H
