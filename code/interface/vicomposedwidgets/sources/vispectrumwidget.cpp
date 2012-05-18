#include "vispectrumwidget.h"
#include "ui_vispectrumwidget.h"

#include <iostream>
using namespace std;

ViSpectrumWidget::ViSpectrumWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViSpectrumWidget();
	mUi->setupUi(this);
}

ViSpectrumWidget::~ViSpectrumWidget()
{
	delete mUi;
}

void ViSpectrumWidget::setEngine(ViAudioEngine *engine)
{
	ViWidget::setEngine(engine);
}

void ViSpectrumWidget::recalculate()
{
	ViFrequencySpectrum &spectrum = mEngine->spectrum();
	spectrum.lock();
	qint32 size = spectrum.size();
	spectrum.unlock();

	QVector<qreal> x(size);
	QVector<qreal> yReal(size);
	QVector<qreal> yImaginary(size);
	spectrum.lock();
cout<<size<<endl;
	for(int i = 0; i < size; ++i)
	{
		x[i] = i;
		yReal[i] = spectrum[i].real();
		yImaginary[i] = -spectrum[i].imaginary();
		//yReal[i] = qSqrt(spectrum[i].real()*spectrum[i].real() + spectrum[i].imaginary()*spectrum[i].imaginary());
		//yImaginary[i] = qAtan(spectrum[i].imaginary()/spectrum[i].real());

	}
	spectrum.unlock();

	mUi->realSpectrum->addGraph();
	mUi->realSpectrum->graph(0)->setData(x, yReal);
	mUi->realSpectrum->xAxis->setLabel("x");
	mUi->realSpectrum->xAxis->setRange(0, size);
	mUi->realSpectrum->yAxis->setRange(-5, 5);
	mUi->realSpectrum->replot();

	mUi->imaginarySpectrum->addGraph();
	mUi->imaginarySpectrum->graph(0)->setData(x, yImaginary);
	mUi->imaginarySpectrum->xAxis->setLabel("x");
	mUi->imaginarySpectrum->xAxis->setRange(0, size);
	mUi->imaginarySpectrum->yAxis->setRange(-5, 5);
	mUi->imaginarySpectrum->replot();
}


