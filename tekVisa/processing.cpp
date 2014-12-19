#include "tekVisa/processing.h"

using namespace std;

processing::processing()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			homing[i][j] = 0;

	for (int i = 0; i < 4; i++)
	{
		data.transformationADC[i] = 0;
		data.errorADC[i] = 0;
		data.nonlinearity[i] = 0;
	}
}

processing::~processing()
{

}

void processing::setChipChannel(int chipGroupChannel)
{
	howChipChannel = chipGroupChannel;
}

processing::factorTransformation	processing::getFactorTransformation(const vector<int> &ampX, const vector<int> &codeY)
{
	if (ampX.size() != codeY.size())
		return factorTransformation({-1, -1, -1});

	int sizeInput	= ampX.size();

	double xGo	= 0;
	double yGo	= 0;
	double x2Go	= 0;
	double xyGo	= 0;

	for (int i = 0; i < sizeInput; i++)
	{
		xGo		+= ampX[i];
		yGo		+= codeY[i];
		x2Go	+= ampX[i] * ampX[i];
		xyGo	+= ampX[i] * codeY[i];
	}
	double	midX	= xGo / (double) sizeInput;
	double	midY	= yGo / (double) sizeInput;
	double	midXY	= xyGo / (double) sizeInput;
	double	midX2	= x2Go / (double) sizeInput;
	double	factor	= (midXY - midX * midY)	/ (midX2 - midX * midX);
	double	free	= midY - factor * midX;
	double	maxDeviation = 10.0;
	return factorTransformation({factor, free, maxDeviation});
//	return pair<double, double>(factor, free);
}

double	processing::getFactorIntegral(double maxDeviation)
{
	const double ampMax = 255.0;
	return maxDeviation / ampMax * 100.0;
}

void processing::setPathToFile(const string& pathToSaveInput)
{
	pathToSave = pathToSaveInput;

//	data = {};
//	homing = {};
}

void processing::writeDataToFiles()
{
	ofstream mainFile(string(pathToSave) += "data");
	for (int i = 0; i < 4; i++)
		mainFile << data.transformationADC[i] << "\t" << data.errorADC[i] << "\t"
				 << data.nonlinearity[i] << endl;
	mainFile << endl;
	for (int i = 0; i < 2; i++)
		mainFile << data.transformationDAC[i] << "\t" << data.errorDAC[i] << endl;
	mainFile.close();
}

void processing::readDataFromFiles()
{
	ifstream mainFile(string(pathToSave) += "data");
	for (int i = 0; i < 4; i++)
		mainFile >> data.transformationADC[i] >> data.errorADC[i]
				 >> data.nonlinearity[i];
	for (int i = 0; i < 2; i++)
		mainFile >> data.transformationDAC[i] >> data.errorDAC[i];
	mainFile.close();
}

void processing::writeHomingToFiles()
{
	ofstream mainFile(string(pathToSave) += "homing");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			mainFile << homing[i][j] << "\t";
		mainFile << endl;
	}
	mainFile.close();
}

void processing::readHomingFromFiles()
{
	ifstream mainFile(string(pathToSave) += "homing");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mainFile >> homing[i][j];
			cout << homing[i][j] << "  ";
		}
		cout << endl;
	}
	mainFile.close();
}

void processing::helpVision()
{
	for (int i = 0; i < 4; i++)
		cout << data.transformationADC[i] << "\t" << data.errorADC[i]
			 << "\t" << data.nonlinearity[i] << endl;
	cout << endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			cout << homing[i][j] << "\t";
		cout << endl;
	}
}

void processing::computeHomingValues(int chipChannelV4, const vector<int> &minDeviation, const vector<int> &maxDeviation)
{
	readHomingFromFiles();
	vector<double> influenceChannel(4);
	for (int i = 0; i < 4; i++)
		if (i != chipChannelV4)
			influenceChannel[i] = 20.0 * log10(maxDeviation[i] / minDeviation[i]);
	influenceChannel[chipChannelV4] = -1000;
	for (int i = 0; i < 4; i++)
			homing[chipChannelV4][i] = influenceChannel[i];
	writeHomingToFiles();
}

void processing::readOneRecordAmp(const string &pathToFile)
{
	vecX.clear();
	vecFirst.clear();
	vecSecond.clear();
	ifstream	mainFile(pathToFile);
	while (!mainFile.eof())
	{
		int readX, readFirst, readSecond;
		mainFile >> readX >> readFirst >> readSecond;
		vecX.push_back(readX);
		vecFirst.push_back(readFirst);
		vecSecond.push_back(readSecond);
	}
	mainFile.close();
}

void processing::readOneRecordForm(const string &pathToFile)
{
	vecX.clear();
	vecFirst.clear();
	vecSecond.clear();
	ifstream	mainFile(pathToFile);
	while (!mainFile.eof())
	{
		int readX, readFirst;
		mainFile >> readX >> readFirst;
		vecX.push_back(readX);
		vecFirst.push_back(readFirst);
	}
	mainFile.close();
}

void processing::computeForOneRecordAmp()
{
	readDataFromFiles();
	readOneRecordAmp(string(pathToSave) += string("amp") += to_string(howChipChannel));
	factorTransformation	exchangeFirst = getFactorTransformation(vecX, vecFirst);
	factorTransformation	exchangeSecond = getFactorTransformation(vecX, vecSecond);
	double factorIntegFirst		= getFactorIntegral(exchangeFirst.maxDeviation);
	double factorIntegSecond	= getFactorIntegral(exchangeSecond.maxDeviation);
	if (howChipChannel == 0)
	{
		data.transformationADC[0]	= exchangeFirst.factor;
		data.errorADC[0]			= exchangeFirst.error;
		data.transformationADC[1]	= exchangeSecond.factor;
		data.errorADC[1]			= exchangeSecond.error;
		data.nonlinearity[0]		= factorIntegFirst;
		data.nonlinearity[1]		= factorIntegSecond;
	}
	else
	{
		data.transformationADC[2]	= exchangeFirst.factor;
		data.errorADC[2]			= exchangeFirst.error;
		data.transformationADC[3]	= exchangeSecond.factor;
		data.errorADC[3]			= exchangeSecond.error;
		data.nonlinearity[2]		= factorIntegFirst;
		data.nonlinearity[3]		= factorIntegSecond;
	}
	writeDataToFiles();
}

void processing::computeForOneRecordForm()
{
	readDataFromFiles();
	readOneRecordForm(string(pathToSave) += string("form") += to_string(howChipChannel));
	factorTransformation	exchange = getFactorTransformation(vecX, vecFirst);
//	double factorInteg	= getFactorIntegral(exchange.maxDeviation);
	if (howChipChannel == 0)
	{
		data.transformationDAC[0]	= exchange.factor;
		data.errorDAC[0]			= exchange.error;
	}
	else
	{
		data.transformationDAC[1]	= exchange.factor;
		data.errorDAC[1]			= exchange.error;
	}
	writeDataToFiles();
}

const dataChip&	processing::returnData()	const
{
	return data;
}

const array< array<int, 4>, 4>&	processing::returnHoming()	const
{
	return homing;
}

