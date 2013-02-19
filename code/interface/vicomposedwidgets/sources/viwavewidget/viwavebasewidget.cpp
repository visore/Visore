#include <viwavebasewidget.h>
#include <QPainter>

#define REPAINT_INTERVAL 100

ViWaveBaseWidget::ViWaveBaseWidget(ViWidget *parent)
	: ViWidget(parent)
{
	mPosition = 0;
	mZoomLevel = 0;
	mIntervalCounter = 0;
	mForm = NULL;
}

void ViWaveBaseWidget::updateWave()
{
	if(mForm != NULL)
	{
		mUnderCutOff = mForm->isUnderCutoff(mZoomLevel);
		++mIntervalCounter;
		if(mIntervalCounter > REPAINT_INTERVAL)
		{
			mIntervalCounter = 0;
			repaint();
		}
	}
}

void ViWaveBaseWidget::paintEvent(QPaintEvent *event)
{
	if(mForm != NULL)
	{
		QPainter painter(this);

		static QPen penNormal(ViThemeManager::color(ViThemeColors::MainColor6));
		static QPen penAverage(ViThemeManager::color(ViThemeColors::MainColor5));
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

void ViWaveBaseWidget::setPosition(ViAudioPosition position)
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

void ViWaveBaseWidget::setWaveForm(ViWaveForm *form, ViAudioFormat format)
{
	mForm = form;
	mFormat = format;
	updateWave();
}

void ViWaveBaseWidget::clear()
{
	setWaveForm(NULL);
}

void ViWaveBaseWidget::resizeEvent(QResizeEvent *event)
{
	mHalfWidth = width() / 2;
	mHalfHeight = height() / 2;
	mRatio = UNSIGNED_CHAR_HALF_VALUE / (height() / 2.0);
}
