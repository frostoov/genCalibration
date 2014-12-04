#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QBoxLayout>
#include <QDialog>
#include <QLabel>
#include <QRadioButton>
#include <string>

#include "tekoutput.h"
#include "teksettings.h"
#include "plots.h"

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
	QPushButton	*exitButton;
	QPushButton *endButton;
	QPushButton	*showSettings;
	QLineEdit	*pathToSaveL;
	QLineEdit	*numberChipL;
	QHBoxLayout *mainHLayout, *controlHLayout, *settingsHLayout;
	QVBoxLayout *rightVLayout, *goApplyVLayout, *addPlotsVLayout;

	void		initializeElements();
	void		initializeLayouts();
	inline void	addToList		(const string& addStr);
	void		readFromMemory	(const int& numberChannel);
	void		writeToMemory	(const int& numberChannel);

	tekModule	*module;
	tekOutput	*output;
	tekSettings *settings;
	chipModule	*chip;
	plotsOutput	*plots;

private slots:
	void		startButtonClick();
	void		goButtonClick();
	void		endButtonClick();
	void		showSettingsClick();
};

#endif // MAINWINDOW_H
