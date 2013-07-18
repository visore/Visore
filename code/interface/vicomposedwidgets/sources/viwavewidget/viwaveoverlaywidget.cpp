#include <viwaveoverlaywidget.h>
#include <QPainter>

#define CONTENT_MARGIN -4

ViWaveOverlayWidget::ViWaveOverlayWidget(ViWidget *parent)
	: ViWidget(parent)
{
	setMouseTracking(true);
	mForm = NULL;
	mPosition = 0;
	mZoomLevel = 0;
	mPointerPositionPixel = 0;
	mPointerPositionSample = 0;

	mHeightRatio = 0;
	mZoomRatio = 0;

	mScroll = false;

	setStyleSheet("ViWaveOverlayWidget { border-radius: 4px; border-style: solid; border-width: 2px; border-color: " + ViThemeManager::color(ViThemeColors::BorderColor2).name() + " }");

	QColor color = ViThemeManager::color(ViThemeColors::MainColor1);
	mPastBox = new ViWidget(this);
	mPastBox->setStyleSheet("border-top-left-radius: 4px; border-bottom-left-radius: 4ps; background: rgba(" + QString::number(color.red()) + ", " + QString::number(color.green()) + ", " + QString::number(color.blue()) + ", 100);");
	mPastBox->lower();
}

ViWaveOverlayWidget::~ViWaveOverlayWidget()
{
	delete mPastBox;
}

void ViWaveOverlayWidget::updateWave()
{
	if(mForm != NULL)
	{
		mUnderCutOff = mForm->isUnderCutoff(mZoomLevel);
		repaint();
	}
}

void ViWaveOverlayWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	static QPen penPosition(ViThemeManager::color(ViThemeColors::MainColor1));
	static QPen penPointer(ViThemeManager::color(ViThemeColors::MainColor7), 2);

	painter.setPen(penPosition);
	painter.drawLine(0, mHalfHeight, width(), mHalfHeight);

	mPastBox->setSize(mHalfWidth, height());

	painter.drawLine(mHalfWidth, 0, mHalfWidth, height());

	if(mForm != NULL)
	{
		painter.setPen(penPointer);

		int position = mPointerPositionSample / mZoomRatio;
		int maximum = mHalfHeight;
		int minimum = mHalfHeight;

		bool test = position >= 0 && position < mForm->size(mZoomLevel);
		if(test)
		{
			maximum = mForm->maximum(position, mZoomLevel) / mHeightRatio;
		}
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
	ViWidget::paintEvent(event);
}


void ViWaveOverlayWidget::mouseMoveEvent(QMouseEvent *event)
{
	if(mScroll)
	{
		ViAudioPosition position(mPosition + ((mScrollStart - event->pos().x()) * mZoomRatio), ViAudioPosition::Samples, mFormat);
		setPosition(position);
		emit positionChanged(position);
		mScrollStart = event->pos().x();
	}
	emit pointerChanged(((mPosition / mZoomRatio) - mHalfWidth + event->x()) * mZoomRatio);
	emit mouseMoved();
}

void ViWaveOverlayWidget::setPosition(ViAudioPosition position)
{
	mPosition = position.position(ViAudioPosition::Samples);
	repaint();
}

void ViWaveOverlayWidget::setPosition(qint64 sample)
{
	if(mFormat.isValid())
	{
		ViAudioPosition position(sample, ViAudioPosition::Samples, mFormat);
		setPosition(position);
		emit positionChanged(position);
	}
	else
	{
		mPosition = 0;
	}
}

void ViWaveOverlayWidget::setZoomLevel(qint16 level)
{
	mZoomLevel = level;
	mZoomRatio = FIRST_ZOOM_LEVEL * qPow(ZOOM_LEVEL_INCREASE, mZoomLevel);
	if(mForm != NULL)
	{
		mUnderCutOff = mForm->isUnderCutoff(mZoomLevel);
	}
	repaint();
}

void ViWaveOverlayWidget::setWaveForm(ViWaveForm *form, ViAudioFormat format)
{
	mForm = form;
	mFormat = format;
	updateWave();
}

void ViWaveOverlayWidget::clear()
{
	setWaveForm(NULL);
}

void ViWaveOverlayWidget::setPointer(qint32 position)
{
	mPointerPositionSample = position;
	mPointerPositionPixel = ((position - mPosition) / mZoomRatio) + mHalfWidth;

	qreal max = 0.0;
	qreal min = 0.0;
	qreal maxAvg = 0.0;
	qreal minAvg = 0.0;
	position /= mZoomRatio;
	if(mForm != NULL && position >= 0 && position < mForm->size(mZoomLevel))
	{
		max = (UNSIGNED_CHAR_HALF_VALUE - int(mForm->maximum(position, mZoomLevel))) / 255.0;
		if(!mUnderCutOff)
		{
			min = (UNSIGNED_CHAR_HALF_VALUE - int(mForm->minimum(position, mZoomLevel))) / 255.0;
			maxAvg = (UNSIGNED_CHAR_HALF_VALUE - int(mForm->maximumAverage(position, mZoomLevel))) / 255.0;
			minAvg = (UNSIGNED_CHAR_HALF_VALUE - int(mForm->minimumAverage(position, mZoomLevel))) / 255.0;
		}
		else
		{
			min = max;
			maxAvg = max;
			minAvg = max;
		}
	}
	emit pointerValuesChanged(max, min, maxAvg, minAvg);
	update();
}

void ViWaveOverlayWidget::resizeEvent(QResizeEvent *event)
{
	mHalfWidth = (width() / 2);
	mHalfHeight = (height() / 2);
	mHeightRatio = UNSIGNED_CHAR_HALF_VALUE / (height() / 2.0);
}

void ViWaveOverlayWidget::wheelEvent(QWheelEvent *event)
{
	int degrees = event->delta() / 8;
	int steps = degrees / 15;
	emit zoomLevelChanged(-steps);
	event->accept();
}

void ViWaveOverlayWidget::mousePressEvent(QMouseEvent *event)
{
	mScroll = true;
	mScrollStart = event->pos().x();
}

void ViWaveOverlayWidget::mouseReleaseEvent(QMouseEvent *event)
{
	mScroll = false;
}
