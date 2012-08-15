#include "vispectrumwidget.h"
#include "visamplesizeconverter.h"
#include "vipowercalculator.h"
#include <QPainter>

#define SHORT_MAX 65535
#define MIRROR_RATIO 0.8

ViSpectrumWidget::ViSpectrumWidget(QWidget *parent)
	: ViWidget(parent)
{
	mCurrentSpectrumIndex = -1;
	mCurrentTime = -1;
	mMainHeight = height() * MIRROR_RATIO;

	setMode(ViSpectrumWidget::Amplitude);
	setSpacing(1);
	setBars(256);

	QObject::connect(engine(), SIGNAL(spectrumChanged(ViRealSpectrum, qint64)), this, SLOT(addSpectrum(ViRealSpectrum, qint64)), Qt::DirectConnection);
	QObject::connect(engine(), SIGNAL(positionChanged(ViAudioPosition)), this, SLOT(update(ViAudioPosition)), Qt::DirectConnection);
}

void ViSpectrumWidget::setBars(int bars)
{
	mOriginalBars = bars;
	mBars = ViPowerCalculator::closestPower(bars);
}

void ViSpectrumWidget::setMode(ViSpectrumWidget::Mode mode)
{
	if(mode == ViSpectrumWidget::Amplitude)
	{
		addSpectrumData = &ViSpectrumWidget::addAmplitudeData;
	}
	else if(mode == ViSpectrumWidget::Decibel)
	{
		addSpectrumData = &ViSpectrumWidget::addDecibelData;
	}
}

void ViSpectrumWidget::setSpacing(int spacing)
{
	mSpacing = spacing;
}

void ViSpectrumWidget::addSpectrum(ViRealSpectrum spectrum, qint64 milliseconds)
{
	if(spectrum.size() < mBars)
	{
		mBars = ViPowerCalculator::previousPower(spectrum.size());
	}
	mIntervals.append(milliseconds);
	(this->*addSpectrumData)(spectrum);
}

void ViSpectrumWidget::addAmplitudeData(ViRealSpectrum &spectrum)
{
	QVector<qbyte16u> data(spectrum.size());
	for(int i = 0; i < data.size(); ++i)
	{
		data[i] = ViSampleSizeConverter::convertTo16u(spectrum[i].polar().amplitude().real());
	}
	mSpectrums.append(data);
}

void ViSpectrumWidget::addDecibelData(ViRealSpectrum &spectrum)
{
	QVector<qbyte16u> data(spectrum.size());
	for(int i = 0; i < data.size(); ++i)
	{
		data[i] = ViSampleSizeConverter::convertTo16u(spectrum[i].polar().decibel().real());
	}
	mSpectrums.append(data);
}

void ViSpectrumWidget::update(ViAudioPosition position)
{
	int milliseconds = position.position(ViAudioPosition::Milliseconds);
	if(mCurrentTime < milliseconds)
	{
		mCurrentSpectrumIndex = -1;
	}
	mCurrentTime = milliseconds;

	//When stopped
	if(milliseconds == 0)
	{
		mCurrentSpectrumIndex = 0;
		mCurrentSpectrum.clear();
		repaint();
	}
	else
	{
		int end = mIntervals.size() - 1;
		for(int i = mCurrentSpectrumIndex + 1; i < end; ++i)
		{
			if(milliseconds > mIntervals[i] && milliseconds <= mIntervals[i + 1])
			{
				mCurrentSpectrumIndex = i;
				mCurrentSpectrum = summarizeSpectrum(mSpectrums[mCurrentSpectrumIndex]);
				repaint();
				return;
			}
		}
	}
}

void ViSpectrumWidget::resizeEvent(QResizeEvent *event)
{
	mMainHeight = height() * MIRROR_RATIO;

	QLinearGradient mainGradient;
	QLinearGradient mirrorGradient;

	mainGradient.setStart(0, mMainHeight);
	mainGradient.setFinalStop(0, 0);
	mainGradient.setColorAt(0, ViThemeManager::color(15));
	mainGradient.setColorAt(1, ViThemeManager::color(11));

	mirrorGradient.setStart(0, height() - mMainHeight);
	mirrorGradient.setFinalStop(0, height());
	mirrorGradient.setColorAt(0, ViThemeManager::color(15));
	mirrorGradient.setColorAt(0.5, ViThemeManager::color(15));
	mirrorGradient.setColorAt(1, Qt::transparent);

	mMainBrush = QBrush(mainGradient);
	mMirrorBrush = QBrush(mirrorGradient);
}

void ViSpectrumWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	int bars = mBars;
	int totalWidth = bars + ((bars - 1) * mSpacing);
	while(width() < totalWidth)
	{
		bars = ViPowerCalculator::previousPower(bars - 1);
		totalWidth = bars + ((bars - 1) * mSpacing);
	}

	if(mCurrentSpectrum.size() > 0)
	{
		int barWidth = (width() - ((bars - 1) * mSpacing)) / qreal(bars);
		int barHeight;
		for(int i = 0; i < bars; ++i)
		{
			barHeight = (log(mCurrentSpectrum[i]) / log(SHORT_MAX)) * mMainHeight;
			//barHeight = (mCurrentSpectrum[i] / qreal(SHORT_MAX)) * mMainHeight *50;
			if(barHeight < 1)
			{
				barHeight = 1;
			}
			painter.fillRect((i * barWidth) + (i * mSpacing), mMainHeight - barHeight, barWidth, barHeight, mMainBrush);
			painter.fillRect((i * barWidth) + (i * mSpacing), mMainHeight + mSpacing, barWidth, ceil(barHeight * (1 - MIRROR_RATIO)), mMirrorBrush);
		}
	}
	else
	{
		int barWidth = (width() - ((bars - 1) * mSpacing)) / qreal(bars);
		int barHeight = 1;
		for(int i = 0; i < bars; ++i)
		{
			painter.fillRect((i * barWidth) + (i * mSpacing), mMainHeight - barHeight, barWidth, barHeight, mMainBrush);
			painter.fillRect((i * barWidth) + (i * mSpacing), mMainHeight + mSpacing, barWidth, barHeight, mMirrorBrush);
		}
	}
	ViWidget::paintEvent(event);
}

QVector<qbyte16u> ViSpectrumWidget::summarizeSpectrum(QVector<qbyte16u> &spectrum)
{
	QVector<qbyte16u> data(mBars);
	int end;
	qbyte16u average;
	qreal combined = spectrum.size() / mBars;
	for(int i = 0; i < mBars; ++i)
	{
		end = combined + (i * combined);
		average = 0;
		for(int j = i * combined; j < end; ++j)
		{
			average += spectrum[j];
		}
		data[i] = ROUND(average / combined);
	}
	return data;
}
