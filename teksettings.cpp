#include "teksettings.h"

tekSettings::tekSettings(tekModule *mod, const QString &title, QWidget *parent)
	: QGroupBox(title,parent),_module(mod)
{
	createWidgets();
	createLayouts();
	for(short i = 0 ; i < 2; ++i)
		memset(&settings[i],0,sizeof(tekSettings_t));
}

tekSettings::~tekSettings()
{
	disconnect(leftFrontEdit,	&QLineEdit::editingFinished,
			   this,			&tekSettings::setLeftFront);
	disconnect(rightFrontEdit,	&QLineEdit::editingFinished,
			   this,			&tekSettings::setRightFront);
	disconnect(intervalEdit,	&QLineEdit::editingFinished,
			   this,			&tekSettings::setInterval);
	disconnect(widthEdit,		&QLineEdit::editingFinished,
			   this,			&tekSettings::setWidth);
	disconnect(amplitudeEdit,	&QLineEdit::editingFinished,
			   this,			&tekSettings::setAmplitude);

	disconnect(channelOneC,	&QRadioButton::clicked,
			   this,		&tekSettings::setChannel);
	disconnect(channelTwoC,	&QRadioButton::clicked,
			   this,		&tekSettings::setChannel);
	delete  leftFrontEdit;
	delete  rightFrontEdit;
	delete  intervalEdit;
	delete  widthEdit;
	delete  amplitudeEdit;
	delete  ampEndEdit;
	delete  ampStepEdit;
	delete	layout;
}

void tekSettings::createWidgets()
{
	leftFrontEdit	= new QLineEdit("0", this);
	rightFrontEdit	= new QLineEdit("0", this);
	intervalEdit	= new QLineEdit("0", this);
	widthEdit		= new QLineEdit("0", this);
	amplitudeEdit	= new QLineEdit("0", this);
	ampEndEdit		= new QLineEdit("0", this);
	ampStepEdit		= new QLineEdit("0", this);
	channelOneC		= new QRadioButton(this);
	channelTwoC		= new QRadioButton(this);

	connect(leftFrontEdit,	&QLineEdit::editingFinished,
			this,			&tekSettings::setLeftFront);
	connect(rightFrontEdit,	&QLineEdit::editingFinished,
			this,			&tekSettings::setRightFront);
	connect(intervalEdit,	&QLineEdit::editingFinished,
			this,			&tekSettings::setInterval);
	connect(widthEdit,		&QLineEdit::editingFinished,
			this,			&tekSettings::setWidth);
	connect(amplitudeEdit,	&QLineEdit::editingFinished,
			this,			&tekSettings::setAmplitude);

	connect(channelOneC,	&QRadioButton::clicked,
			this,			&tekSettings::setChannel);
	connect(channelTwoC,	&QRadioButton::clicked,
			this,			&tekSettings::setChannel);
	channelOneC->setChecked(true);
}

void tekSettings::createLayouts()
{
	layout			= new QFormLayout;
	layout->addRow(tr("Channel One"),channelOneC);
	layout->addRow(tr("Channel Two"),channelTwoC);
	layout->addRow(tr("Left Front"),leftFrontEdit);
	layout->addRow(tr("Right Front"),rightFrontEdit);
	layout->addRow(tr("Interval"),intervalEdit);
	layout->addRow(tr("Width"),widthEdit);
	layout->addRow(tr("Amplitutde Start"),amplitudeEdit);
	layout->addRow(tr("Amplitutde End"),ampEndEdit);
	layout->addRow(tr("Amplitude Step"),ampStepEdit);
	setLayout(layout);
}

void tekSettings::obsUpdate(const Subject *subject)
{
	if(subject == _module)
	{
		widthEdit->		setText(QString::fromStdString(_module->getWidth()));
		amplitudeEdit->	setText(QString::fromStdString(_module->getAmplitude()));
		leftFrontEdit->	setText(QString::fromStdString(_module->getLeftFront()));
		rightFrontEdit->setText(QString::fromStdString(_module->getRightFront()));
		intervalEdit->	setText(QString::fromStdString(_module->getInterval()));
		settings[channel].width		= widthEdit->text().toInt();
		settings[channel].amplitude	= amplitudeEdit->text().toInt();
		settings[channel].leftFront	= leftFrontEdit->text().toInt();
		settings[channel].rightFront= rightFrontEdit->text().toInt();
		settings[channel].interval	= intervalEdit->text().toInt();
	}
}

void tekSettings::setChannel()
{
	if(channel == getCurrentChannel() ) return;
	channel = getCurrentChannel();
	_module->setChannel( channel );
	readSettings();
	setSettings();
	_module->notify(tekModule::statusUpdate);
}

void tekSettings::setLeftFront()
{
	_module->setLeftFront( leftFrontEdit->text().toInt() );
	_module->notify(tekModule::statusUpdate);
}

void tekSettings::setRightFront()
{
	_module->setRightFront( rightFrontEdit->text().toInt() );
	_module->notify(tekModule::statusUpdate);
}

void tekSettings::setInterval()
{
	_module->setInterval( intervalEdit->text().toInt() );
	_module->notify(tekModule::statusUpdate);
}

void tekSettings::setWidth()
{
	_module->setWidth( widthEdit->text().toInt() );
	_module->notify(tekModule::statusUpdate);
}

void tekSettings::setAmplitude()
{
	_module->setAmplitude( amplitudeEdit->text().toInt() );
	_module->notify(tekModule::statusUpdate);
}

void tekSettings::setSettings()
{
	_module->setRightFront	(settings[channel].rightFront);
	_module->setLeftFront	(settings[channel].leftFront);
	_module->setAmplitude	(settings[channel].amplitude);
	_module->setInterval	(settings[channel].interval);
	_module->setWidth		(settings[channel].width);
}

void tekSettings::readSettings()
{
	amplitudeEdit->	setText(tr("%1").arg(settings[channel].amplitude));
	ampEndEdit->	setText(tr("%1").arg(settings[channel].ampEnd));
	ampStepEdit->	setText(tr("%1").arg(settings[channel].ampStep));
	intervalEdit->	setText(tr("%1").arg(settings[channel].interval));
	widthEdit->		setText(tr("%1").arg(settings[channel].width));
	leftFrontEdit->	setText(tr("%1").arg(settings[channel].leftFront));
	rightFrontEdit->setText(tr("%1").arg(settings[channel].rightFront));
}

int tekSettings::getCurrentChannel()
{
	if(channelOneC->isChecked()) return 0;
	if(channelTwoC->isChecked()) return 1;
	return -1;
}
