#include "tekmodule.h"

using namespace std;

void tekModule::setChannel(const int &channel)
{
	_channel = channel;
}

bool tekModule::openSession()
{
	ViStatus status;
	ViUInt32 numInit;
	char resourceName[] = "USB?*INSTR{VI_ATTR_MANF_ID==0x0699}";
	ViChar desc[VI_FIND_BUFLEN];
	status = viOpenDefaultRM(&_mainSession);
	if (status < VI_SUCCESS)
		return false;
	status = viFindRsrc(_mainSession, resourceName , &_mainFList, &numInit, desc);
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

bool tekModule::closeSession()
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

string tekModule::whoYou()
{
	ViStatus status;
	ViUInt32 retCount = 0;
	ViChar buffer[256];
	status = viWrite(_thisSession, (unsigned char*)"*IDN?", 5, &retCount);
	status = viRead(_thisSession, (unsigned char*)buffer, 255, &retCount);
	buffer[retCount] = '\0';
	return string(buffer);
}

void tekModule::pushAction(tekActions action, ViStatus status)
{
	actions.push(actionInfo_s(action,status));
}

bool tekModule::setDefaultSettings(const int& channel)
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

bool tekModule::setPing(const int &mSec)
{
	ViUInt32 retCount = 0;
	stringstream data;
	data << "SOUR" << _channel << ":BURS:TDEL " << mSec << "ms";
	auto status = viWrite(_thisSession, (unsigned char*)data.str().c_str(), data.str().size(), &retCount);
	pushAction(tekActions::setPing , status);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool tekModule::setLeftFront(const int &leftFront)
{
	ViUInt32 retCount = 0;
	stringstream data;
	data << "SOUR" << _channel << ":PULS:TRAN:LEAD " << leftFront << "ns";
	auto status = viWrite(_thisSession, (unsigned char*)data.str().c_str(), data.str().size(), &retCount);
	pushAction(tekActions::setLeftFront , status);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool tekModule::setRightFront(const int &rightFront)
{
	ViStatus status;
	ViUInt32 retCount = 0;
	stringstream data;
	data << "SOUR" << _channel << ":PULS:TRAN:TRA " << rightFront << "ns";
	status = viWrite(_thisSession, (unsigned char*)data.str().c_str(), data.str().size(), &retCount);
	pushAction(tekActions::setRightFront , status);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool tekModule::setCountSignals(const int &count)
{
	ViUInt32 retCount = 0;
	stringstream data;
	data << "SOUR" << _channel << ":BURS:NCYC " << count;
	auto status = viWrite(_thisSession, (unsigned char*)data.str().c_str(), data.str().size(), &retCount);
	pushAction(tekActions::setCountSignals , status);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool tekModule::setFrequency(const int &freq)
{
	ViStatus status;
	ViUInt32 retCount = 0;
	stringstream data;
	data << "SOUR" << _channel << ":FREQ " << freq << "kHZ";
	status = viWrite(_thisSession, (unsigned char*)data.str().c_str(), data.str().size(), &retCount);
	pushAction(tekActions::setFrequency , status);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool tekModule::setAmplitude(const int &amp)
{
//SOUR1:PULS:TRAN:LEAD 200ns
	ViStatus status;
	ViUInt32 retCount = 0;
	stringstream data;
	data << "SOUR" << _channel << ":VOLT:LOW 0mV";
	status = viWrite(_thisSession, (unsigned char*)data.str().c_str(), data.str().size(), &retCount);
	pushAction(tekActions::setAmplitudeLow , status);
	if (status < VI_SUCCESS)
		return false;
	data.str().clear();
	data << "SOUR" << _channel << ":VOLT:HIGH " << amp << "mV";
	status = viWrite(_thisSession, (unsigned char*)data.str().c_str(), data.str().size(), &retCount);
	pushAction(tekActions::setAmplitudeHigh , status);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool tekModule::setInterval(const int &interval)
{
	ViStatus status;
	ViUInt32 retCount = 0;
	stringstream data;
	data << "TRIG:SEQ:TIM " << interval << "ms";
	status = viWrite(_thisSession, (unsigned char*)data.str().c_str(), data.str().size(), &retCount);
	pushAction(tekActions::setInterval , status);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool tekModule::activateChannel(const bool &statusChannel)
{
	ViStatus status;
	ViUInt32 retCount = 0;
	stringstream data;
	data << "OUTP" << _channel << ":STAT ";
	if (statusChannel == true)
		data << "ON";
	if (statusChannel == false)
		data << "OFF";
	status = viWrite(_thisSession, (unsigned char*)data.str().c_str(), data.str().size(), &retCount);
	pushAction(tekActions::activateChannel , status);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool tekModule::setWidth(const int &mSec)
{
	ViStatus status;
	ViUInt32 retCount = 0;
	stringstream data;
	data << "SOUR" << _channel << ":PULS:WIDT " << mSec << "ns";
	status = viWrite(_thisSession, (unsigned char*)data.str().c_str(), data.str().size(), &retCount);
	pushAction(tekActions::setWidth , status);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

string tekModule::getWidth() const
{
	ViStatus status;
	ViUInt32 retCount = 0;
	ViChar buffer[256];
	stringstream data;
	data << "SOUR" << _channel << ":PULS:WIDT?";
	status = viWrite(_thisSession, (unsigned char*)data.str().c_str(), data.str().size(), &retCount);
	status = viRead(_thisSession, (unsigned char*)buffer, 256, &retCount);
	buffer[retCount - 4] = 0;
	return string(buffer);
}

string tekModule::getAmplitude() const
{
	ViStatus status;
	ViUInt32 retCount = 0;
	ViChar buffer[256];
	stringstream data;
	data << "SOUR" << _channel << ":VOLT:HIGH?";
	status = viWrite(_thisSession, (unsigned char*)data.str().c_str(), data.str().size(), &retCount);
	status = viRead(_thisSession, (unsigned char*)buffer, 256, &retCount);
	buffer[retCount - 4] = 0;
	return string(buffer);
}

string tekModule::getLeftFront() const
{
	ViStatus status;
	ViUInt32 retCount = 0;
	ViChar buffer[256];
	stringstream data;
	data << "SOUR" << _channel << ":PULS:TRAN:LEAD?";
	status = viWrite(_thisSession, (unsigned char*)data.str().c_str(), data.str().size(), &retCount);
	status = viRead(_thisSession, (unsigned char*)buffer, 256, &retCount);
	buffer[retCount - 4] = 0;
	return string(buffer);
}

string tekModule::getRightFront() const
{
	ViStatus status;
	ViUInt32 retCount = 0;
	ViChar buffer[256];
	stringstream data;
	data << "SOUR" << _channel << ":PULS:TRAN:TRA?";
	status = viWrite(_thisSession, (unsigned char*)data.str().c_str(), data.str().size(), &retCount);
	status = viRead(_thisSession, (unsigned char*)buffer, 256, &retCount);
	buffer[retCount - 4] = 0;
	return string(buffer);
}

string tekModule::getInterval() const
{
	ViStatus status;
	ViUInt32 retCount = 0;
	ViChar buffer[256];
	stringstream data;
	data << "TRIG:SEQ:TIM?";
	status = viWrite(_thisSession, (unsigned char*)data.str().c_str(), data.str().size(), &retCount);
	status = viRead(_thisSession, (unsigned char*)buffer, 256, &retCount);
	buffer[retCount - 4] = 0;
	return string(buffer);
}



