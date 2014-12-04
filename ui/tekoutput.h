#ifndef TEKOUTPUT_H
#define TEKOUTPUT_H

#include <QPlainTextEdit>
#include <QDateTime>
#include <QString>
#include <fstream>
#include "mutex"

#include "observer/observer.h"
#include "tekVisa/tekmodule.h"
#include "types.hpp"

using namespace std;

class tekOutput : public QPlainTextEdit,public Observer
{
	Q_OBJECT
	typedef tekVisa::genModule tekModule;
	typedef tekVisa::actionInfo_s actionInfo_s;
public:
	explicit tekOutput(tekModule *sub, QWidget *parent = 0);
	~tekOutput();
	void		printAction		(const actionInfo_s &act);
	void		printInfo		(const actionInfo_s &tdcAction,int16_t data);
	void		obsUpdate		(const Subject *subject);
protected:
	void		print			(const QString& text);
	void		printDate();
	QString		getDate			(const QDateTime &dt);
	QString		getTime			(const QDateTime &tm);
	QString		getDataTime		(const QDateTime &tmdt);
private:
	mutex		outputLock;
	tekModule	*_module;
	ofstream	logStream;
};

#endif // TEKOUTPUT_H
