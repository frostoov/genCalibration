#ifndef OBSERVER_H
#define OBSERVER_H

#include <list>

using namespace std;

class Subject;
class Observer;

class Observer
{
public:
	virtual ~Observer();
	virtual void obsUpdate(const Subject *subject) = 0;
protected:
	Observer();
};

class Subject
{
	class obsCell
	{
	public:
		obsCell(Observer *val,int interest)
		{
			_item		= val;
			_interest	= interest;
		}
		bool operator ==(const obsCell &cell)
		{
			if(cell.interest()	== _interest &&
			   cell.item()		== _item )
				return true;
			else return false;
		}
		bool operator !=(const obsCell &cell)
		{
			if(cell.interest()	!= _interest ||
			   cell.item()		!= _item )
				return true;
			else return false;
		}
		Observer	*item()		const	{return _item;}
		int			interest()	const	{return _interest;}
	private:
		Observer	*_item;
		int			_interest;
	};
public:
	virtual ~Subject();
	virtual void attach(Observer *obs, int interset);
	virtual void detach(Observer *obs, int interset);
	virtual void notify(int interest);
protected:
	Subject();
private:
	list<obsCell> _observers;
};
#endif // OBSERVER_H
