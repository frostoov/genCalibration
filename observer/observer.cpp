#include "observer.h"

Observer::Observer(){}

Subject::Subject(){}

Observer::~Observer(){}

Subject::~Subject(){}

void Subject::attach(Observer *obs,int interest)
{
	_observers.push_back(obsCell(obs,interest));
}

void Subject::detach(Observer *obs,int interest)
{
	_observers.remove(obsCell(obs,interest));
}

void Subject::notify(int interest)
{
	for(auto &obs : _observers)
		if(obs.interest() == interest)
			obs.item()->obsUpdate(this);
}
