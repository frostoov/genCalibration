#ifndef TEKOUTPUT_H
#define TEKOUTPUT_H

#include <QPlainTextEdit>
#include <QDateTime>

#include "observer.h"
#include "tekmodule.h"

class tekOutput : public QPlainTextEdit, Observer
{
public:
	tekOutput(tekModule *mod,QWidget *parent = 0);
protected:
	QString		getDate(QDateTime &dt);
	QString		getTime(QDateTime &tm);
	void		printDate();
private:
	tekModule *_module;
};

#endif // TEKOUTPUT_H
