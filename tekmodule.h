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
	typedef queue<actionInfo_s>	 actionQueue;

public:
	enum interest
	{statusUpdate};
	tekModule();
	bool		openSession	();
	bool		closeSession();
	bool		setPing				(int mSec);
	bool		setWidth			(int mSec);
	bool		setFrequency		(int freq);
	bool		setAmplitude		(int amp);
	bool		setCountSignals		(int count);
	bool		setLeftFront		(int leftFront);
	bool		setRightFront		(int rightFront);
	bool		setInterval			(int interval);
	void		setChannel			(int channel);
	bool		setDefaultSettings	(int channel);
	bool		activateChannel		(bool statusChannel);

	string		getWidth()			const;
	string		getAmplitude()		const;
	string		getLeftFront()		const;
	string		getRightFront()		const;
	string		getInterval()		const;
	string		whoYou();

	ViSession	resMN()		const	{return _resMN;}
	ViSession	session()	const	{return _session;}
	actionQueue &getActions()		{return actions;}

protected:
	void		pushAction(tekActions action,ViStatus status);
	ViStatus	write(const string &text) const;
private:

	bool		active;
	ViSession	_resMN;
	ViSession	_session;
	ViFindList	_mainFList;
	int			_channel;
	actionQueue	actions;
};

#endif // INTERLOCUTOR_H
