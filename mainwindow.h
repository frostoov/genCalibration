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
#include "interlocutor.h"

using namespace std;

typedef struct
{
	string leftFront, rightFront, interval, width, ampStart, ampEnd, ampStep;
} memoryGUI_t;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
private:
	QWidget		*centralWidget;
	QPushButton *startButton, *exitButton, *goButton, *endButton, *applyButton, *stopButton;
	QLineEdit	*leftFrontEdit, *rightFrontEdit, *intervalEdit, *widthEdit, *ampStartEdit;
	QLineEdit	*ampStepEdit, *ampEndEdit;
	QPlainTextEdit *viewList;
	QHBoxLayout *mainHLayout, *controlHLayout, *settingsHLayout;
	QVBoxLayout *rightVLayout, *editVLayout, *goApplyVLayout;
	QLabel		*leftFrontLabel, *rightFrontLabel, *intervalLabel, *widthLabel, *ampStartLabel;
	QLabel		*ampStepLabel, *ampEndLabel;
	QRadioButton	*channelOneButton, *channelTwoButton;
	QTimer		*goTimer;

	void		initializeElements();
	void		initializeLayouts();
	inline void	addToList(const string& addStr);
	void		readFromMemory(const int& numberChannel);
	void		writeToMemory(const int& numberChannel);

	ViInterlocutor	converVi;
	memoryGUI_t		oneChannelMemory, twoChannelMemory;
	double			amp, ampStep, ampEnd;
private slots:
	void		startButtonClick();
	void		endButtonClick();
	void		applyButtonClick();
	void		goButtonClick();
	void		chooseOneChannel()		{readFromMemory(1);}
	void		chooseTwoChannel()		{readFromMemory(2);}
	void		stopButtonClick();
	void		changeSettings();
	void		returnSettings();
};

#endif // MAINWINDOW_H
