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

	struct	factorTransformation
	{
		double	factor;
		double	error;
		double	maxDeviation;
	};
	factorTransformation	getFactorTransformation(const vector<int>& ampX, const vector<int>& codeY);
	double	getFactorIntegral(double maxDeviation);

	void	computeHomingValues(int chipChannelV4, const vector<int>& minDeviation, const vector<int>& maxDeviation);
	void	readDataFromFiles();
	void	writeDataToFiles();
	void	readHomingFromFiles();
	void	writeHomingToFiles();
	void	computeForOneRecordAmp();
	void	computeForOneRecordForm();
	void	readOneRecordAmp(const string& pathToFile);
	void	readOneRecordForm(const string& pathToFile);
	void	setPathToFile(const string& pathToSaveInput);
	void	setChipChannel(int chipGroupChannel);
	void	helpVision();
	const array< array<int, 4>, 4>&	returnHoming()	const;
	const dataChip&					returnData	()	const;

private:
	dataChip	data;
	array< array<int, 4>, 4>	homing;
	vector<int>	vecX;
	vector<int>	vecFirst;
	vector<int>	vecSecond;
	string		pathToSave;
	int			howChipChannel;
};

#endif // PROCESSING_H
