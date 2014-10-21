#include "interlocutor.h"

using namespace std;

void ViInterlocutor::setChannel(const int &channel)
{
	_channel = channel;
}

bool ViInterlocutor::openSession()
{
	ViStatus status;
	ViUInt32 numInit;
	ViChar desc[VI_FIND_BUFLEN];
	status = viOpenDefaultRM(&_mainSession);
	if (status < VI_SUCCESS)
		return false;
	status = viFindRsrc(_mainSession, "USB?*INSTR{VI_ATTR_MANF_ID==0x0699}", &_mainFList, &numInit, desc);
	if (status < VI_SUCCESS)
		return false;
	status = viOpen(_mainSession, desc, VI_NULL, VI_NULL, &_thisSession);
	if (status < VI_SUCCESS)
		return false;
	status = viSetAttribute(_thisSession, VI_ATTR_TMO_VALUE, 1000);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool ViInterlocutor::closeSession()
{
	ViStatus status;
	status = viClose(_thisSession);
	if (status < VI_SUCCESS)
		return false;
	status = viClose(_mainSession);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool ViInterlocutor::setPing(const int &mSec)
{
	//	"SOURce1:BURSt:TDELay 20ms"
	ViStatus status;
	ViUInt32 retCount = 0;
	string compile("SOUR");
	compile.append(to_string(_channel));
	compile.append(":BURS:TDEL ");
	compile.append(to_string(mSec));
	compile.append("ms");
	status = viWrite(_thisSession, (unsigned char*)compile.c_str(), compile.size(), &retCount);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool ViInterlocutor::setLeftFront(const int &leftFront)
{
	//SOUR1:PULS:TRAN:LEAD 200ns
	ViStatus status;
	ViUInt32 retCount = 0;
	string compile("SOUR");
	compile.append(to_string(_channel));
	compile.append(":PULS:TRAN:LEAD ");
	compile.append(to_string(leftFront));
	compile.append("ns");
	status = viWrite(_thisSession, (unsigned char*)compile.c_str(), compile.size(), &retCount);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool ViInterlocutor::setCountSignals(const int &count)
{
	//	"SOURce1:BURSt:NCYCles 2"
	ViStatus status;
	ViUInt32 retCount = 0;
	string compile("SOUR");
	compile.append(to_string(_channel));
	compile.append(":BURS:NCYC ");
	compile.append(to_string(count));
	status = viWrite(_thisSession, (unsigned char*)compile.c_str(), compile.size(), &retCount);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool ViInterlocutor::setFrequency(const int &freq)
{
	ViStatus status;
	ViUInt32 retCount = 0;
	string compile("SOUR");
	compile.append(to_string(_channel));
	compile.append(":FREQ ");
	compile.append(to_string(freq));
	compile.append("kHZ");
	status = viWrite(_thisSession, (unsigned char*)compile.c_str(), compile.size(), &retCount);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool ViInterlocutor::setAmplitude(const int &amp)
{
	//SOUR1:PULS:TRAN:LEAD 200ns
	ViStatus status;
	ViUInt32 retCount = 0;
	string compile("SOUR");
	compile.append(to_string(_channel));
	compile.append(":VOLT:LOW 0mV");
	status = viWrite(_thisSession, (unsigned char*)compile.c_str(), compile.size(), &retCount);
	if (status < VI_SUCCESS)
		return false;
	compile.clear();
	compile.append("SOUR");
	compile.append(to_string(_channel));
	compile.append(":VOLT:HIGH ");
	compile.append(to_string(amp));
	compile.append("mV");
	status - viWrite(_thisSession, (unsigned char*)compile.c_str(), compile.size(), &retCount);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool ViInterlocutor::setRightFront(const int &rightFront)
{
	ViStatus status;
	ViUInt32 retCount = 0;
	string compile("SOUR");
	compile.append(to_string(_channel));
	compile.append(":PULS:TRAN:TRA ");
	compile.append(to_string(rightFront));
	compile.append("ns");
	status = viWrite(_thisSession, (unsigned char*)compile.c_str(), compile.size(), &retCount);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool ViInterlocutor::setInterval(const int &interval)
{
	ViStatus status;
	ViUInt32 retCount = 0;
	string compile("TRIG:SEQ:TIM ");
	compile.append(to_string(interval));
	compile.append("ms");
	status = viWrite(_thisSession, (unsigned char*)compile.c_str(), compile.size(), &retCount);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool ViInterlocutor::setDefaultSettings(const int& channel)
{
	ViStatus status;
	ViUInt32 retCount = 0;
	string fatherCompile, compile;
	fatherCompile.append("SOUR");
	fatherCompile.append(to_string(channel));
	compile = fatherCompile;
	compile.append(":BURS:STAT ON");
	status = viWrite(_thisSession, (unsigned char*)compile.c_str(), compile.size(), &retCount);
	if (status < VI_SUCCESS)
		return false;
	compile.clear();
	compile = fatherCompile;
	compile.append(":FUNC PULS");
	status = viWrite(_thisSession, (unsigned char*)compile.c_str(), compile.size(), &retCount);
	if (status < VI_SUCCESS)
		return false;
	compile.clear();
	compile = fatherCompile;
	compile.append(":BURS:MODE TRIG");
	status = viWrite(_thisSession, (unsigned char*)compile.c_str(), compile.size(), &retCount);
	if (status < VI_SUCCESS)
		return false;
	compile.clear();
	compile = fatherCompile;
	compile.append(":FREQ 1000kHz");
	status = viWrite(_thisSession, (unsigned char*)compile.c_str(), compile.size(), &retCount);
	if (status < VI_SUCCESS)
		return false;
	compile.clear();
	compile = fatherCompile;
	compile.append(":BURS:NCYC 1");
	status = viWrite(_thisSession, (unsigned char*)compile.c_str(), compile.size(), &retCount);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool ViInterlocutor::activateChannel(const bool &statusChannel)
{
	//OUTP1:STAT ON
	ViStatus status;
	ViUInt32 retCount = 0;
	string compile("OUTP");
	compile.append(to_string(_channel));
	compile.append(":STAT ");
	if (statusChannel == true)
		compile.append("ON");
	if (statusChannel == false)
		compile.append("OFF");
	status = viWrite(_thisSession, (unsigned char*)compile.c_str(), compile.size(), &retCount);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool ViInterlocutor::setWidth(const int &mSec)
{
	ViStatus status;
	ViUInt32 retCount = 0;
	string compile("SOUR");
	compile.append(to_string(_channel));
	compile.append(":PULS:WIDT ");
	compile.append(to_string(mSec));
	compile.append("ns");
	status = viWrite(_thisSession, (unsigned char*)compile.c_str(), compile.size(), &retCount);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

string ViInterlocutor::getWidth() const
{
	ViStatus status;
	ViUInt32 retCount = 0;
	ViChar buffer[256];
	string compile("SOUR");
	compile.append(to_string(_channel));
	compile.append(":PULS:WIDT?");
	status = viWrite(_thisSession, (unsigned char*)compile.c_str(), compile.size(), &retCount);
	status = viRead(_thisSession, (unsigned char*)buffer, 256, &retCount);
	buffer[retCount - 4] = 0;
	return string(buffer);
}

string ViInterlocutor::getAmplitude() const
{
	ViStatus status;
	ViUInt32 retCount = 0;
	ViChar buffer[256];
	string compile("SOUR");
	compile.append(to_string(_channel));
	compile.append(":VOLT:HIGH?");
	status = viWrite(_thisSession, (unsigned char*)compile.c_str(), compile.size(), &retCount);
	status = viRead(_thisSession, (unsigned char*)buffer, 256, &retCount);
	buffer[retCount - 4] = 0;
	return string(buffer);
}

string ViInterlocutor::getLeftFront() const
{
	ViStatus status;
	ViUInt32 retCount = 0;
	ViChar buffer[256];
	string compile("SOUR");
	compile.append(to_string(_channel));
	compile.append(":PULS:TRAN:LEAD?");
	status = viWrite(_thisSession, (unsigned char*)compile.c_str(), compile.size(), &retCount);
	status = viRead(_thisSession, (unsigned char*)buffer, 256, &retCount);
	buffer[retCount - 4] = 0;
	return string(buffer);
}

string ViInterlocutor::getRightFront() const
{
	ViStatus status;
	ViUInt32 retCount = 0;
	ViChar buffer[256];
	string compile("SOUR");
	compile.append(to_string(_channel));
	compile.append(":PULS:TRAN:TRA?");
	status = viWrite(_thisSession, (unsigned char*)compile.c_str(), compile.size(), &retCount);
	status = viRead(_thisSession, (unsigned char*)buffer, 256, &retCount);
	buffer[retCount - 4] = 0;
	return string(buffer);
}

string ViInterlocutor::getInterval() const
{
	ViStatus status;
	ViUInt32 retCount = 0;
	ViChar buffer[256];
	string compile;
	compile.append("TRIG:SEQ:TIM?");
	status = viWrite(_thisSession, (unsigned char*)compile.c_str(), compile.size(), &retCount);
	status = viRead(_thisSession, (unsigned char*)buffer, 256, &retCount);
	buffer[retCount - 4] = 0;
	return string(buffer);
}



