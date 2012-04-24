#include "viwavebasewidget.h"

ViWaveBaseWidget::ViWaveBaseWidget(ViAudioEngine *engine, ViAudioBuffer::ViAudioBufferType type, QWidget *parent)
	: ViWidget(parent)
{
	setEngine(engine);

	mForm = mEngine->waveSummary(type);
	if(type == ViAudioBuffer::Original)
	{
		ViObject::connect(mEngine, SIGNAL(originalWaveChanged()), this, SLOT(repaint()));
	}
	else
	{
		ViObject::connect(mEngine, SIGNAL(correctedWaveChanged()), this, SLOT(repaint()));
	}
	ViObject::connect(mEngine, SIGNAL(positionChanged(ViAudioPosition)), this, SLOT(positionChanged(ViAudioPosition)));
	
	mPosition = 0;
	mZoomLevel = 0;
}

void ViWaveBaseWidget::paintEvent(QPaintEvent *event)
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

void ViWaveBaseWidget::positionChanged(ViAudioPosition position)
{
	mPosition = position.position(ViAudioPosition::Samples);
	repaint();
}

void ViWaveBaseWidget::setZoomLevel(qint16 level)
{
	mZoomLevel = level;
	mUnderCutOff = mForm->isUnderCutoff(mZoomLevel);
	repaint();
}

void ViWaveBaseWidget::resizeEvent(QResizeEvent *event)
{
	mHalfWidth = width() / 2;
	mHalfHeight = height() / 2;
	mRatio = UNSIGNED_CHAR_HALF_VALUE / (height() / 2.0);
}
