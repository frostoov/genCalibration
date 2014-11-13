#ifndef INTERLOCUTOR_H
#define INTERLOCUTOR_H

#include <string>
#include <queue>
#include <sstream>
#include "visa.h"
#include "visatype.h"
#include "observer.h"
#include "tektypes.h"

using namespace std;

class tekModule : public Subject
{
public:
	tekModule();
	bool		openSession	();
	bool		closeSession();
	bool		setPing				(const int& mSec);
	bool		setWidth			(const int& mSec);
	bool		setFrequency		(const int& freq);
	bool		setAmplitude		(const int& amp);
	bool		setCountSignals		(const int& count);
	bool		setLeftFront		(const int& leftFront);
	bool		setRightFront		(const int& rightFront);
	bool		setInterval			(const int& interval);
	void		setChannel			(const int& channel);
	bool		setDefaultSettings	(const int &channel);
	bool		activateChannel		(const bool& statusChannel);

	string		getWidth()			const;
	string		getAmplitude()		const;
	string		getLeftFront()		const;
	string		getRightFront()		const;
	string		getInterval()		const;
	string		whoYou();
protected:
	void pushAction(tekActions action,ViStatus status);
private:

	bool					active;
	ViSession				_mainSession;
	ViSession				_thisSession;
	ViFindList				_mainFList;
	int						_channel;
	queue<actionInfo_s>		actions;
};

#endif // INTERLOCUTOR_H
