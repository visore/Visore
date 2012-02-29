#include "vibutton.h"

ViButton::ViButton(QWidget *parent)
	: QWidget(parent)
{
	setSize(46, 46);
	mIsHover = false;
	mIsCheckable = false;
	mIsChecked = false;
}

ViButton::~ViButton()
{
}

void ViButton::setSize(int width, int height)
{
	setMinimumSize(width, height);
	setMaximumSize(width, height);
}

void ViButton::setNormalIcon(QImage image)
{
	mNormalImage = image;
	repaint();
}

void ViButton::setCheckedIcon(QImage image)
{
	mCheckedImage = image;
	mIsCheckable = true;
	repaint();
}

void ViButton::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	int halfHeight = height() / 2;
	int halfWidth = width() / 2;

	if(mIsHover)
	{
		painter.setPen(Qt::NoPen);
		painter.setRenderHint(QPainter::Antialiasing, true);

		QColor color = ViThemeManager::color(14);
		color.setAlpha(150);
		QGradient gradient;
		gradient.setColorAt(1, Qt::transparent);
		gradient.setColorAt(0.8, ViThemeManager::color(14));
		gradient.setColorAt(0.7, color);
		gradient.setColorAt(0, color);

		QLinearGradient linearGradient;

		QPainterPath clip;
		clip.moveTo(0, 0);
		clip.lineTo(width(), 0);
		clip.lineTo(0, height());
		clip.lineTo(width(), height());
		clip.closeSubpath();
		painter.setClipPath(clip);

		linearGradient = QLinearGradient(0, 0, width(), halfHeight);
		linearGradient.setStops(gradient.stops());
		linearGradient.setStart(halfWidth, halfHeight);
		linearGradient.setFinalStop(halfWidth, 0);
		painter.setBrush(linearGradient);
		painter.drawRect(0, 0, width(), halfHeight);

		linearGradient = QLinearGradient(0, 0, width(), halfHeight);
		linearGradient.setStops(gradient.stops());
		linearGradient.setStart(halfWidth, halfHeight);
		linearGradient.setFinalStop(halfWidth, height());
		painter.setBrush(linearGradient);
		painter.drawRect(0, halfHeight, width(), halfHeight);

		QPainterPath clip2;
		clip2.moveTo(width(), 0);
		clip2.lineTo(width(), height());
		clip2.lineTo(0, 0);
		clip2.lineTo(0, height());
		clip2.closeSubpath();
		painter.setClipPath(clip2);

		linearGradient = QLinearGradient(0, 0, halfWidth, height());
		linearGradient.setStops(gradient.stops());
		linearGradient.setStart(halfWidth, halfHeight);
		linearGradient.setFinalStop(width(), halfHeight);
		painter.setBrush(linearGradient);
		painter.drawRect(halfWidth, 0, width(), height());

		linearGradient = QLinearGradient(0, 0, halfWidth, height());
		linearGradient.setStops(gradient.stops());
		linearGradient.setStart(halfWidth, halfHeight);
		linearGradient.setFinalStop(0, halfHeight);
		painter.setBrush(linearGradient);
		painter.drawRect(0, 0, halfWidth, height());

		painter.setClipRect(0, 0, width(), height());
	}

	if(!mIsChecked || !mIsCheckable)
	{
		QRect rectangle(halfWidth - mNormalImage.width() / 2, halfHeight- mNormalImage.height() / 2, mNormalImage.width(), mNormalImage.height());
		painter.drawImage(rectangle, mNormalImage, mNormalImage.rect());
	}
	else
	{
		QRect rectangle(halfWidth - mCheckedImage.width() / 2, halfHeight- mCheckedImage.height() / 2, mCheckedImage.width(), mCheckedImage.height());
		painter.drawImage(rectangle, mCheckedImage, mCheckedImage.rect());
	}
}

void ViButton::enterEvent(QEvent *event)
{
	mIsHover = true;
	repaint();
}

void ViButton::leaveEvent(QEvent *event)
{
	mIsHover = false;
	repaint();
}

void ViButton::mouseReleaseEvent(QMouseEvent *event)
{
	if(mIsCheckable)
	{
		mIsChecked = !mIsChecked;
		emit clicked(mIsChecked);
		repaint();
	}
	else
	{
		emit clicked();
	}
}
