#include "mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	module	= new tekModule;
	chip	= new chipModule	(module);
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
	connect(goButton,		&QPushButton::clicked,
			this,			&MainWindow::goButtonClick);

	module->attach(output,tekModule::statusUpdate);
	module->attach(settings,tekModule::statusUpdate);
	chip->attach(output, chipModule::statusUpdate);
}

MainWindow::~MainWindow()
{
	disconnect(exitButton,		&QPushButton::clicked,
			this,				&MainWindow::close);
	disconnect(startButton,		&QPushButton::clicked,
			this,				&MainWindow::startButtonClick);
	disconnect(endButton,		&QPushButton::clicked,
			this,				&MainWindow::endButtonClick);
	disconnect(goButton,		&QPushButton::clicked,
			this,				&MainWindow::goButtonClick);


	delete module;
	delete output;
	delete settings;
	delete startButton;
	delete endButton;
	delete exitButton;
	delete goButton;

//	delete settingsHLayout;
//	delete controlHLayout;
//	delete rightVLayout;
//	delete mainHLayout;

//	delete centralWidget;
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
	controlHLayout->	addWidget(goButton);
	controlHLayout->	addWidget(endButton);
	controlHLayout->	addWidget(exitButton);
	rightVLayout->		addLayout(controlHLayout);
	rightVLayout->		addWidget(settings);
	mainHLayout->		addWidget(output);
	mainHLayout->		addLayout(rightVLayout);
}

void MainWindow::initializeElements()
{
	startButton =	new QPushButton("Start session",this);
	goButton =		new QPushButton("Go Calibration", this);
	endButton =		new QPushButton("End session",this);
	exitButton =	new QPushButton("Exit",this);
}

void MainWindow::addToList(const string &addStr)
{
	output->appendPlainText(QString::fromStdString(addStr));
	output->ensureCursorVisible();
}

void MainWindow::startButtonClick()
{
	module->setChannel( settings->currentChannel() + 1);
	module->openSession();
	module->notify(tekModule::statusUpdate);
	chip->openSession();
}

void MainWindow::endButtonClick()
{
	module->closeSession();
	module->notify(tekModule::statusUpdate);
}

void MainWindow::goButtonClick()
{

}

void MainWindow::saveResultToFile()
{

}

