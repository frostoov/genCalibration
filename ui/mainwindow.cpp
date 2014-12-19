#include "mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	module	= new tekModule;
	chip	= new chipModule	(module);
	output	= new tekOutput		(module, chip, this);
	settings= new tekSettings	(module,tr("Settings"));
	plots	= new plotsOutput	(chip, this);
	calculation	= new processing;

	initializeElements();
	initializeLayouts();
	centralWidget->setLayout(addPlotsVLayout);
	settings->hide();
	tablesWidget->setWindowTitle("Tables Window");
	dataTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	dataTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	homingTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	homingTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			homingTable->setItem(i, j, &homingItems[i][j]);

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			dataTable->setItem(i, j, &dataItems[i][j]);

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
	connect(stopButton,		&QPushButton::clicked,
			this,			&MainWindow::stopButtonClick);
	connect(showTables,		&QPushButton::clicked,
			this,			&MainWindow::showTablesClick);

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
	disconnect(stopButton,		&QPushButton::clicked,
			this,				&MainWindow::stopButtonClick);
	disconnect(showTables,		&QPushButton::clicked,
			this,				&MainWindow::showTablesClick);

	delete calculation;
	delete showSettings;
	delete plots;
	delete module;
	delete output;
	delete settings;
	delete startButton;
	delete stopButton;
	delete endButton;
	delete exitButton;
	delete goButton;
	delete pathToSaveL;
	delete dataTable;
	delete homingTable;
	delete showTables;
	delete tablesWidget;

//	delete settingsHLayout;
//	delete controlHLayout;
//	delete rightVLayout;
//	delete mainHLayout;

//	delete centralWidget;
}

void MainWindow::initializeLayouts()
{
	centralWidget = new QWidget(this);
	tablesWidget =	new QWidget;
	setCentralWidget(centralWidget);

	mainHLayout =		new QHBoxLayout;
	rightVLayout =		new QVBoxLayout;
	controlHLayout =	new QHBoxLayout;
	settingsHLayout =	new QHBoxLayout;
	goApplyVLayout =	new QVBoxLayout;
	addPlotsVLayout =	new QVBoxLayout;
	tableHLayout =		new QHBoxLayout;
	saveVLayout =		new QFormLayout;
	settingsHLayout->	addLayout(goApplyVLayout);
	controlHLayout->	addWidget(startButton);
	controlHLayout->	addWidget(showSettings);
	controlHLayout->	addWidget(showTables);
	controlHLayout->	addWidget(goButton);
	controlHLayout->	addWidget(stopButton);
	controlHLayout->	addWidget(endButton);
	controlHLayout->	addWidget(exitButton);
	saveVLayout->		addRow("Path to save folder:", pathToSaveL);
	saveVLayout->		addRow("Name chip", numberChipL);
	rightVLayout->		addLayout(controlHLayout);
	rightVLayout->		addLayout(saveVLayout);
//	rightVLayout->		addWidget(settings);
	mainHLayout->		addWidget(output);
	mainHLayout->		addLayout(rightVLayout);
	addPlotsVLayout->	addLayout(mainHLayout, 1);
	addPlotsVLayout->	addWidget(plots, 4);

	tableHLayout->		addWidget(dataTable);
	tableHLayout->		addWidget(homingTable);
	tablesWidget->		setLayout(tableHLayout);
}

void MainWindow::initializeElements()
{
	startButton =	new QPushButton("Start session",this);
	goButton =		new QPushButton("Go Calibration", this);
	endButton =		new QPushButton("End session",this);
	exitButton =	new QPushButton("Exit",this);
	showSettings =	new QPushButton("Show Settings", this);
	showTables =	new QPushButton("Show Tables", this);
	stopButton =	new QPushButton("Stop Calibration", this);
	pathToSaveL =	new QLineEdit("/home/main/log/", this);
	numberChipL =	new QLineEdit("0", this);

	dataTable =		new QTableWidget(4, 4, this);
	homingTable =	new QTableWidget(4, 4, this);
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
//	int howChipChannel = chip->searchAndSetChipChannel();
//	chip->calibration(100, 10);
//	chip->saveToFileAmp("/home/main/data/", howChipChannel);
//	chip->saveToFileThresh("/home/main/data/", howChipChannel);
//	calculation->setPathToFile("/home/main/data/");
//	calculation->setChipChannel(howChipChannel);
//	auto	aimingLowAmp	= chip->searchAiming(howChipChannel, 100);
//	auto	aimingHighAmp	= chip->searchAiming(howChipChannel, 10000);
//	if (howChipChannel == 0)
//	{
//		calculation->computeHomingValues(0, aimingLowAmp.first, aimingHighAmp.first);
//		calculation->computeHomingValues(1, aimingLowAmp.second, aimingHighAmp.second);
//	}
//	if (howChipChannel == 1)
//	{
//		calculation->computeHomingValues(2, aimingLowAmp.first, aimingHighAmp.first);
//		calculation->computeHomingValues(3, aimingLowAmp.second, aimingHighAmp.second);
//	}
//	calculation->computeForOneRecordAmp();
//	calculation->computeForOneRecordForm();
}

void MainWindow::stopButtonClick()
{
	chip->stopCalibration();
}

void MainWindow::showTablesClick()
{
	if (tablesWidget->isHidden() == true)
	{
		calculation->setPathToFile("/home/main/data/");
		calculation->readHomingFromFiles();
		calculation->readDataFromFiles();
		writeToHomingTable();
		writeToDataTable();
		tablesWidget->show();
	}
	else
		tablesWidget->hide();
}

void MainWindow::writeToHomingTable()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			homingItems[i][j].setText(
						QString::number(calculation->returnHoming()[i][j]));
		}
}

void MainWindow::writeToDataTable()
{
	for (int j = 0; j < 4; j++)
	{
		dataItems[j][0].setText(
					QString::number(calculation->returnData().transformationADC[j]));
		dataItems[j][1].setText(
					QString::number(calculation->returnData().errorADC[j]));
		dataItems[j][2].setText(
					QString::number(calculation->returnData().nonlinearity[j]));
	}
}


