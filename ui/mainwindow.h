#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//The fetred
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QBoxLayout>
#include <QDialog>
#include <QLabel>
#include <QRadioButton>
#include <QTableWidget>
#include <string>
#include <thread>

#include "tekoutput.h"
#include "teksettings.h"
#include "plots.h"
#include "tekVisa/processing.h"

#include "chip/blocks.h"

using namespace std;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
private:
	QWidget		*centralWidget;
	QPushButton *startButton;
	QPushButton *goButton;
	QPushButton	*stopButton;
	QPushButton	*exitButton;
	QPushButton *endButton;
	QPushButton	*showSettings;
	QPushButton	*showTables;
	QLineEdit	*pathToSaveL;
	QLineEdit	*numberChipL;
	QHBoxLayout *mainHLayout, *controlHLayout, *settingsHLayout;
	QVBoxLayout *rightVLayout, *goApplyVLayout, *addPlotsVLayout;
	QFormLayout	*saveVLayout;

	QWidget		*tablesWidget;
	QTableWidget*dataTable;
	QTableWidget*homingTable;
	QHBoxLayout	*tableHLayout;
	array< array<QTableWidgetItem, 4>, 4>	homingItems;
//	vector< vector<QTableWidgetItem> >	homingItems;

	void		initializeElements	();
	void		initializeLayouts	();
	inline void	addToList			(const string& addStr);
	void		readFromMemory		(const int& numberChannel);
	void		writeToMemory		(const int& numberChannel);
	void		writeToDataTable	();
	void		writeToHomingTable	();

	tekModule	*module;
	tekOutput	*output;
	tekSettings *settings;
	chipModule	*chip;
	plotsOutput	*plots;
	processing	*calculation;

private slots:
	void		startButtonClick();
	void		stopButtonClick();
	void		goButtonClick();
	void		endButtonClick();
	void		showSettingsClick();
	void		showTablesClick();
};

#endif // MAINWINDOW_H
