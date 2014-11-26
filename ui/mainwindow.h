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
	QPushButton	*exitButton;
	QPushButton *endButton;
	QHBoxLayout *mainHLayout, *controlHLayout, *settingsHLayout;
	QVBoxLayout *rightVLayout, *goApplyVLayout;

	void		initializeElements();
	void		initializeLayouts();
	inline void	addToList		(const string& addStr);
	void		readFromMemory	(const int& numberChannel);
	void		writeToMemory	(const int& numberChannel);

	tekModule	*module;
	tekOutput	*output;
	tekSettings *settings;
private slots:
	void		startButtonClick();
	void		endButtonClick();
	void		changeSettings();
};

#endif // MAINWINDOW_H
