#include "plots.h"
#include <iostream>

plotsOutput::plotsOutput(chipModule *chip, QWidget *parent)
	: QWidget(parent), _chip(chip)
{
	ampFirst	=	new QCustomPlot(this);
	ampSecond	=	new QCustomPlot(this);
	form		=	new QCustomPlot(this);

	ampHLayout	=	new QHBoxLayout;
	formVLayout	=	new QVBoxLayout;
	isRescaleButton	= new QCheckBox("auto Rescale", this);
	ampHLayout->	addWidget(ampFirst);
	ampHLayout->	addWidget(ampSecond);
	formVLayout->	addWidget(isRescaleButton);
	formVLayout->	addLayout(ampHLayout);
	formVLayout->	addWidget(form);

	setLayout(formVLayout);
	createFirst();
	createSecond();
	createForm();
}

plotsOutput::~plotsOutput()
{
	delete isRescaleButton;
	delete ampFirst;
	delete ampSecond;
	delete form;
	delete ampHLayout;
	delete formVLayout;
}

void plotsOutput::obsUpdate(const Subject *subject)
{
	if (subject == _chip)
	{
		if (_chip->returnMode() == chipModule::mode::ampFirst)
		{
			ampFirstData.first.push_back(_chip->returnLastAmp().first);
			ampFirstData.second.push_back(_chip->returnLastAmp().second);
			renderFirst();
		}
		if (_chip->returnMode() == chipModule::mode::ampSecond)
		{
			ampSecondData.first.push_back(_chip->returnLastAmp().first);
			ampSecondData.second.push_back(_chip->returnLastAmp().second);
			renderSecond();
		}
		if (_chip->returnMode() == chipModule::mode::form)
		{
			formData.first.push_back(_chip->returnLastThresh().first);
			formData.second.push_back(_chip->returnLastThresh().second);
			renderForm();
		}
	}
}

void plotsOutput::createFirst()
{
	ampFirst->addGraph();
	ampFirst->graph(0)->setLineStyle(QCPGraph::lsNone);
	ampFirst->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));

	ampFirst->rescaleAxes();

	ampFirst->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void plotsOutput::createSecond()
{
	ampSecond->addGraph();
	ampSecond->graph(0)->setLineStyle(QCPGraph::lsNone);
	ampSecond->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));

	ampSecond->rescaleAxes();

	ampSecond->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void plotsOutput::createForm()
{
	form->addGraph();
	form->graph(0)->setLineStyle(QCPGraph::lsNone);
	form->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));

	form->rescaleAxes();

	form->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void plotsOutput::renderFirst()
{
	ampFirst->graph(0)->setData(ampFirstData.first, ampFirstData.second);
	ampFirst->replot();
	if (isRescaleButton->isChecked() == true)
		ampFirst->rescaleAxes();
}

void plotsOutput::renderSecond()
{
	ampSecond->graph(0)->setData(ampSecondData.first, ampSecondData.second);
	ampSecond->replot();
	if (isRescaleButton->isChecked() == true)
		ampSecond->rescaleAxes();
}

void plotsOutput::renderForm()
{
	form->graph(0)->setData(formData.first, formData.second);
	form->replot();
	if (isRescaleButton->isChecked() == true)
		form->rescaleAxes();
}
