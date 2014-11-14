#ifndef TEKSETTINGS_H
#define TEKSETTINGS_H

#include <QWidget>
#include <QGroupBox>
#include <QFormLayout>
#include <QRadioButton>
#include <QPushButton>
#include <QLineEdit>

#include <iostream>

#include "observer.h"
#include "tekmodule.h"
#include "tektypes.h"
#include "visa.h"

using namespace std;

class tekSettings : public QGroupBox, public Observer
{
	Q_OBJECT
public:
	explicit tekSettings(tekModule *mod,const QString &title, QWidget *parent = 0);
	~tekSettings();
	int currentChannel () {return channel;}
	void obsUpdate(const Subject *subject);

protected:
	void	setSettings();
	void	writeSettings();
	void	readSettings();
	int		getCurrentChannel();
	void	createWidgets();
	void	createLayouts();
protected:
	void setLeftFront();
	void setRightFront();
	void setInterval();
	void setWidth();
	void setAmplitude();
	void setChannel();
private:
	tekModule		*_module;
	tekSettings_t	settings[2];
	int channel;

	QFormLayout *layout;

	QRadioButton *channelOneC;
	QRadioButton *channelTwoC;

	QLineEdit	*leftFrontEdit;
	QLineEdit	*rightFrontEdit;
	QLineEdit	*intervalEdit;
	QLineEdit	*widthEdit;
	QLineEdit	*amplitudeEdit;
	QLineEdit	*ampStepEdit;
	QLineEdit	*ampEndEdit;
};

#endif // TEKSETTINGS_H
