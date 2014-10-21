#include "mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
{
	initializeElements();
	initializeLayouts();
	this->setLayout(mainHLayout);

	connect(exitButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(startButton, SIGNAL(clicked()), this, SLOT(startButtonClick()));
	connect(endButton, SIGNAL(clicked()), this, SLOT(endButtonClick()));
	connect(goButton, SIGNAL(clicked()), this, SLOT(goButtonClick()));
	connect(applyButton, SIGNAL(clicked()), this, SLOT(applyButtonClick()));
	connect(channelOneButton, SIGNAL(clicked()), this, SLOT(chooseOneChannel()));
	connect(channelTwoButton, SIGNAL(clicked()), this, SLOT(chooseTwoChannel()));
	connect(stopButton, SIGNAL(clicked()), this, SLOT(stopButtonClick()));
	connect(goTimer, SIGNAL(timeout()), this, SLOT(changeSettings()));
}

void MainWindow::initializeLayouts()
{
	mainHLayout =		new QHBoxLayout;
	rightVLayout =		new QVBoxLayout;
	controlHLayout =	new QHBoxLayout;
	settingsHLayout =	new QHBoxLayout;
	editVLayout =		new QVBoxLayout;
	goApplyVLayout =	new QVBoxLayout;
	editVLayout->		addWidget(leftFrontLabel);
	editVLayout->		addWidget(leftFrontEdit);
	editVLayout->		addWidget(rightFrontLabel);
	editVLayout->		addWidget(rightFrontEdit);
	editVLayout->		addWidget(intervalLabel);
	editVLayout->		addWidget(intervalEdit);
	editVLayout->		addWidget(widthLabel);
	editVLayout->		addWidget(widthEdit);
	editVLayout->		addWidget(ampStartLabel);
	editVLayout->		addWidget(ampStartEdit);
	editVLayout->		addWidget(ampEndLabel);
	editVLayout->		addWidget(ampEndEdit);
	editVLayout->		addWidget(ampStepLabel);
	editVLayout->		addWidget(ampStepEdit);
	goApplyVLayout->	addWidget(applyButton);
	goApplyVLayout->	addWidget(goButton);
	goApplyVLayout->	addWidget(stopButton);
	settingsHLayout->	addLayout(editVLayout);
	settingsHLayout->	addLayout(goApplyVLayout);
	controlHLayout->	addWidget(startButton);
	controlHLayout->	addWidget(endButton);
	controlHLayout->	addWidget(exitButton);
	rightVLayout->		addLayout(controlHLayout);
	rightVLayout->		addWidget(channelOneButton);
	rightVLayout->		addWidget(channelTwoButton);
	rightVLayout->		addLayout(settingsHLayout);
	mainHLayout->		addWidget(viewList);
	mainHLayout->		addLayout(rightVLayout);
}

void MainWindow::initializeElements()
{
	startButton =		new QPushButton("Start session");
	endButton =			new QPushButton("End session");
	exitButton =		new QPushButton("Exit");
	goButton =			new QPushButton("Go");
	applyButton =		new QPushButton("Apply");
	stopButton =		new QPushButton("Stop");
	leftFrontEdit =		new QLineEdit;
	rightFrontEdit =	new QLineEdit;
	intervalEdit =		new QLineEdit;
	widthEdit =			new QLineEdit;
	ampStartEdit =		new QLineEdit;
	ampEndEdit =		new QLineEdit;
	ampStepEdit =		new QLineEdit;
	leftFrontLabel =	new QLabel("Left Front, ns");
	rightFrontLabel =	new QLabel("Right Front, ns");
	intervalLabel =		new QLabel("Interval, ms");
	widthLabel =		new QLabel("Width, ns");
	ampStartLabel =		new QLabel("Amplitude start, mV");
	ampEndLabel =		new QLabel("Amplitude end, mV");
	ampStepLabel =		new QLabel("Step amplitude per second");
	channelOneButton =	new QRadioButton;
	channelOneButton->setText("Channel One");
	channelOneButton->setChecked(true);
	channelTwoButton =	new QRadioButton;
	channelTwoButton->setText("ChannelTwo");
	viewList =			new QListWidget;
	goTimer =			new QTimer;
}

MainWindow::~MainWindow()
{
	delete startButton;	delete endButton;	delete exitButton;	delete goButton;
	delete leftFrontEdit;	delete rightFrontEdit;	delete intervalEdit;	delete widthEdit;
	delete ampStepEdit;		delete leftFrontLabel;	delete rightFrontLabel;	delete intervalLabel;
	delete widthLabel;	delete ampStartLabel;	delete channelOneButton;
	delete editVLayout;	delete settingsHLayout;	delete controlHLayout;	delete rightVLayout;
	delete mainHLayout; delete channelTwoButton;	delete ampStepLabel;	delete ampStepEdit;
	delete ampStartLabel;	delete ampEndEdit; delete ampEndLabel;	delete stopButton;
	delete goTimer;
}

void MainWindow::addToList(const string &addStr)
{
	viewList->addItem("##############\n\n");
	viewList->addItem(QString::fromStdString(addStr));
	viewList->addItem("\n\n##############");
}

void MainWindow::startButtonClick()
{
	if (channelOneButton->isChecked() == true)
		converVi.setChannel(1);
	if (channelTwoButton->isChecked() == true)
		converVi.setChannel(2);
	if (converVi.openSession() == false)
		addToList("Open session is failed");
	else
		addToList("Open successfully");
	converVi.setDefaultSettings(1);
	converVi.setDefaultSettings(2);
}

void MainWindow::endButtonClick()
{
	if (converVi.closeSession() == false)
		addToList("Close session is failed");
	else
		addToList("Close session successfully");
}

void MainWindow::applyButtonClick()
{
	bool success = true;
	if (channelOneButton->isChecked() == true)
		converVi.setChannel(1);
	if (channelTwoButton->isChecked() == true)
		converVi.setChannel(2);

	if (converVi.setAmplitude(ampStartEdit->text().toInt()) == false)
	{
		addToList("Set amplitude is failed");
		success = false;
	}
	if (converVi.setLeftFront(leftFrontEdit->text().toInt()) == false)
	{
		addToList("Set left front is failed");
		success = false;
	}
	if (converVi.setRightFront(rightFrontEdit->text().toInt()) == false)
	{
		addToList("Set right front is failed");
		success = false;
	}
	if (converVi.setWidth(widthEdit->text().toInt()) == false)
	{
		addToList("Set right front is failed");
		success = false;
	}
	if (converVi.setInterval(intervalEdit->text().toInt()) == false)
	{
		addToList("Set right front is failed");
		success = false;
	}
	if (success == true)
		addToList("All success");

	returnSettings();
	if (channelOneButton->isChecked() == true)
		writeToMemory(1);
	if (channelTwoButton->isChecked() == true)
		writeToMemory(2);
}

void MainWindow::goButtonClick()
{
	if (channelOneButton->isChecked() == true)
		converVi.setChannel(1);
	if (channelTwoButton->isChecked() == true)
		converVi.setChannel(2);
	converVi.activateChannel(true);
	amp = ampStartEdit->text().toDouble();
	ampEnd = ampEndEdit->text().toDouble();
	ampStep = ampStepEdit->text().toDouble();
	goTimer->start(10);
}

void MainWindow::writeToMemory(const int& numberChannel)
{
	if (numberChannel == 1)
	{
		oneChannelMemory.ampStart =		ampStartEdit->text().toStdString();
		oneChannelMemory.ampEnd =		ampEndEdit->text().toStdString();
		oneChannelMemory.ampStep =		ampStepEdit->text().toStdString();
		oneChannelMemory.interval =		intervalEdit->text().toStdString();
		oneChannelMemory.width =		widthEdit->text().toStdString();
		oneChannelMemory.leftFront =	leftFrontEdit->text().toStdString();
		oneChannelMemory.rightFront =	rightFrontEdit->text().toStdString();
	}
	if (numberChannel == 2)
	{
		twoChannelMemory.ampStart =		ampStartEdit->text().toStdString();
		twoChannelMemory.ampEnd =		ampEndEdit->text().toStdString();
		twoChannelMemory.ampStep =		ampStepEdit->text().toStdString();
		twoChannelMemory.interval =		intervalEdit->text().toStdString();
		twoChannelMemory.width =		widthEdit->text().toStdString();
		twoChannelMemory.leftFront =	leftFrontEdit->text().toStdString();
		twoChannelMemory.rightFront =	rightFrontEdit->text().toStdString();
	}
}

void MainWindow::readFromMemory(const int &numberChannel)
{
	if (numberChannel == 1)
	{
		ampStartEdit->setText(QString::fromStdString(oneChannelMemory.ampStart));
		ampEndEdit->setText(QString::fromStdString(oneChannelMemory.ampEnd));
		ampStepEdit->setText(QString::fromStdString(oneChannelMemory.ampStep));
		intervalEdit->setText(QString::fromStdString(oneChannelMemory.interval));
		widthEdit->setText(QString::fromStdString(oneChannelMemory.width));
		leftFrontEdit->setText(QString::fromStdString(oneChannelMemory.leftFront));
		rightFrontEdit->setText(QString::fromStdString(oneChannelMemory.rightFront));
	}
	if (numberChannel == 2)
	{
		ampStartEdit->setText(QString::fromStdString(twoChannelMemory.ampStart));
		ampEndEdit->setText(QString::fromStdString(twoChannelMemory.ampEnd));
		ampStepEdit->setText(QString::fromStdString(twoChannelMemory.ampStep));
		intervalEdit->setText(QString::fromStdString(twoChannelMemory.interval));
		widthEdit->setText(QString::fromStdString(twoChannelMemory.width));
		leftFrontEdit->setText(QString::fromStdString(twoChannelMemory.leftFront));
		rightFrontEdit->setText(QString::fromStdString(twoChannelMemory.rightFront));
	}
}

void MainWindow::stopButtonClick()
{
	if (channelOneButton->isChecked() == true)
		converVi.setChannel(1);
	if (channelTwoButton->isChecked() == true)
		converVi.setChannel(2);
	converVi.activateChannel(false);
	goTimer->stop();
}

void MainWindow::changeSettings()
{
	amp += ampStep;
	if (amp > ampEnd)
	{
		goTimer->stop();
		converVi.activateChannel(false);
		return;
	}
	converVi.setAmplitude(amp);
}

void MainWindow::returnSettings()
{
	widthEdit->setText(QString::fromStdString(converVi.getWidth()));
	ampStartEdit->setText(QString::fromStdString(converVi.getAmplitude()));
	leftFrontEdit->setText(QString::fromStdString(converVi.getLeftFront()));
	rightFrontEdit->setText(QString::fromStdString(converVi.getRightFront()));
	intervalEdit->setText(QString::fromStdString(converVi.getInterval()));
}
