#ifndef CHIPMODULE_H
#define CHIPMODULE_H

#include <vector>
#include <fstream>
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
	void	setThresh(int numberChipChannel, int codeThresh);
	void	mountThresh();
	int		sendOneSignal(int amp);
	void	sendLongSignal(int ampStart, int ampEnd, int ampStep);
	void	saveToFileAmp(const string& pathToFile);
	void	saveToFileThresh(const string& pathToFile);
	void	setSettings(int channel, int ampStart, int ampEnd, int ampStep);
	void	ampCalibration(int step);
	int		searchThresh();
	void	threshCalibration(int step);

	void	setChipChannel(const vector<char>& chipChoose);
	int		returnChipChannel()	const;
	mode	returnMode()	const;
	pair<int, int>	returnLastAmp()	const;
	pair<int, int>	returnLastThresh()	const;

protected:

private:
	genModule			*_module;
	vector<char>		_chipChannel;
	int					_thresh[NCLAST * NDET];
	vector<pair<int, int>>	_calibr;
	vector<pair<int, int>>	_threshList;
	calibrSettings_t	settings[2];
	mode				activeMode;
};

#endif // CHIPMODULE_H
