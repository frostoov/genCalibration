#ifndef TEKOUTPUT_H
#define TEKOUTPUT_H

#include <QPlainTextEdit>
#include <QDateTime>
#include <QString>
#include <fstream>
#include "mutex"

#include "observer.h"
#include "tekmodule.h"
#include "tektypes.h"
#include "visa.h"

using namespace std;

class tekOutput : public QPlainTextEdit,public Observer
{
	Q_OBJECT
public:
	explicit tekOutput(tekModule *sub, QWidget *parent = 0);
	~tekOutput();
	void printAction		(const actionInfo_s &act);
	void printInfo			(const actionInfo_s &tdcAction,int16_t data);
	void obsUpdate			(const Subject *subject);
protected:
	void		print			(const QString& text);
	void		printDate();
	QString		getDate			(const QDateTime &dt);
	QString		getTime			(const QDateTime &tm);
	QString		getDataTime		(const QDateTime &tmdt);
	const char*	decodeAction	(tekActions op) const;
private:
	mutex		outputLock;
	tekModule	*_module;
	ofstream	logStream;
};

#endif // TEKOUTPUT_H
