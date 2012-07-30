#include "vispectrumwidget.h"
#include "visamplesizeconverter.h"
#include <QPainter>

#define DEFAULT_BARS 40
#define SHORT_MAX 65535

ViSpectrumWidget::ViSpectrumWidget(QWidget *parent)
	: ViWidget(parent)
{
	mBars = DEFAULT_BARS;
	mSpectrumChanged = false;
	mCurrentSpectrumIndex = -1;

	QObject::connect(engine(), SIGNAL(spectrumChanged(ViRealSpectrum, qint64)), this, SLOT(addSpectrum(ViRealSpectrum, qint64)));
	QObject::connect(engine(), SIGNAL(positionChanged(ViAudioPosition)), this, SLOT(update(ViAudioPosition)));
}

void ViSpectrumWidget::setBars(int bars)
{
	mBars = bars;
}

void ViSpectrumWidget::addSpectrum(ViRealSpectrum spectrum, qint64 milliseconds)
{
	QVector<qbyte16u> data(spectrum.size());
	for(int i = 0; i < data.size(); ++i)
	{
		data[i] = ViSampleSizeConverter::convertTo16u(spectrum[i].polar().decibel().real());
	}
	mSpectrums.append(data);
	mIntervals.append(milliseconds);
}

void ViSpectrumWidget::update(ViAudioPosition position)
{
	int milliseconds = position.position(ViAudioPosition::Milliseconds);
	int end = mIntervals.size() - 1;
	for(int i = mCurrentSpectrumIndex + 1; i < end; ++i)
	{
		if(milliseconds > mIntervals[i] && milliseconds <= mIntervals[i + 1])
		{
			mSpectrumChanged = true;
			mCurrentSpectrumIndex = i;
		}
	}
	
	//Test for the last spectrum
	if(!mSpectrumChanged && mIntervals.size() > 1 && milliseconds > mIntervals[mIntervals.size() - 1] && milliseconds <= mIntervals[mIntervals.size()])
	{ 
		mSpectrumChanged = true;
		mCurrentSpectrumIndex = mIntervals.size() - 1;
	}

	if(mSpectrumChanged)
	{
		mCurrentSpectrum = summarizeSpectrum(mSpectrums[mCurrentSpectrumIndex]);
		repaint();
	}
}

void ViSpectrumWidget::paintEvent(QPaintEvent *event)
{QPainter painter(this);
	if(mSpectrumChanged)
	{
		mSpectrumChanged = false;
		
		int barWidth = width() / qreal(mBars);
		for(int i = 0; i < mBars; ++i)
		{
			painter.fillRect(i * barWidth, height() - ((mCurrentSpectrum[i] / qfloat(SHORT_MAX)) * height() * 10), barWidth, height(), Qt::red);
		}
	}
	
	ViWidget::paintEvent(event);
}

QVector<qbyte16u> ViSpectrumWidget::summarizeSpectrum(QVector<qbyte16u> spectrum)
{
	QVector<qbyte16u> data(mBars);
	int end;
	qbyte16u average;
	for(int i = 0; i < mBars; ++i)
	{
		end = mBars + (i * mBars);
		average = 0;
		for(int j = i * mBars; j < end; ++j)
		{
			average += spectrum[j];
		}
		data[i] = ROUND(average / qreal(mBars));
	}
	return data;
}
