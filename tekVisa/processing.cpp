#include "tekVisa/processing.h"

using namespace std;

processing::processing()
{

}

processing::~processing()
{

}

pair<double, double>	processing::getFactorTransformation(const vector<int> &ampX, const vector<int> &codeY)
{
	if (ampX.size() != codeY.size())
		return pair<double, double>(-1, -1);

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
	return pair<double, double>(factor, free);
}

void processing::setPathToFile(const string& pathToSaveInput)
{
	pathToSave = pathToSaveInput;

	data = {};
	homing = {};
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
			mainFile >> homing[i][j];
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

void processing::setHomingValues(int chipChannel, const vector<int> &minDeviation, const vector<int> &maxDeviation)
{
	readHomingFromFiles();
	vector<double> influenceChannel(4);
	for (int i = 0; i < 4; i++)
		if (i != chipChannel)
			influenceChannel[i] = 20.0 * log10(maxDeviation[i] / minDeviation[i]);
	influenceChannel[chipChannel] = -1000;
	for (int i = 0; i < 4; i++)
			homing[chipChannel][i] = influenceChannel[i];
	writeHomingToFiles();
}

const dataChip&	processing::returnData()	const
{
	return data;
}

const array< array<int, 4>, 4>&	processing::returnHoming()	const
{
	return homing;
}

