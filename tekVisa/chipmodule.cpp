#include "chipmodule.h"
#include <iostream>

chipModule::chipModule(genModule *tekMod)
{
	_module = tekMod;
	_chipChannel.resize(4);
}

bool chipModule::openSession()
{
	Create_Dir_Files();
	ReadStopTime();
	ReadConfigFile(Thresh, HV);
	for (int i = 0; i < NCLAST * NDET; i++)
		_thresh[i] = Thresh[i];
	char host[] = "192.168.1.89";
	Connect(host, sizeof(host));
	sleep(2);
	return true;
}

void chipModule::closeSession()
{
	endMainBlock();
	UnConnect();
}

void chipModule::setThresh(int numberChipChannel, int codeThresh)
{
	_thresh[numberChipChannel + 12] = codeThresh;
}

void chipModule::mountThresh()
{
	byte Claster;
	for (int ii=0; ii<NCLASTREADY; ii++)
	{
		Claster = ClasterReady[ii];
		printf("Claster=%i\n",Claster+1);
		SetThresholdWithBaseLine ( Claster, _thresh, 0 );
	}
	printf("\n");
}

int chipModule::sendOneSignal(int amp)
{
	_module->setLowLevel(0);
	_module->setHighLevel(amp);
	_module->activateChannel(true);
	vector<int> max = sniffMainBlockSignals(nnn, _thresh, 1, 1);
	_module->activateChannel(false);
	int howChipChannel = returnChipChannel();
	return max[howChipChannel];
}

void chipModule::sendLongSignal(int ampStart, int ampEnd, int ampStep)
{
	_calibr.clear();
	int howChipChannel = returnChipChannel();
	_module->setLowLevel(0);
	_module->setHighLevel(ampStart);
	_module->activateChannel(true);
	for (int ampGo = ampStart + ampStep; ampGo <= ampEnd; ampGo += ampStep)
	{
		if (stopFlag == true)
			break;
		_module->setHighLevel(ampGo);
		vector<int> max = sniffMainBlockThree(nnn, 1, 1);
		cout << max[0] << "  " << max[1] << "  " << max[2] << "  " << max[3] << endl;
		_calibr.push_back(pair<int, int>(ampGo, max[0]));
		cout << "ampGo: " << ampGo << endl;
	}
	_module->activateChannel(false);
}

void chipModule::sendLongSignalNotify(int ampStart, int ampEnd, int ampStep)
{
	ampCalibrMutex.lock();
	_calibr.clear();
	int howChipChannel = returnChipChannel();
	_module->setLowLevel(0);
	_module->setHighLevel(ampStart);
	_module->activateChannel(true);
	for (int ampGo = ampStart + ampStep; ampGo <= ampEnd; ampGo += ampStep)
	{
		if (stopFlag == true)
			break;
		_module->setHighLevel(ampGo);
		vector<int> max;
		for (int i = 0; i < 2; i++)
			max = sniffMainBlockThree(nnn, 1, 1);
		cout << max[0] << "  " << max[1] << "  " << max[2] << "  " << max[3] << endl;
		_calibr.push_back(pair<int, int>(ampGo, max[0]));
		this->notify(chipModule::statusUpdate);
		cout << "ampGo: " << ampGo << endl;
	}
	_module->activateChannel(false);
	ampCalibrMutex.unlock();
}

void chipModule::setChipChannel(const vector<char> &chipChoose)
{
	for (int i = 0; i < (signed)_chipChannel.size(); i++)
		_chipChannel[i] = chipChoose[i];
}

int chipModule::returnChipChannel()	const
{
	int howChipChannelChoose = -1;
	if (_module->getChannelActive() == 1)
		for (int i = 0; i < 4; i++)
			if (_chipChannel[i] == '1')
			{
				howChipChannelChoose = i;
				break;
			}
	if (_module->getChannelActive() == 2)
		for (int i = 3; i >= 0; i--)
			if (_chipChannel[i] == '1')
			{
				howChipChannelChoose = i;
				break;
			}
	return howChipChannelChoose;
}

void chipModule::saveToFileAmp(const string& pathToFile)
{
	ofstream saveFile(pathToFile);
	for (int i = 0; i < (signed)_calibr.size(); i++)
		saveFile << _calibr[i].first << "\t" << _calibr[i].second << endl;
	saveFile.close();
}

void chipModule::setSettings(int channel, int ampStart, int ampEnd, int ampStep)
{
	settings[channel].ampStart	= ampStart;
	settings[channel].ampEnd	= ampEnd;
	settings[channel].ampStep	= ampStep;
}

void chipModule::ampCalibration(int stepAmp, int stepThresh)
{
	stopFlag = false;
	activeMode = chipModule::mode::ampFirst;
	setThresh(0, 10);
	setThresh(1, 10);
	setThresh(2, 10);
	setThresh(3, 10);
	mountThresh();
//	std::thread	calibr(&chipModule::sendLongSignalNotify, this, 100, 10000, step);
//	calibr.detach();
	sendLongSignalNotify(100, 10000, stepAmp);
	std::thread calibrThresh(&chipModule::threshCalibration, this, stepThresh);
	calibrThresh.detach();
}

int chipModule::searchThresh()
{
	_calibr.clear();
	int howChipChannel = returnChipChannel();
	_module->setLowLevel(0);
	_module->setHighLevel(100);
	_module->activateChannel(true);
	int sizeCalibr = 0;
	for (int ampGo = 100; ampGo <= 10000; ampGo += 100)
	{
		if (stopFlag == true)
			break;
		if (sizeCalibr > 3)
			if (_calibr[sizeCalibr].second > 0 && _calibr[sizeCalibr - 1].second > 0 && _calibr[sizeCalibr - 2].second > 0)
				return _calibr[sizeCalibr - 2].first;
		_module->setHighLevel(ampGo);
		vector<int> max = sniffMainBlockThree(nnn, 1, 1);
		cout << max[0] << "  " << max[1] << "  " << max[2] << "  " << max[3] << endl;
		_calibr.push_back(pair<int, int>(ampGo, max[0]));
		sizeCalibr++;
		cout << "ampGo: " << ampGo << endl;
	}
	_module->activateChannel(false);
	return -1;
}

void chipModule::goThreshold(int step)
{
	for (int threshGo = 10; threshGo < 140; threshGo += step)
	{
		if (stopFlag == true)
			break;
		setThresh(0, threshGo);
		setThresh(1, threshGo);
		setThresh(2, threshGo);
		setThresh(3, threshGo);
		mountThresh();
		_threshList.push_back(pair<int, int>(threshGo, searchThresh()));
		this->notify(chipModule::statusUpdate);
	}
}

void chipModule::threshCalibration(int step)
{
	stopFlag = false;
	activeMode = chipModule::mode::form;
	goThreshold(step);
//	std::thread calibr(&chipModule::goThreshold, this, step);
//	calibr.detach();
}

void chipModule::saveToFileThresh(const string& pathToFile)
{
	ofstream saveFile(pathToFile);
	for (int i = 0; i < (signed)_threshList.size(); i++)
		saveFile << _threshList[i].first << "\t" << _threshList[i].second << endl;
	saveFile.close();
}

chipModule::mode chipModule::returnMode()	const
{
	return activeMode;
}

pair<int, int>	chipModule::returnLastAmp()	const
{
	return _calibr.back();
}

pair<int, int>	chipModule::returnLastThresh()	const
{
	return _threshList.back();
}

void chipModule::stopCalibration()
{
	stopFlag = true;
}

void chipModule::calibration(int stepAmp, int stepThresh)
{
	std::thread calibrAmp(&chipModule::ampCalibration, this, stepAmp, stepThresh);
	calibrAmp.detach();
}

vector<int> chipModule::searchAiming(int amp)
{
	_calibr.clear();
	int howChipChannel = returnChipChannel();
	_module->setLowLevel(0);
	_module->setHighLevel(amp);
	_module->activateChannel(true);
	vector<int> deviation;
	for (int i = 0; i < 100; i++)
	{
		vector<int> max = sniffMainBlockThree(nnn, 1, 1);
		for (int j = 0; j < 4; j++)
			if (deviation[j] < max[j])
				deviation[j] = max[j];
	}
	_module->activateChannel(false);
	//TODO
	howChipChannel = 0;
	deviation[howChipChannel] = -1000;
	return deviation;
}


