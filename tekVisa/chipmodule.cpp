#include "chipmodule.h"

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
	_exchange = startMainBlock(nnn, _thresh, 1);
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
//	setInterval(1000);
	_module->activateChannel(true);
//	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	vector<int> max = sniffMainBlock(_exchange);
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
		vector<int> max = sniffMainBlock(_exchange);
		_calibr.push_back(pair<int, int>(ampGo, max[howChipChannel]));
	}
	_module->activateChannel(false);
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
