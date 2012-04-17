#include "viwaveoverlaywidget.h"

#define CONTENT_MARGIN -4

ViWaveOverlayWidget::ViWaveOverlayWidget(ViAudioEngine *engine, ViAudioBuffer::ViAudioBufferType type, QWidget *parent)
	: ViWidget(parent)
{
	setEngine(engine);
	mForm = mEngine->waveSummary(type);
	ViObject::connect(mEngine, SIGNAL(positionChanged(ViAudioPosition)), this, SLOT(positionChanged(ViAudioPosition)));
	setMouseTracking(true);
	mPosition = 0;
	mZoomLevel = 0;
	mPointerPositionPixel = 0;
	mPointerPositionSample = 0;

	mHeightRatio = 0;
	mZoomRatio = 0;
}

void ViWaveOverlayWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	static QPen penPosition(ViThemeManager::color(12));
	static QPen penPointer(ViThemeManager::color(18), 2);

	painter.setPen(penPosition);
	painter.drawLine(0, mHalfHeight, width(), mHalfHeight);

	QRect rectangle(0, 0, mHalfWidth, height());
	QColor color = ViThemeManager::color(11);
	color = QColor(color.red(), color.green(), color.blue(), 100);
	painter.fillRect(rectangle, color);

	painter.drawLine(mHalfWidth, 0, mHalfWidth, height());

	painter.setPen(penPointer);

	int position = mPointerPositionSample / mZoomRatio;
	int maximum = mHalfHeight;
	int minimum = mHalfHeight;

	bool test = position >= 0 && position < mForm->size(mZoomLevel);
	if(test)
	{
		maximum = mForm->maximum(position, mZoomLevel) / mHeightRatio;
	}
	+mPointerPositionPixel;
	painter.drawLine(mPointerPositionPixel, 0, mPointerPositionPixel, maximum - 4);
	painter.drawEllipse(mPointerPositionPixel - 3, maximum - 3, 8, 8);
	if(!test || mUnderCutOff || maximum + 1 >= minimum)
	{
		painter.drawLine(mPointerPositionPixel, maximum + 6, mPointerPositionPixel, height());
	}
	else
	{
		minimum = mForm->minimum(position, mZoomLevel) / mHeightRatio;
		painter.drawLine(mPointerPositionPixel, maximum + 5, mPointerPositionPixel, minimum - 4);
		painter.drawEllipse(mPointerPositionPixel - 3, minimum - 3, 8, 8);
		painter.drawLine(mPointerPositionPixel, minimum + 5, mPointerPositionPixel, height());
	}
}


void ViWaveOverlayWidget::mouseMoveEvent(QMouseEvent *event)
{
	emit pointerMoved(((mPosition / mZoomRatio) - mHalfWidth + event->x()) * mZoomRatio);
}

void ViWaveOverlayWidget::positionChanged(ViAudioPosition position)
{
	mPosition = position.sample();
}

void ViWaveOverlayWidget::setZoomLevel(qint16 level)
{
	mZoomLevel = level;
	mZoomRatio = FIRST_ZOOM_LEVEL * qPow(ZOOM_LEVEL_INCREASE, mZoomLevel);
	mUnderCutOff = mForm->isUnderCutoff(mZoomLevel);
	repaint();
}

void ViWaveOverlayWidget::setPointer(qint32 position)
{
	mPointerPositionSample = position;
	mPointerPositionPixel = ((position - mPosition) / mZoomRatio) + mHalfWidth;
	update();
}

void ViWaveOverlayWidget::resizeEvent(QResizeEvent *event)
{
	mHalfWidth = (width() / 2);
	mHalfHeight = (height() / 2);
	mHeightRatio = UNSIGNED_CHAR_HALF_VALUE / (height() / 2.0);
}
