#include "vispectrumwidget.h"
#include "ui_vispectrumwidget.h"

#define EXTRA_SPACE 0.05

#include <iostream>
using namespace std;

ViSpectrumWidget::ViSpectrumWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViSpectrumWidget();
	mUi->setupUi(this);

	mUi->realSpectrum->addGraph();
	mUi->imaginarySpectrum->addGraph();

	QObject::connect(mUi->sizeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(replot()));
	QObject::connect(mUi->frequencyBox, SIGNAL(currentIndexChanged(int)), this, SLOT(replot()));
	QObject::connect(mUi->valueBox, SIGNAL(currentIndexChanged(int)), this, SLOT(replot()));
	QObject::connect(mUi->representationBox, SIGNAL(currentIndexChanged(int)), this, SLOT(replot()));
}

ViSpectrumWidget::~ViSpectrumWidget()
{
	delete mUi;
}

void ViSpectrumWidget::setEngine(ViAudioEngine *engine)
{
	ViWidget::setEngine(engine);
}

void ViSpectrumWidget::replot()
{
	ViFrequencySpectrum<float> &spectrum = mEngine->spectrum();

	qint32 size = spectrum.size();
	QVector<qreal> x(size);
	QVector<qreal> yReal(size);
	QVector<qreal> yImaginary(size);

	qreal xMaximum, yRealMinimum, yRealMaximum, yImaginaryMinimum, yImaginaryMaximum;
	if(mUi->frequencyBox->currentIndex() == 0)
	{
		xMaximum = spectrum[size - 1].frequencyRange();
		for(int i = 0; i < size; ++i)
		{
			x[i] = spectrum[i].frequencyRange();
		}
	}
	else
	{
		xMaximum = spectrum[size - 1].frequencyHertz();
		for(int i = 0; i < size; ++i)
		{
			x[i] = spectrum[i].frequencyHertz();
		}
	}

	if(mUi->representationBox->currentIndex() == 0)
	{
		if(mUi->valueBox->currentIndex() == 0)
		{
			yRealMinimum = spectrum.minimum().polar().amplitude().real() * (1 + EXTRA_SPACE);
			yRealMaximum = spectrum.maximum().polar().amplitude().real() * (1 + EXTRA_SPACE);
			yImaginaryMinimum = spectrum.minimum().polar().amplitude().imaginary() * (1 + EXTRA_SPACE);
			yImaginaryMaximum = spectrum.maximum().polar().amplitude().imaginary() * (1 + EXTRA_SPACE);
			for(int i = 0; i < size; ++i)
			{
				yReal[i] = spectrum[i].polar().amplitude().real();
				yImaginary[i] = spectrum[i].polar().amplitude().imaginary();
			}
		}
		else
		{
			
			yRealMinimum = spectrum.minimum().polar().decibel().real() * (1 + EXTRA_SPACE);
			yRealMaximum = spectrum.maximum().polar().decibel().real() * (1 + EXTRA_SPACE);
			yImaginaryMinimum = spectrum.minimum().polar().decibel().imaginary() * (1 + EXTRA_SPACE);
			yImaginaryMaximum = spectrum.maximum().polar().decibel().imaginary() * (1 + EXTRA_SPACE);
			for(int i = 0; i < size; ++i)
			{
				yReal[i] = spectrum[i].polar().decibel().real();
				yImaginary[i] = spectrum[i].polar().decibel().imaginary();
			}
		}
	}
	else
	{
		if(mUi->valueBox->currentIndex() == 0)
		{
			yRealMinimum = spectrum.minimum().rectangular().amplitude().real() * (1 + EXTRA_SPACE);
			yRealMaximum = spectrum.maximum().rectangular().amplitude().real() * (1 + EXTRA_SPACE);
			yImaginaryMinimum = spectrum.minimum().rectangular().amplitude().imaginary() * (1 + EXTRA_SPACE);
			yImaginaryMaximum = spectrum.maximum().rectangular().amplitude().imaginary() * (1 + EXTRA_SPACE);
			for(int i = 0; i < size; ++i)
			{
				yReal[i] = spectrum[i].rectangular().amplitude().real();
				yImaginary[i] = spectrum[i].rectangular().amplitude().imaginary();
			}
		}
		else
		{
			yRealMinimum = spectrum.minimum().rectangular().decibel().real() * (1 + EXTRA_SPACE);
			yRealMaximum = spectrum.maximum().rectangular().decibel().real() * (1 + EXTRA_SPACE);
			yImaginaryMinimum = spectrum.minimum().rectangular().decibel().imaginary() * (1 + EXTRA_SPACE);
			yImaginaryMaximum = spectrum.maximum().rectangular().decibel().imaginary() * (1 + EXTRA_SPACE);
			for(int i = 0; i < size; ++i)
			{
				yReal[i] = spectrum[i].rectangular().decibel().real();
				yImaginary[i] = spectrum[i].rectangular().decibel().imaginary();
			}
		}
	}

	mUi->realSpectrum->graph(0)->setData(x, yReal);
	mUi->realSpectrum->xAxis->setLabel("x");
	mUi->realSpectrum->xAxis->setRange(0, xMaximum);
	mUi->realSpectrum->yAxis->setRange(yRealMinimum, yRealMaximum);
	mUi->realSpectrum->replot();

	mUi->imaginarySpectrum->graph(0)->setData(x, yImaginary);
	mUi->imaginarySpectrum->xAxis->setLabel("x");
	mUi->imaginarySpectrum->xAxis->setRange(0, xMaximum);
	mUi->imaginarySpectrum->yAxis->setRange(yImaginaryMinimum, yImaginaryMaximum);
	mUi->imaginarySpectrum->replot();

}


