#include "teksettings.h"

tekSettings::tekSettings(tekModule *mod, const QString &title, QWidget *parent)
	: QGroupBox(title,parent),_module(mod)
{
	createWidgets();
	createLayouts();
	channel = 0;
}

tekSettings::~tekSettings()
{
	disconnect(leftFrontL,	&QLineEdit::editingFinished,
			   this,		&tekSettings::setLeftFront);
	disconnect(rightFrontL,	&QLineEdit::editingFinished,
			   this,		&tekSettings::setRightFront);
	disconnect(intervalL,	&QLineEdit::editingFinished,
			   this,		&tekSettings::setInterval);
	disconnect(widthL,		&QLineEdit::editingFinished,
			   this,		&tekSettings::setWidth);
	disconnect(highLevelL,	&QLineEdit::editingFinished,
			this,			&tekSettings::setHighLevel);
	disconnect(lowLevelL,	&QLineEdit::editingFinished,
			this,			&tekSettings::setLowLevel);

	disconnect(channelOneC,	&QRadioButton::clicked,
			   this,		&tekSettings::setChannel);
	disconnect(channelTwoC,	&QRadioButton::clicked,
			   this,		&tekSettings::setChannel);
	delete  leftFrontL;
	delete  rightFrontL;
	delete  intervalL;
	delete  widthL;
	delete	layout;
}

void tekSettings::createWidgets()
{
	leftFrontL	= new QLineEdit("0", this);
	rightFrontL	= new QLineEdit("0", this);
	intervalL	= new QLineEdit("0", this);
	widthL		= new QLineEdit("0", this);
	highLevelL	= new QLineEdit("0",this);
	lowLevelL	= new QLineEdit("0",this);
	channelOneC		= new QRadioButton(this);
	channelTwoC		= new QRadioButton(this);

	connect(leftFrontL,	&QLineEdit::editingFinished,
			this,		&tekSettings::setLeftFront);
	connect(rightFrontL,&QLineEdit::editingFinished,
			this,		&tekSettings::setRightFront);
	connect(intervalL,	&QLineEdit::editingFinished,
			this,		&tekSettings::setInterval);
	connect(widthL,		&QLineEdit::editingFinished,
			this,		&tekSettings::setWidth);
	connect(highLevelL,	&QLineEdit::editingFinished,
			this,		&tekSettings::setHighLevel);
	connect(lowLevelL,	&QLineEdit::editingFinished,
			this,		&tekSettings::setLowLevel);
	connect(channelOneC,&QRadioButton::clicked,
			this,		&tekSettings::setChannel);
	connect(channelTwoC,&QRadioButton::clicked,
			this,		&tekSettings::setChannel);
	channelOneC->setChecked(true);
	setChannel();
}

void tekSettings::createLayouts()
{
	layout			= new QFormLayout;
	layout->addRow(tr("Channel One"),channelOneC);
	layout->addRow(tr("Channel Two"),channelTwoC);
	layout->addRow(tr("Left Front"),leftFrontL);
	layout->addRow(tr("Right Front"),rightFrontL);
	layout->addRow(tr("Interval"),intervalL);
	layout->addRow(tr("Width"),widthL);
	layout->addRow(tr("High Level"),highLevelL);
	layout->addRow(tr("Low Level"),lowLevelL);
	setLayout(layout);
}

void tekSettings::obsUpdate(const Subject *subject)
{
	if(subject == _module)
	{
		updateSettings();
	}
}

void tekSettings::updateSettings()
{
	int val;
	if( _module->getWidth(val) )
		widthL->setText(QString::number(val,10));
	if( _module->getHighLevel(val) )
		highLevelL->setText(QString::number(val,10));
	if( _module->getLowLevel(val) )
		lowLevelL->setText(QString::number(val,10));
	if( _module->getLeftFront(val) )
		leftFrontL->setText(QString::number(val,10));
	if( _module->getRightFront(val) )
		rightFrontL->setText(QString::number(val,10));
	if( _module->getInterval(val) )
		intervalL->setText(QString::number(val,10));
}

void tekSettings::setChannel()
{
	if(channel == getCurrentChannel() ) return;
	channel = getCurrentChannel();
	_module->setChannel( channel );
	_module->notify(tekModule::statusUpdate);
}

void tekSettings::setLeftFront()
{
	_module->setLeftFront( leftFrontL->text().toInt() );
	_module->notify(tekModule::statusUpdate);
}

void tekSettings::setRightFront()
{
	_module->setRightFront( rightFrontL->text().toInt() );
	_module->notify(tekModule::statusUpdate);
}

void tekSettings::setInterval()
{
	_module->setInterval( intervalL->text().toInt() );
	_module->notify(tekModule::statusUpdate);
}

void tekSettings::setWidth()
{
	_module->setWidth( widthL->text().toInt() );
	_module->notify(tekModule::statusUpdate);
}

void tekSettings::setHighLevel()
{
	_module->setHighLevel( highLevelL->text().toInt() );
	_module->notify(tekModule::statusUpdate);
}

void tekSettings::setLowLevel()
{
	_module->setLowLevel( lowLevelL->text().toInt() );
	_module->notify(tekModule::statusUpdate);
}

void tekSettings::setSettings()
{
	_module->setRightFront	( rightFrontL->text().toInt() );
	_module->setLeftFront	( leftFrontL->text().toInt() );
	_module->setHighLevel	( highLevelL->text().toInt() );
	_module->setLowLevel	( lowLevelL->text().toInt() );
	_module->setInterval	( intervalL->text().toInt() );
	_module->setWidth		( widthL->text().toInt() );
}

void tekSettings::readSettings()
{
	highLevelL->setText(QString::number(_module->getSettgins().highLevel));
	lowLevelL->setText(QString::number(_module->getSettgins().lowLevel));
	intervalL->setText(QString::number(_module->getSettgins().interval));
	widthL->setText(QString::number( _module->getSettgins().width));
	leftFrontL->	setText(QString::number(_module->getSettgins().leftFront));
	rightFrontL->	setText(QString::number(_module->getSettgins().rightFront));
}

int tekSettings::getCurrentChannel()
{
	if(channelOneC->isChecked()) return 0;
	if(channelTwoC->isChecked()) return 1;
	return -1;
}
