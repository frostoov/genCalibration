#ifndef CHIPMODULE_H
#define CHIPMODULE_H

#include <vector>
#include <map>
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
	{amp, form};
	explicit chipModule(genModule *tekMod);

	bool	openSession();
	void	closeSession();
	void	calibration(int stepAmp, int stepThresh);
	void	stopCalibration();

	mode	returnMode()	const;
	const map<int, array<int, 2>>::const_reverse_iterator& returnLastAmp()	const;
	pair<int, int>	returnLastThresh()	const;
	pair<vector<int>, vector<int> > searchAiming(int chipChannel, int amp);
	int		searchAndSetChipChannel();
	void	saveToFileAmp(const string& pathToFile, int chipChannel);
	void	saveToFileThresh(const string& pathToFile, int chipChannel);

private:
	void	setThresh(int numberChipChannel, int codeThresh);
	void	mountThresh();
	vector<int> sendOneSignal(int amp);
	void	sendLongSignal(int ampStart, int ampEnd, int ampStep);
	void	sendLongSignalNotify(int ampStart, int ampEnd, int ampStep);
	void	setSettings(int channel, int ampStart, int ampEnd, int ampStep);
	void	ampCalibration(int stepAmp, int stepThresh);
	int		searchThresh();
	void	threshCalibration(int step);
	void	goThreshold(int step);
	void	tekModuleActivateChannel(int channel, bool status);
	void	tekModuleSetHighLevel(int channel, int level);

	genModule			*_module;
	int					howChipChannel;
	int					_thresh[NCLAST * NDET];
	map<int, array<int, 2>>	_calibr;
	vector<pair<int, int>>	_threshList;
	calibrSettings_t	settings[2];
	mode				activeMode;
	bool				stopFlag;
	mutex				ampCalibrMutex;
};

#endif // CHIPMODULE_H
