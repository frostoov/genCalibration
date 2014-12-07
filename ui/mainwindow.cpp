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
	calculation	= new processing;

	initializeElements();
	initializeLayouts();
	centralWidget->setLayout(addPlotsVLayout);
//	mainHLayout->setSizeConstraint(QLayout::SetFixedSize);
	rightVLayout->setSizeConstraint(QLayout::SetFixedSize);
	saveVLayout->setSizeConstraint(QLayout::SetFixedSize);
	output->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	output->resize(output->size().width(), output->size().height() - 100);
	plots->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//	addPlotsVLayout->setSizeConstraint(QLayout::SetFixedSize);
//	centralWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	centralWidget->setFixedSize(1024, 768);
	settings->hide();
	dataTable->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	tablesWidget->setWindowTitle("Tables Window");

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			homingTable->setItem(i, j, &homingItems[i][j]);


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
	rightVLayout->		addWidget(settings);
	mainHLayout->		addWidget(output);
	mainHLayout->		addLayout(rightVLayout);
	addPlotsVLayout->	addLayout(mainHLayout);
	addPlotsVLayout->	addWidget(plots);

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
	numberChipL =	new QLineEdit("0", this);
	pathToSaveL =	new QLineEdit("/home/main/log/", this);

	dataTable =		new QTableWidget(4, 3, this);
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

}

void MainWindow::stopButtonClick()
{
	chip->stopCalibration();
}

void MainWindow::showTablesClick()
{
	if (tablesWidget->isHidden() == true)
	{
		calculation->readHomingFromFiles();
		writeToHomingTable();
		tablesWidget->show();
	}
	else
		tablesWidget->hide();
}

void MainWindow::writeToHomingTable()
{
	static array< array<QTableWidgetItem, 4>, 4>	homingItems;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			homingItems[i][j].setText(
						QString::number(calculation->returnHoming()[i][j]));
}

void MainWindow::writeToDataTable()
{
	for (int i = 0; i < 4; i++)
		dataTable->setItem(i, 0, new QTableWidgetItem(
								QString::number(calculation->returnData().transformationADC[i])));
	for (int i = 0; i < 4; i++)
		dataTable->setItem(i, 1, new QTableWidgetItem(
							   QString::number(calculation->returnData().errorADC[i])));
	for (int i = 0; i < 4; i++)
		dataTable->setItem(i, 2, new QTableWidgetItem(
							   QString::number(calculation->returnData().nonlinearity[i])));
	for (int i = 0; i < 2; i++)
		dataTable->setItem(i, 3, new QTableWidgetItem(
							   QString::number(calculation->returnData().transformationDAC[i])));
	for (int i = 0; i < 2; i++)
		dataTable->setItem(i, 4, new QTableWidgetItem(
							   QString::number(calculation->returnData().errorDAC[i])));
}


