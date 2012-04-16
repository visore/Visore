#include "viwavewidget.h"

ViWaveWidget::ViWaveWidget(ViAudioEngine *engine, ViAudioBuffer::ViAudioBufferType type, QWidget *parent)
	: ViWidget(parent)
{
	setEngine(engine);
	setMouseTracking(true);

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

void ViWaveWidget::paintEvent(QPaintEvent *event)
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

void ViWaveWidget::positionChanged(ViAudioPosition position)
{
	mPosition = position.sample();
	repaint();
}

void ViWaveWidget::setZoomLevel(qint16 level)
{
	mZoomLevel = level;
	mUnderCutOff = mForm->isUnderCutoff(mZoomLevel);
	repaint();
}

qint64 ViWaveWidget::position()
{
	return mPosition;
}

void ViWaveWidget::resizeEvent(QResizeEvent *event)
{
	mHalfWidth = width() / 2;
	mHalfHeight = height() / 2;
	mRatio = UNSIGNED_CHAR_HALF_VALUE / (height() / 2.0);
}

void ViWaveWidget::mouseMoveEvent(QMouseEvent *event)
{

qreal ratio = UNSIGNED_CHAR_HALF_VALUE / (height() / 2.0);
qreal position = mPosition / mRatio;
qreal start = position - mHalfWidth;
qreal end = position + mHalfWidth;

	

qreal pos = (event->x() - mHalfWidth + position) / double(FIRST_ZOOM_LEVEL * qPow(ZOOM_LEVEL_INCREASE, mZoomLevel));


start+=event->x();
pos=start;
//if(pos < 0) pos = 0;
//if(pos < mForm->size(mZoomLevel)) 

//cout<<int(mForm->maximum(pos, mZoomLevel))<<" "<<(127-int(mForm->maximum(pos, mZoomLevel)))/255.0<<"  "<<pos<<endl;





pos*=double(FIRST_ZOOM_LEVEL * qPow(ZOOM_LEVEL_INCREASE, mZoomLevel));
emit pointerMoved(pos);
}

qreal ViWaveWidget::maximum(qint64 position)
{
	if(position >= mForm->size(mZoomLevel))
	{
		return mHalfHeight;
	}
	return mForm->maximum(position, mZoomLevel) / mRatio;
}

qreal ViWaveWidget::minimum(qint64 position)
{
	if(mUnderCutOff)
	{
		return maximum(position);
	}
	else if(position >= mForm->size(mZoomLevel))
	{
		return mHalfHeight;
	}
	return mForm->minimum(position, mZoomLevel) / mRatio;
}
