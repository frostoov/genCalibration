#ifndef TEKSETTINGS_H
#define TEKSETTINGS_H

#include <QWidget>
#include <QGroupBox>
#include <QFormLayout>
#include <QRadioButton>
#include <QPushButton>
#include <QLineEdit>

#include <iostream>

#include "observer/observer.h"
#include "tekVisa/tekmodule.h"
#include "types.hpp"

using namespace std;

class tekSettings : public QGroupBox, public Observer
{
	Q_OBJECT
public:
	explicit tekSettings(tekModule *mod,const QString &title, QWidget *parent = 0);
	~tekSettings();
	int currentChannel () {return channel;}
	void obsUpdate(const Subject *subject);

	void setHighLevel();
	void setLowLevel();
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
	void setChannel();
	void updateSettings();
private:
	tekModule		*_module;
	int channel;

	QFormLayout *layout;

	QRadioButton *channelOneC;
	QRadioButton *channelTwoC;

	QLineEdit	*leftFrontL;
	QLineEdit	*rightFrontL;
	QLineEdit	*intervalL;
	QLineEdit	*widthL;
	QLineEdit	*lowLevelL;
	QLineEdit   *highLevelL;
};

#endif // TEKSETTINGS_H
