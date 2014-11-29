#ifndef CHIPMODULE_H
#define CHIPMODULE_H

#include <vector>
#include "observer/observer.h"
#include "tekmodule.h"
#include "muons/muons_2012.h"

using namespace std;
using namespace tekVisa;

class chipModule : public Subject	//Subject пока для вида
{
public:
	enum interest
	{statusUpdate};
	explicit chipModule(genModule *tekMod);

	bool	openSession();
	void	closeSession();
	void	setThresh(int numberChipChannel, int codeThresh);
	void	mountThresh();
	int		sendOneSignal(int amp);
	void	sendLongSignal(int ampStart, int ampEnd, int ampStep);

	void	setChipChannel(const vector<char>& chipChoose);
	int		returnChipChannel()	const;
protected:

private:
	genModule			*_module;
	vector<char>		_chipChannel;
	exchangeBlock		_exchange;
	int					_thresh[NCLAST * NDET];
	vector<pair<int, int>>	_calibr;
};

#endif // CHIPMODULE_H
