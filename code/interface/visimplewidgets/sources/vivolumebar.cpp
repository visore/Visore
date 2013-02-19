#include "vivolumebar.h"

ViVolumeBar::ViVolumeBar(QWidget *parent, ViVolumeBar::ViVolumeBarType type)
	: QWidget(parent)
{
	mIsHover = false;
	mIsPressed = false;
	mValue = 0;
	mRealValue = 0;
	setType(type);
}

void ViVolumeBar::setType(ViVolumeBar::ViVolumeBarType type)
{
	mType = type;
}

void ViVolumeBar::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	qint16 theWidth = width() - (PEN_WIDTH - 1);
	qint16 theHeight = height() - (PEN_WIDTH - 1);

	QPen pen(ViThemeManager::color(ViThemeColors::BorderColor1));
	pen.setWidth(PEN_WIDTH);

	QLinearGradient linearGradient(QPoint(0, theHeight / 2), QPoint(theWidth, theHeight / 2));

	if(mIsHover)
	{
		linearGradient.setColorAt(0, ViThemeManager::color(ViThemeColors::MainColor7));
		linearGradient.setColorAt(1, ViThemeManager::color(ViThemeColors::MainColor5));
	}
	else
	{
		linearGradient.setColorAt(0, ViThemeManager::color(ViThemeColors::MainColor6));
		linearGradient.setColorAt(1, ViThemeManager::color(ViThemeColors::MainColor4));
	}

	QBrush brush(linearGradient);
	painter.setBrush(brush);

	QPoint border[4];
	QPoint volume[3];
	QPoint point1, point2, point3, point4, point5, point6;

	if(mType == ViVolumeBar::Straight)
	{
		point1 = QPoint(1, theHeight);
		point2 = QPoint(theWidth, 1);
		point3 = QPoint(theWidth, theHeight);
		--theWidth;
		--theHeight;
		if(mValue < (PEN_WIDTH - 1) * 2)
		{
			mValue = (PEN_WIDTH - 1) * 2;
		}
		qint16 newHeight = mValue * (theHeight / qreal(theWidth));
		point4 = QPoint((PEN_WIDTH - 1) * 2, theHeight);
		point5 = QPoint(mValue, theHeight - newHeight + (PEN_WIDTH - 1));
		point6 = QPoint(mValue, theHeight);
	}
	else if(mType == ViVolumeBar::Angled)
	{
		point1 = QPoint(1, theHeight / 2);
		point2 = QPoint(theWidth, 1);
		point3 = QPoint(theWidth, theHeight);
		--theWidth;
		--theHeight;
		if(mValue < (PEN_WIDTH - 1) * 2)
		{
			mValue = (PEN_WIDTH - 1) * 2;
		}
		qint16 newHeight = mValue * (theHeight / qreal(theWidth));
		qint16 halfHeight = theHeight / 2;
		qint16 newPosition = ((mValue / qreal(theWidth)) * halfHeight);
		point4 = QPoint((PEN_WIDTH - 1) * 2, halfHeight);
		point5 = QPoint(mValue, halfHeight - newPosition + (PEN_WIDTH - 1));
		point6 = QPoint(mValue, newPosition + halfHeight + (PEN_WIDTH - 1));
	}

	border[0] = point1;
	border[1] = point2;
	border[2] = point3;
	border[3] = point1;
	volume[0] = point4;
	volume[1] = point5;
	volume[2] = point6;
	
	painter.setPen(Qt::NoPen);
	painter.drawPolygon(volume, 3);
	painter.setPen(pen);
	painter.drawPolyline(border, 4);
}

void ViVolumeBar::enterEvent(QEvent *event)
{
	mIsHover = true;
	repaint();
}

void ViVolumeBar::leaveEvent(QEvent *event)
{
	mIsHover = false;
	repaint();
}

void ViVolumeBar::mousePressEvent(QMouseEvent *event)
{
	mIsPressed = true;
	if(mIsHover)
	{
		setXValue(event->x());
		repaint();
	}
}

void ViVolumeBar::mouseReleaseEvent(QMouseEvent *event)
{
	mIsPressed = false;
}

void ViVolumeBar::mouseMoveEvent(QMouseEvent *event)
{
	if(mIsPressed && mIsHover)
	{
		setXValue(event->x());
		repaint();
	}
}

void ViVolumeBar::setXValue(qint16 value)
{
	if(value < PEN_WIDTH + 6)
	{
		mValue = PEN_WIDTH + 6;
	}
	else if(value > width() - PEN_WIDTH)
	{
		mValue = width() - PEN_WIDTH;
	}
	else
	{
		mValue = value;
	}
	mRealValue = ((qreal(mValue) - PEN_WIDTH - 6) / (width() - 6 - (PEN_WIDTH * 2))) * 100.0;
	emit valueChanged(mRealValue);
}

void ViVolumeBar::setValue(qint8 value)
{
	setXValue(qCeil(((qreal(value) * (width() - 6 - PEN_WIDTH * 2)) / 100.0) + 6 + PEN_WIDTH));
	repaint();
}

qint8 ViVolumeBar::value()
{
	return mRealValue;
}

void ViVolumeBar::setSize(qint16 width, qint16 height)
{
	setMinimumSize(width, height);
	setMaximumSize(width, height);
}
