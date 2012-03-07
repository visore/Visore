#include "vidialer.h"

ViDialer::ViDialer(QWidget *parent)
	: QDial(parent)
{
	mIsHover = false;
}

void ViDialer::setSize(int width, int height)
{
	setMinimumSize(width, height);
	setMaximumSize(width, height);
}

void ViDialer::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	if(mIsHover)
	{
		QImage gradient = ViGradientCreator::createGradient(ViGradientCreator::Circle, width() + 10, height() + 10, ViThemeManager::color(14));
		painter.drawImage(rect(), gradient, gradient.rect());
	}
	QDial::paintEvent(event);
}

void ViDialer::enterEvent(QEvent *event)
{
	mIsHover = true;
	repaint();
}

void ViDialer::leaveEvent(QEvent *event)
{
	mIsHover = false;
	repaint();
}
