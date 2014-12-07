#ifndef PROCESSING_H
#define PROCESSING_H

#include <fstream>
#include <iostream>
#include <cmath>
#include "tekVisa/tektypes.h"

class processing
{
public:
	explicit processing();
	~processing();

	pair<double, double>	getFactorTransformation(const vector<int>& ampX, const vector<int>& codeY);

	void	setHomingValues(int chipChannel, const vector<int>& minDeviation, const vector<int>& maxDeviation);
	void	readDataFromFiles();
	void	writeDataToFiles();
	void	readHomingFromFiles();
	void	writeHomingToFiles();
	void	setPathToFile(const string& pathToSaveInput);
	void	helpVision();
	const array< array<int, 4>, 4>&	returnHoming()	const;
	const dataChip&					returnData	()	const;

private:
	dataChip	data;
	array< array<int, 4>, 4>	homing;
	vector<int>	vecX;
	vector<int>	vecY;
	string		pathToSave;
};

#endif // PROCESSING_H
