#ifndef TEKSETTINGS_H
#define TEKSETTINGS_H

#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QLineEdit>

#include "observer.h"

class tekSettings : public QGroupBox,Observer
{
public:
	explicit tekSettings(QWidget *parent = 0);

private:
	QLineEdit	*leftFrontEdit;
	QLineEdit	*rightFrontEdit;
	QLineEdit	*intervalEdit;
	QLineEdit	*widthEdit;
	QLineEdit	*ampStartEdit;
	QLineEdit	*ampStepEdit;
	QLineEdit	*ampEndEdit;
};

#endif // TEKSETTINGS_H
