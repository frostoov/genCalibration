#include "mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	module	= new tekModule;
	output	= new tekOutput		(module,this);
	settings= new tekSettings	(module,tr("Settings"),this);

	initializeElements();
	initializeLayouts();
	centralWidget->setLayout(mainHLayout);

	connect(exitButton,		&QPushButton::clicked,
			this,			&MainWindow::close);
	connect(startButton,	&QPushButton::clicked,
			this,			&MainWindow::startButtonClick);
	connect(endButton,		&QPushButton::clicked,
			this,			&MainWindow::endButtonClick);

	module->attach(output,tekModule::statusUpdate);
	module->attach(settings,tekModule::statusUpdate);
}

MainWindow::~MainWindow()
{
	disconnect(exitButton,		&QPushButton::clicked,
			this,				&MainWindow::close);
	disconnect(startButton,		&QPushButton::clicked,
			this,				&MainWindow::startButtonClick);
	disconnect(endButton,		&QPushButton::clicked,
			this,				&MainWindow::endButtonClick);

	delete startButton;
	delete endButton;
	delete exitButton;

	delete settingsHLayout;
	delete controlHLayout;
	delete rightVLayout;
	delete mainHLayout;

	delete centralWidget;
}

void MainWindow::initializeLayouts()
{
	centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);

	mainHLayout =		new QHBoxLayout;
	rightVLayout =		new QVBoxLayout;
	controlHLayout =	new QHBoxLayout;
	settingsHLayout =	new QHBoxLayout;
	goApplyVLayout =	new QVBoxLayout;
	settingsHLayout->	addLayout(goApplyVLayout);
	controlHLayout->	addWidget(startButton);
	controlHLayout->	addWidget(endButton);
	controlHLayout->	addWidget(exitButton);
	rightVLayout->		addLayout(controlHLayout);
	rightVLayout->		addWidget(settings);
	mainHLayout->		addWidget(output);
	mainHLayout->		addLayout(rightVLayout);
}

void MainWindow::initializeElements()
{
	startButton =		new QPushButton("Start session",this);
	endButton =			new QPushButton("End session",this);
	exitButton =		new QPushButton("Exit",this);
}

void MainWindow::addToList(const string &addStr)
{
	output->appendPlainText(QString::fromStdString(addStr));
	output->ensureCursorVisible();
}

void MainWindow::startButtonClick()
{
	module->setChannel( settings->currentChannel() );

	if (module->openSession())
	{
		module->setDefaultSettings(1);
		module->setDefaultSettings(2);
	}
	module->notify(tekModule::statusUpdate);
}

void MainWindow::endButtonClick()
{
	if (module->closeSession() == false)
		addToList("Close session is failed");
	else
		addToList("Close session successfully");
}

//void MainWindow::goButtonClick()
//{
//	if (channelOneButton->isChecked() == true)
//		module->setChannel(1);
//	if (channelTwoButton->isChecked() == true)
//		module->setChannel(2);
//	module->activateChannel(true);
//	amp =		ampStartEdit->text().toDouble();
//	ampEnd =	ampEndEdit->text().toDouble();
//	ampStep =	ampStepEdit->text().toDouble();
//	goTimer->start(10);
//}

//void MainWindow::stopButtonClick()
//{
//	if (channelOneButton->isChecked() == true)
//		module->setChannel(1);
//	if (channelTwoButton->isChecked() == true)
//		module->setChannel(2);
//	module->activateChannel(false);
//	goTimer->stop();
//}

void MainWindow::changeSettings()
{
//	amp += ampStep;
//	if (amp > ampEnd)
//	{
//		goTimer->stop();
//		module->activateChannel(false);
//		return;
//	}
//	module->setAmplitude(amp);
}
