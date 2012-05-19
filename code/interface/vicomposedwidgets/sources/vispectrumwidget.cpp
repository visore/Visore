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
	qint32 size = 2049;//spectrum.size();
	spectrum.unlock();

	QVector<qreal> x(size);
	QVector<qreal> yReal(size);
	QVector<qreal> yImaginary(size);
	spectrum.lock();
int c=0;
int t=0;
cout<<size<<endl;

for(int i = 0; i < size; ++i)
	{
x[i] =(i/qreal(size))*44100;
		yReal[i] = 0;
		yImaginary[i] = 0;
}


	for(int i = 0; i < spectrum.size(); ++i)
	{
		//x[c] += i;
		//yReal[c] += spectrum[i].real();
		//yImaginary[c] += spectrum[i].imaginary();
yReal[c] += qSqrt((spectrum[i].real()*spectrum[i].real()) + (spectrum[i].imaginary()*spectrum[i].imaginary()));
yImaginary[c] += qAtan(spectrum[i].imaginary()/spectrum[i].real());
c++;
if(c==size) {c=0; t++;}
		//yReal[i] = qSqrt(spectrum[i].real()*spectrum[i].real() + spectrum[i].imaginary()*spectrum[i].imaginary());
		//yImaginary[i] = qAtan(spectrum[i].imaginary()/spectrum[i].real());

	}



qreal max = 0;
qreal min = 1000000;


for(int i = 0; i < size; ++i)
{
	//yReal[i] /= t;
	//yImaginary[i] /=t;
	//yReal[i] = qSqrt(yReal[i]*yReal[i] + yImaginary[i]*yImaginary[i]);
	//yImaginary[i] *=yImaginary[i];		


	//yImaginary[i] *=yImaginary[i];
	yReal[i]  = 20 * log10(yReal[i]);
	if(	yReal[i] > max) max = yReal[i];	
}



for(int i = 0; i < size; ++i)
{
	yReal[i] -= max;
}

int j = -1;
for(int i = 0; i < size-1; ++i)
{
	
	if(	yReal[i] < min){min = yReal[i]; j=i;}
}

cout<<"****: "<<min<<" "<<j<<endl;

cout<<endl;


	spectrum.unlock();

	mUi->realSpectrum->addGraph();
	mUi->realSpectrum->graph(0)->setData(x, yReal);
	mUi->realSpectrum->xAxis->setLabel("x");
	mUi->realSpectrum->xAxis->setRange(0, 22050);
	mUi->realSpectrum->yAxis->setRange(min, 0);
	mUi->realSpectrum->replot();

	mUi->imaginarySpectrum->addGraph();
	mUi->imaginarySpectrum->graph(0)->setData(x, yImaginary);
	mUi->imaginarySpectrum->xAxis->setLabel("x");
	mUi->imaginarySpectrum->xAxis->setRange(0, size);
	mUi->imaginarySpectrum->yAxis->setRange(0, 4);
	mUi->imaginarySpectrum->replot();
}


