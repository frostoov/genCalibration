#include "tekmodule.h"

using namespace std;

void tekModule::setChannel(int channel)
{
	_channel = channel;
}

tekModule::tekModule()
{
	active =false;
}

bool tekModule::openSession()
{
	ViStatus status;
	ViUInt32 numInit;
	const ViString resourceName = (const ViString)"USB?*INSTR{VI_ATTR_MANF_ID==0x0699}";
	ViChar desc[VI_FIND_BUFLEN];
	status = viOpenDefaultRM(&_resMN);
	if (status < VI_SUCCESS)
	{
		pushAction(tekActions::init,status);
		return false;
	}
	status = viFindRsrc(_resMN, resourceName , &_mainFList, &numInit, desc);
	if (status < VI_SUCCESS)
	{
		pushAction(tekActions::init,status);
		return false;
	}
	status = viOpen(_resMN, desc, VI_NULL, VI_NULL, &_session);
	if (status < VI_SUCCESS)
	{
		pushAction(tekActions::init,status);
		return false;
	}
//	status = viSetAttribute(_session, VI_ATTR_TMO_VALUE, 1000);
//	if (status < VI_SUCCESS)
//	{
//		pushAction(tekActions::init,status);
//		return false;
//	}
	active = true;
	pushAction(tekActions::init,VI_SUCCESS);
	return true;
}

bool tekModule::closeSession()
{
	ViStatus status;
	status = viClose(_session);
	if (status < VI_SUCCESS)
		return false;
	status = viClose(_resMN);
	if (status < VI_SUCCESS)
		return false;
	active = false;
	return true;
}

string tekModule::whoYou()
{
	ViStatus status;
	ViUInt32 retCount = 0;
	ViChar buffer[256];
	status = viWrite(_session, (unsigned char*)"*IDN?", 5, &retCount);
	status = viRead(_session, (unsigned char*)buffer, 255, &retCount);
	buffer[retCount] = '\0';
	return string(buffer);
}

void tekModule::pushAction(tekActions action, ViStatus status)
{
	actions.push(actionInfo_s(action,status));
}

bool tekModule::setDefaultSettings(int channel)
{
	if(!active) return false;
	ViStatus status;
	ViUInt32 retCount = 0;
	stringstream data;
	data << "SOUR" << channel << ":BURS:STAT ON";
	status = viWrite(_session, (unsigned char*)data.str().c_str(), data.str().size(), &retCount);
	if (status < VI_SUCCESS)
		return false;
	data.str().clear();
	data << "SOUR" << channel << ":FUNC PULS";
	status = viWrite(_session, (unsigned char*)data.str().c_str(), data.str().size(), &retCount);
	if (status < VI_SUCCESS)
		return false;
	data.str().clear();
	data << "SOUR" << channel << ":BURS:MODE TRIG";
	status = viWrite(_session, (unsigned char*)data.str().c_str(), data.str().size(), &retCount);
	if (status < VI_SUCCESS)
		return false;
	data.str().clear();
	data << "SOUR" << channel << ":FREQ 1000kHz";
	status = viWrite(_session, (unsigned char*)data.str().c_str(), data.str().size(), &retCount);
	if (status < VI_SUCCESS)
		return false;
	data.str().clear();
	data << "SOUR" << channel << ":BURS:NCYC 1";
	status = viWrite(_session, (unsigned char*)data.str().c_str(), data.str().size(), &retCount);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool tekModule::setPing(int mSec)
{
	if(!active) return false;
	stringstream data;
	data << "SOUR" << _channel << ":BURS:TDEL " << mSec << "ms";
	auto status = write(data.str());
	pushAction(tekActions::setPing , status);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool tekModule::setLeftFront(int leftFront)
{
	if(!active) return false;
	stringstream data;
	data << "SOUR" << _channel << ":PULS:TRAN:LEAD " << leftFront << "ns";
	auto status = write(data.str());
	pushAction(tekActions::setLeftFront , status);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool tekModule::setRightFront(int rightFront)
{
	if(!active) return false;
	stringstream data;
	data << "SOUR" << _channel << ":PULS:TRAN:TRA " << rightFront << "ns";
	auto status = write(data.str());
	pushAction(tekActions::setRightFront , status);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool tekModule::setCountSignals(int count)
{
	if(!active) return false;
	stringstream data;
	data << "SOUR" << _channel << ":BURS:NCYC " << count;
	auto status = write(data.str());
	pushAction(tekActions::setCountSignals , status);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool tekModule::setFrequency(int freq)
{
	if(!active) return false;
	stringstream data;
	data << "SOUR" << _channel << ":FREQ " << freq << "kHZ";
	auto status = write(data.str());
	pushAction(tekActions::setFrequency , status);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool tekModule::setAmplitude(int amp)
{
	if(!active) return false;
	stringstream data;
	data << "SOUR" << _channel << ":VOLT:LOW 0mV";
	auto status = write(data.str());
	pushAction(tekActions::setAmplitudeLow , status);
	if (status < VI_SUCCESS)
		return false;
	data.str().clear();
	data << "SOUR" << _channel << ":VOLT:HIGH " << amp << "mV";
	status = write(data.str());
	pushAction(tekActions::setAmplitudeHigh , status);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool tekModule::setInterval(int interval)
{
	if(!active) return false;
	stringstream data;
	data << "TRIG:SEQ:TIM " << interval << "ms";
	auto status = write(data.str());
	pushAction(tekActions::setInterval , status);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool tekModule::activateChannel(bool statusChannel)
{
	if(!active) return false;
	stringstream data;
	data << "OUTP" << _channel << ":STAT ";
	if (statusChannel == true)
		data << "ON";
	if (statusChannel == false)
		data << "OFF";
	auto status = write(data.str());
	pushAction(tekActions::activateChannel , status);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

bool tekModule::setWidth(int mSec)
{
	if(!active) return false;
	stringstream data;
	data << "SOUR" << _channel << ":PULS:WIDT " << mSec << "ns";
	auto status = write(data.str());
	pushAction(tekActions::setWidth , status);
	if (status < VI_SUCCESS)
		return false;
	return true;
}

string tekModule::getWidth() const
{
	if(!active) return string("0");
	ViUInt32 retCount = 0;
	ViChar buff[256];
	stringstream data;
	data << "SOUR" << _channel << ":PULS:WIDT?";
	auto status = write(data.str());
	status = viRead(_session, (ViPBuf)buff, 256, &retCount);
	buff[retCount - 4] = '\0';
	return string(buff);
}

string tekModule::getAmplitude() const
{
	if(!active) return string("0");
	ViUInt32 retCount = 0;
	ViChar buffer[256];
	stringstream data;
	data << "SOUR" << _channel << ":VOLT:HIGH?";
	auto status = write(data.str());
	status = viRead(_session, (unsigned char*)buffer, 256, &retCount);
	buffer[retCount - 4] = 0;
	return string(buffer);
}

string tekModule::getLeftFront() const
{
	if(!active) return string("0");
	ViUInt32 retCount = 0;
	ViChar buffer[256];
	stringstream data;
	data << "SOUR" << _channel << ":PULS:TRAN:LEAD?";
	auto status = write(data.str());
	status = viRead(_session, (unsigned char*)buffer, 256, &retCount);
	buffer[retCount - 4] = 0;
	return string(buffer);
}

string tekModule::getRightFront() const
{
	if(!active) return string("0");
	ViUInt32 retCount = 0;
	ViChar buffer[256];
	stringstream data;
	data << "SOUR" << _channel << ":PULS:TRAN:TRA?";
	auto status = write(data.str());
	status = viRead(_session, (unsigned char*)buffer, 256, &retCount);
	buffer[retCount - 4] = 0;
	return string(buffer);
}

string tekModule::getInterval() const
{
	if(!active) return string("0");
	ViUInt32 retCount = 0;
	ViChar buffer[256];
	stringstream data;
	data << "TRIG:SEQ:TIM?";
	auto status = write(data.str());
	status = viRead(_session, (unsigned char*)buffer, 256, &retCount);
	buffer[retCount - 4] = 0;
	return string(buffer);
}

ViStatus tekModule::write(const string &text) const
{
	ViUInt32 retCount = 0;
	auto status = viWrite(_session, (ViBuf)text.c_str(),text.size(),&retCount);
	if(retCount != text.size())
		return VI_ERROR_BERR;
	return status;
}
