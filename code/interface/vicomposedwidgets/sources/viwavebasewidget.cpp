#include "viwavebasewidget.h"

#define REPAINT_INTERVAL 100

ViWaveBaseWidget::ViWaveBaseWidget(QWidget *parent)
	: ViWidget(parent)
{	
	mPosition = 0;
	mZoomLevel = 0;
	mIntervalCounter = 0;
	mForm = NULL;
}

void ViWaveBaseWidget::updateWave()
{
	mForm = &mEngine->wave(mMode);
	mUnderCutOff = mForm->isUnderCutoff(mZoomLevel);
	++mIntervalCounter;
	if(mIntervalCounter > REPAINT_INTERVAL)
	{
		mIntervalCounter = 0;
		repaint();
	}
}

void ViWaveBaseWidget::setMode(ViAudio::Mode mode)
{
	mMode = mode;
	QObject::connect(mEngine, SIGNAL(chainChanged()), this, SLOT(updateWave()));
	QObject::connect(mEngine, SIGNAL(positionChanged(ViAudioPosition)), this, SLOT(positionChanged(ViAudioPosition)));
}

void ViWaveBaseWidget::paintEvent(QPaintEvent *event)
{
	if(mForm != NULL)
	{
		QPainter painter(this);

		static QPen penNormal(ViThemeManager::color(15));
		static QPen penAverage(ViThemeManager::color(14));

		int position = mPosition / (FIRST_ZOOM_LEVEL * qPow(ZOOM_LEVEL_INCREASE, mZoomLevel));
		int start = position - mHalfWidth;
		int end = position + mHalfWidth;
		int zoomSize = mForm->size(mZoomLevel);

		if(start < 0)
		{
			start = 0;
		}
		if(end > zoomSize)
		{
			end = zoomSize;
		}
		int drawStart = mHalfWidth + (start - position);

		if(mUnderCutOff)
		{
			int previous = mHalfHeight;
			for(int i = start; i < end; ++i)
			{
				painter.setPen(penNormal);
				int now = mForm->maximum(i, mZoomLevel) / mRatio;
				painter.drawLine(drawStart, previous, drawStart + 1, now);
				previous = now;
				drawStart++;
			}
		}
		else
		{
			for(int i = start; i < end; ++i)
			{
				painter.setPen(penNormal);
				painter.drawLine(drawStart, mHalfHeight, drawStart, mForm->maximum(i, mZoomLevel) / mRatio);
				painter.drawLine(drawStart, mHalfHeight, drawStart, mForm->minimum(i, mZoomLevel) / mRatio);

				painter.setPen(penAverage);
				painter.drawLine(drawStart, mHalfHeight, drawStart, mForm->maximumAverage(i, mZoomLevel) / mRatio);
				painter.drawLine(drawStart, mHalfHeight, drawStart, mForm->minimumAverage(i, mZoomLevel) / mRatio);
				drawStart++;
			}
		}
	}
}

void ViWaveBaseWidget::positionChanged(ViAudioPosition position)
{
	mPosition = position.position(ViAudioPosition::Samples);
	repaint();
}

void ViWaveBaseWidget::setZoomLevel(qint16 level)
{
	mZoomLevel = level;
	if(mForm != NULL)
	{
		mUnderCutOff = mForm->isUnderCutoff(mZoomLevel);
	}
	repaint();
}

void ViWaveBaseWidget::resizeEvent(QResizeEvent *event)
{
	mHalfWidth = width() / 2;
	mHalfHeight = height() / 2;
	mRatio = UNSIGNED_CHAR_HALF_VALUE / (height() / 2.0);
}
