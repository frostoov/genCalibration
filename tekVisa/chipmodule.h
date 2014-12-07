#ifndef CHIPMODULE_H
#define CHIPMODULE_H

#include <vector>
#include <fstream>
#include <thread>
#include <mutex>
#include <ctime>
#include "observer/observer.h"
#include "tekmodule.h"
#include "chip/blocks.h"

using namespace std;
using namespace tekVisa;

class chipModule : public Subject
{
public:
	enum interest
	{statusUpdate};
	enum mode
	{ampFirst, ampSecond, form};
	explicit chipModule(genModule *tekMod);

	bool	openSession();
	void	closeSession();
	void	calibration(int stepAmp, int stepThresh);
	void	stopCalibration();

	void	setChipChannel(const vector<char>& chipChoose);
	mode	returnMode()	const;
	pair<int, int>	returnLastAmp()	const;
	pair<int, int>	returnLastThresh()	const;
	vector<int>	searchAiming(int amp);
	int		returnChipChannel()	const;

private:
	void	setThresh(int numberChipChannel, int codeThresh);
	void	mountThresh();
	int		sendOneSignal(int amp);
	void	sendLongSignal(int ampStart, int ampEnd, int ampStep);
	void	sendLongSignalNotify(int ampStart, int ampEnd, int ampStep);
	void	saveToFileAmp(const string& pathToFile);
	void	saveToFileThresh(const string& pathToFile);
	void	setSettings(int channel, int ampStart, int ampEnd, int ampStep);
	void	ampCalibration(int stepAmp, int stepThresh);
	int		searchThresh();
	void	threshCalibration(int step);
	void	goThreshold(int step);

	genModule			*_module;
	vector<char>		_chipChannel;
	int					_thresh[NCLAST * NDET];
	vector<pair<int, int>>	_calibr;
	vector<pair<int, int>>	_threshList;
	calibrSettings_t	settings[2];
	mode				activeMode;
	bool				stopFlag;
	mutex				ampCalibrMutex;
};

#endif // CHIPMODULE_H
