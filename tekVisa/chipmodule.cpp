#include "chipmodule.h"
#include <iostream>

chipModule::chipModule(genModule *tekMod)
{
	_module = tekMod;
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

void chipModule::tekModuleActivateChannel(int channel, bool status)
{
	_module->setChannel(channel);
	_module->activateChannel(status);
}

void chipModule::tekModuleSetHighLevel(int channel, int level)
{
	_module->setChannel(channel);
	_module->setHighLevel(level);
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
		SetThresholdWithBaseLine ( Claster, _thresh);
	}
	printf("\n");
}

vector<int> chipModule::sendOneSignal(int amp)
{
	tekModuleActivateChannel(0, true);
	_module->setLowLevel(0);
	tekModuleSetHighLevel(0, amp);
	tekModuleActivateChannel(1, true);
	_module->setLowLevel(0);
	tekModuleSetHighLevel(1, amp);
	vector<int> max = sniffMainBlockThree(nnn, 1);
	tekModuleActivateChannel(0, false);
	tekModuleActivateChannel(1, false);
	return max;
}

void chipModule::sendLongSignal(int ampStart, int ampEnd, int ampStep)
{
	_calibr.clear();
	tekModuleActivateChannel(0, true);
	_module->setLowLevel(0);
	tekModuleSetHighLevel(0, ampStart);
	tekModuleActivateChannel(1, true);
	_module->setLowLevel(0);
	tekModuleSetHighLevel(1, ampStart);
	for (int ampGo = ampStart + ampStep; ampGo <= ampEnd; ampGo += ampStep)
	{
		if (stopFlag == true)
			break;
		tekModuleSetHighLevel(0, ampGo);
		tekModuleSetHighLevel(1, ampGo);
		vector<int> max = sniffMainBlockThree(nnn, 1);
		cout << max[0] << "  " << max[1] << "  " << max[2] << "  " << max[3] << endl;
		if (howChipChannel == 0)
			_calibr[ampGo] = array<int, 2>({max[0], max[1]});
		else
			_calibr[ampGo] = array<int, 2>({max[2], max[3]});
		cout << "ampGo: " << ampGo << endl;
	}
	tekModuleActivateChannel(0, false);
	tekModuleActivateChannel(1, false);
}

void chipModule::sendLongSignalNotify(int ampStart, int ampEnd, int ampStep)
{
	ampCalibrMutex.lock();
	_calibr.clear();
	tekModuleActivateChannel(0, true);
	_module->setLowLevel(0);
	tekModuleSetHighLevel(0, ampStart);
	tekModuleActivateChannel(1, true);
	_module->setLowLevel(0);
	tekModuleSetHighLevel(1, ampStart);
	for (int ampGo = ampStart + ampStep; ampGo <= ampEnd; ampGo += ampStep)
	{
		if (stopFlag == true)
			break;
		_module->setHighLevel(ampGo);
		vector<int> max;
		for (int i = 0; i < 2; i++)
			max = sniffMainBlockThree(nnn, 1);
		cout << max[0] << "  " << max[1] << "  " << max[2] << "  " << max[3] << endl;
		if (howChipChannel == 0)
			_calibr[ampGo] = array<int, 2>({max[0], max[1]});
		else
			_calibr[ampGo] = array<int, 2>({max[2], max[3]});
		this->notify(chipModule::statusUpdate);
		cout << "ampGo: " << ampGo << endl;
	}
	tekModuleActivateChannel(0, false);
	tekModuleActivateChannel(1, false);
	ampCalibrMutex.unlock();
}

int chipModule::searchAndSetChipChannel()
{
	vector<int>	response = sendOneSignal(5000);
	if (response[0] > 50 && response[1] > 50)
	{
		howChipChannel = 0;
		return 0;
	}
	if (response[2] > 50 && response[3] > 50)
	{
		howChipChannel = 0;
		return 1;
	}
	return -1;
}

void chipModule::saveToFileAmp(const string& pathToFile, int chipChannel)
{
	ofstream saveFile(string(pathToFile) += string("amp") += to_string(chipChannel));
	for (auto it : _calibr)
		saveFile << it.first << "\t" << it.second[0] << "\t" << it.second[1] << endl;
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
	activeMode = chipModule::mode::amp;
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
	tekModuleActivateChannel(0, true);
	_module->setLowLevel(0);
	tekModuleSetHighLevel(0, 100);
	tekModuleActivateChannel(1, true);
	_module->setLowLevel(0);
	tekModuleSetHighLevel(1, 100);
	int sizeCalibr = 0;
	for (int ampGo = 100; ampGo <= 10000; ampGo += 100)
	{
		if (stopFlag == true)
			break;
//		if (sizeCalibr > 3)
//			if (_calibr[sizeCalibr][0] > 0 && _calibr[sizeCalibr - 1][0] > 0 && _calibr[sizeCalibr - 2][0] > 0)
//				return _calibr[sizeCalibr - 2];
		//TODO
		tekModuleSetHighLevel(0, ampGo);
		tekModuleSetHighLevel(1, ampGo);
		vector<int> max = sniffMainBlockThree(nnn, 1);
		cout << max[0] << "  " << max[1] << "  " << max[2] << "  " << max[3] << endl;
		if (howChipChannel == 0)
			_calibr[ampGo] = array<int, 2>({max[0], max[1]});
		else
			_calibr[ampGo] = array<int, 2>({max[2], max[3]});
		sizeCalibr++;
		cout << "ampGo: " << ampGo << endl;
	}
	tekModuleActivateChannel(0, false);
	tekModuleActivateChannel(1, false);
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

void chipModule::saveToFileThresh(const string& pathToFile, int chipChannel)
{
	ofstream saveFile(string(pathToFile) += string("thresh") += to_string(chipChannel));
	for (auto it : _threshList)
		saveFile << it.first << "\t" << it.second << endl;
//	for (int i = 0; i < (signed)_threshList.size(); i++)
//		saveFile << _threshList[i].first << "\t" << _threshList[i].second << endl;
	saveFile.close();
}

chipModule::mode chipModule::returnMode()	const
{
	return activeMode;
}

const map<int, array<int, 2>>::const_reverse_iterator& chipModule::returnLastAmp()	const
{
	return _calibr.rbegin();
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

pair<vector<int>, vector<int>> chipModule::searchAiming(int chipChannel, int amp)
{
	_calibr.clear();
	int chipV4channel;
	pair<vector<int>, vector<int>>	deviation;
	if (chipChannel == 0)
		chipV4channel = 0;
	if (chipChannel == 1)
		chipV4channel = 2;
	tekModuleActivateChannel(0, true);
	_module->setLowLevel(0);
	tekModuleSetHighLevel(0, amp);
	for (int i = 0; i < 100; i++)
	{
		vector<int> max = sniffMainBlockThree(nnn, 1);
		for (int j = 0; j < 4; j++)
			if (deviation.first[j] < max[j])
				deviation.first[j] = max[j];
	}
	tekModuleActivateChannel(0, false);
	deviation.first[chipV4channel] = -1000;


	if (chipChannel == 0)
		chipV4channel = 1;
	if (chipChannel == 1)
		chipV4channel = 3;
	tekModuleActivateChannel(1, true);
	_module->setLowLevel(0);
	tekModuleSetHighLevel(1, amp);
	for (int i = 0; i < 100; i++)
	{
		vector<int> max = sniffMainBlockThree(nnn, 1);
		for (int j = 0; j < 4; j++)
			if (deviation.second[j] < max[j])
				deviation.second[j] = max[j];
	}
	tekModuleActivateChannel(1, false);
	deviation.second[chipV4channel] = -1000;
	return deviation;
}


