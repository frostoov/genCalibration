#ifndef PLOTS_H
#define PLOTS_H

#include <QWidget>
#include <QCheckBox>
#include <QVector>
#include <QBoxLayout>

#include "observer/observer.h"
#include "tekVisa/chipmodule.h"
#include "qcustomplot.h"

class plotsOutput : public QWidget, public Observer
{
	Q_OBJECT
public:
	explicit plotsOutput(chipModule *chip, QWidget *parent = 0);
	~plotsOutput();
	void obsUpdate(const Subject *subject);
private slots:
	void	renderFirst();
	void	renderSecond();
	void	renderForm();

private:
	chipModule	*_chip;
	QCustomPlot	*ampFirst;
	QCustomPlot	*ampSecond;
	QCustomPlot	*form;
	pair<QVector<double>, QVector<double>>	ampFirstData;
	pair<QVector<double>, QVector<double>>	ampSecondData;
	pair<QVector<double>, QVector<double>>	formData;

	QHBoxLayout	*ampHLayout;
	QVBoxLayout	*formVLayout;
	QCheckBox	*isRescaleButton;

	void	createFirst();
	void	createSecond();
	void	createForm();
};

#endif // PLOTS_H
