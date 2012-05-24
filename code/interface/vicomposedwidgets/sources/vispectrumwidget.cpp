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

	QObject::connect(mUi->sizeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(recalculate()));

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
	QObject::connect(mEngine, SIGNAL(spectrumChanged()), this, SLOT(replot()));
}

void ViSpectrumWidget::show()
{
	recalculate();
	ViWidget::show();
}

void ViSpectrumWidget::recalculate()
{
	mEngine->calculateSpectrum(mUi->sizeBox->currentText().toInt());
}

void ViSpectrumWidget::replot()
{
	ViFrequencySpectrum<float> &spectrum = mEngine->spectrum();
	qint32 size = spectrum.size();

	if(size > 0)
	{
		QVector<qreal> x(size);
		QVector<qreal> yReal(size);
		QVector<qreal> yImaginary(size);

		QString labelX = "Frequency";
		QString labelY = "Amplitude";
		QString unitX = "";
		QString unitY = "";

		bool fill = false;
		bool drawImaginary = true;

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
			unitX = "Hz";
		}

		if(mUi->representationBox->currentIndex() == 0)
		{
			fill = true;
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
				unitY = "dB";
				drawImaginary = false;
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
				unitY = "dB";
				drawImaginary = false;
			}
		}

		mUi->realSpectrum->setScale(ViFrequencyPlot::X, 0, xMaximum);
		mUi->realSpectrum->setScale(ViFrequencyPlot::Y, yRealMinimum, yRealMaximum);
		mUi->realSpectrum->setLabel(ViFrequencyPlot::X, labelX);
		mUi->realSpectrum->setLabel(ViFrequencyPlot::Y, labelY);
		mUi->realSpectrum->setUnit(ViFrequencyPlot::X, unitX);
		mUi->realSpectrum->setUnit(ViFrequencyPlot::Y, unitY);
		mUi->realSpectrum->fill(fill);
		mUi->realSpectrum->setData(x, yReal);

		mUi->imaginarySpectrum->setScale(ViFrequencyPlot::X, 0, xMaximum);
		mUi->imaginarySpectrum->setLabel(ViFrequencyPlot::X, labelX);
		mUi->imaginarySpectrum->setLabel(ViFrequencyPlot::Y, labelY);
		mUi->imaginarySpectrum->setUnit(ViFrequencyPlot::X, unitX);
		mUi->imaginarySpectrum->setUnit(ViFrequencyPlot::Y, unitY);
		mUi->imaginarySpectrum->fill(fill);
		if(drawImaginary)
		{
			mUi->imaginarySpectrum->setScale(ViFrequencyPlot::Y, yImaginaryMinimum, yImaginaryMaximum);
			mUi->imaginarySpectrum->setData(x, yImaginary);
		}
		else
		{
			mUi->imaginarySpectrum->setScale(ViFrequencyPlot::Y, 0, 1);
			mUi->imaginarySpectrum->setData(QVector<qreal>(0), QVector<qreal>(0));
		}
	}
}


