#include "mainwindow.h"
#include <thread>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	module	= new tekModule;
	chip	= new chipModule	(module);
	output	= new tekOutput		(module, chip, this);
	settings= new tekSettings	(module,tr("Settings"),this);
	plots	= new plotsOutput	(chip, this);

	initializeElements();
	initializeLayouts();
	centralWidget->setLayout(addPlotsVLayout);
//	mainHLayout->setSizeConstraint(QLayout::SetFixedSize);
	rightVLayout->setSizeConstraint(QLayout::SetFixedSize);
	saveVLayout->setSizeConstraint(QLayout::SetFixedSize);
	output->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	plots->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//	addPlotsVLayout->setSizeConstraint(QLayout::SetFixedSize);
//	centralWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	centralWidget->setFixedSize(1024, 768);
	settings->hide();

	connect(exitButton,		&QPushButton::clicked,
			this,			&MainWindow::close);
	connect(startButton,	&QPushButton::clicked,
			this,			&MainWindow::startButtonClick);
	connect(endButton,		&QPushButton::clicked,
			this,			&MainWindow::endButtonClick);
	connect(goButton,		&QPushButton::clicked,
			this,			&MainWindow::goButtonClick);
	connect(showSettings,	&QPushButton::clicked,
			this,			&MainWindow::showSettingsClick);

	module->attach(output,tekModule::statusUpdate);
	module->attach(settings,tekModule::statusUpdate);
	chip->attach(output, chipModule::statusUpdate);
	chip->attach(plots, chipModule::statusUpdate);
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
	disconnect(showSettings,	&QPushButton::clicked,
			this,				&MainWindow::showSettingsClick);


	delete showSettings;
	delete plots;
	delete module;
	delete output;
	delete settings;
	delete startButton;
	delete endButton;
	delete exitButton;
	delete goButton;
	delete pathToSaveL;

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
	addPlotsVLayout =	new QVBoxLayout;
	saveVLayout =		new QFormLayout;
	settingsHLayout->	addLayout(goApplyVLayout);
	controlHLayout->	addWidget(startButton);
	controlHLayout->	addWidget(showSettings);
	controlHLayout->	addWidget(goButton);
	controlHLayout->	addWidget(endButton);
	controlHLayout->	addWidget(exitButton);
	saveVLayout->		addRow("Path to save folder:", pathToSaveL);
	saveVLayout->		addRow("Name chip", numberChipL);
	rightVLayout->		addLayout(controlHLayout);
	rightVLayout->		addLayout(saveVLayout);
	rightVLayout->		addWidget(settings);
	mainHLayout->		addWidget(output);
	mainHLayout->		addLayout(rightVLayout);
	addPlotsVLayout->	addLayout(mainHLayout);
	addPlotsVLayout->	addWidget(plots);
}

void MainWindow::initializeElements()
{
	startButton =	new QPushButton("Start session",this);
	goButton =		new QPushButton("Go Calibration", this);
	endButton =		new QPushButton("End session",this);
	exitButton =	new QPushButton("Exit",this);
	showSettings =	new QPushButton("Show Settings", this);
	numberChipL =	new QLineEdit(this);
	pathToSaveL =	new QLineEdit("/home/main/log/", this);
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
	chip->closeSession();
	module->notify(tekModule::statusUpdate);
}

void MainWindow::showSettingsClick()
{
	if (settings->isHidden() == true)
		settings->show();
	else
		settings->hide();
}

void MainWindow::goButtonClick()
{
	string helpToSave(pathToSaveL->text().toStdString());
	chip->ampCalibration(10);
	helpToSave += pathToSaveL->text().toStdString();
	QDir().mkdir(QString::fromStdString(helpToSave));
	chip->saveToFileAmp(helpToSave += "log");

	chip->threshCalibration(100);
	chip->saveToFileThresh(helpToSave += "Thresh");
}


