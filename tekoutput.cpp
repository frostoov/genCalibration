#include "tekoutput.h"

tekOutput::tekOutput(tekModule *mod, QWidget *parent)
	: QPlainTextEdit(parent) , _module(mod)
{
	setReadOnly(true);
	logStream.open("log",ofstream::binary | ofstream::app);
	print("Starting tekControl");
	print(tr("Date: [") + getDate(QDateTime::currentDateTime()) + tr("]"));
	print(tr("Time: [") + getTime(QDateTime::currentDateTime()) + tr("]"));
}

tekOutput::~tekOutput()
{
	if(logStream.is_open())
		logStream.close();
}


void tekOutput::obsUpdate(const Subject *subject)
{
	if(subject == _module)
	{
		while( _module->getActions().size() )
		{
			printAction( _module->getActions().front() );
			_module->getActions().pop();
		}
	}
}

void tekOutput::print(const QString &text)
{
	outputLock.lock();
	appendPlainText(text);
	if(logStream.is_open())
		logStream << text.toUtf8().data() << endl;
	outputLock.unlock();
}


const char* tekOutput::decodeAction(tekActions op) const
{
	if(op == tekActions::init)
		return (const char*)"Init";
	if(op == tekActions::activateChannel)
		return (const char*)"Activate Channel";
	if(op == tekActions::setAmplitudeHigh)
		return (const char*)"Set Amplitude[High]";
	if(op == tekActions::setAmplitudeLow)
		return (const char*)"Set Amplitude[Low]";
	if(op == tekActions::setChannel)
		return (const char*)"Set Channel";
	if(op == tekActions::setCountSignals)
		return (const char*)"Set Count Signals";
	if(op == tekActions::setDefaultSettings)
		return (const char*)"Set Default Settings";
	if(op == tekActions::setFrequency)
		return (const char*)"Set Frequency";
	if(op == tekActions::setInterval)
		return (const char*)"Set Interval";
	if(op == tekActions::setLeftFront)
		return (const char*)"Set Left Front";
	if(op == tekActions::setPing)
		return (const char*)"Set Ping";
	if(op == tekActions::setRightFront)
		return (const char*)"Set Right Front";
	if(op == tekActions::setWidth)
		return (const char*)"Set Width";
	return (const char*)"";
}

void tekOutput::printAction(const actionInfo_s &act)
{
	if(act.errCode == VI_SUCCESS)
		print(getDataTime(QDateTime::currentDateTime()) + tr("\t") +
			  tr("%1 - OK").arg(decodeAction(act.actCode)) );
	else
		print(getDataTime(QDateTime::currentDateTime()) +tr("\t") +
			  tr("%1 - FAILED").arg(decodeAction(act.actCode)) );
	ensureCursorVisible();
}

QString tekOutput::getDate(const QDateTime &dt)
{
	return	tr("%1.%2.%3").
			arg(dt.date().day(),2,10,QChar('0')).
			arg(dt.date().month(),2,10,QChar('0')).
			arg(dt.date().year(),4,10,QChar('0'));
}

QString tekOutput::getTime(const QDateTime &tm)
{
	return	tr("%1:%2:%3").
			arg(tm.time().hour(),2,10,QChar('0')).
			arg(tm.time().minute(),2,10,QChar('0')).
			arg(tm.time().second(),2,10,QChar('0'));
}

QString tekOutput::getDataTime(const QDateTime &tmdt)
{
	return tr("[") + getTime(tmdt) + tr("]");
}
