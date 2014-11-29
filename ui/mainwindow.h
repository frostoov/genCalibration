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
#include <QTimer>
#include <string>

#include "tekoutput.h"
#include "teksettings.h"

#include "muons/muons_2012.h"

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
	QHBoxLayout *mainHLayout, *controlHLayout, *settingsHLayout;
	QVBoxLayout *rightVLayout, *goApplyVLayout;

	void		initializeElements();
	void		initializeLayouts();
	inline void	addToList		(const string& addStr);
	void		readFromMemory	(const int& numberChannel);
	void		writeToMemory	(const int& numberChannel);

	void		saveResultToFile();

	tekModule	*module;
	tekOutput	*output;
	tekSettings *settings;
	chipModule	*chip;
private slots:
	void		startButtonClick();
	void		goButtonClick();
	void		endButtonClick();
};

#endif // MAINWINDOW_H
