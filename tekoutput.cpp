#include "tekoutput.h"

tekOutput::tekOutput(tekModule *mod, QWidget *parent)
	: QPlainTextEdit(parent) , _module(mod)
{
}

QString tekOutput::getDate(QDateTime &dt)
{
	return	tr("%1.%2.%3").
			arg(dt.date().day(),2,10,QChar('0')).
			arg(dt.date().month(),2,10,QChar('0')).
			arg(dt.date().year(),4,10,QChar('0'));
}

QString tekOutput::getTime(QDateTime &tm)
{
	return	tr("%1:%2:%3").
			arg(tm.time().hour(),2,10,QChar('0')).
			arg(tm.time().minute(),2,10,QChar('0')).
			arg(tm.time().second(),2,10,QChar('0'));
}

void tekOutput::printDate()
{
	QDateTime tmdt = QDateTime::currentDateTime();
	appendPlainText(tr("Date:") + getDate(tmdt) + tr("\n") +
					tr("Time: ")+ getTime(tmdt) + tr("\n"));
	ensureCursorVisible();
}
