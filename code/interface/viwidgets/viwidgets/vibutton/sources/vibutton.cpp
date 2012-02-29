#include "vibutton.h"

ViButton::ViButton(QWidget *parent)
	: QWidget(parent)
{
	mIsHover = false;
	mIsCheckable = false;
	mIsChecked = false;
	mIsPressed = false;
}

ViButton::~ViButton()
{
}

void ViButton::setSize(int width, int height)
{
	setMinimumSize(width, height);
	setMaximumSize(width, height);
}

void ViButton::setCheckable()
{
	mIsCheckable = true;
}

void ViButton::setChecked(bool checked)
{
	mIsChecked = checked;
	repaint();
}

void ViButton::setNormalIcon(QImage image)
{
	mNormalImage = image;
	setSize(mNormalImage.width() + 8, mNormalImage.height() + 8);
	repaint();
}

void ViButton::setCheckedIcon(QImage image)
{
	mCheckedImage = image;
	repaint();
}

void ViButton::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	int halfHeight = height() / 2;
	int halfWidth = width() / 2;

	if(mIsHover)
	{
		QImage gradient = ViGradientCreator::createGradient(ViGradientCreator::Rectangle, width(), height());
		painter.drawImage(rect(), gradient, gradient.rect());
	}

	if(mIsPressed)
	{
		int smallerWidth = (mCheckedImage.width() - mCheckedImage.width() * 0.02);
		int smallerHeight = (mCheckedImage.height() - mCheckedImage.height() * 0.02);
		QRect rectangle(halfWidth - smallerWidth / 2, halfHeight - smallerHeight / 2, smallerWidth, smallerHeight);
		painter.drawImage(rectangle, mCheckedImage, mCheckedImage.rect());
	}
	else if(!mIsChecked || !mIsCheckable)
	{
		QRect rectangle(halfWidth - mNormalImage.width() / 2, halfHeight - mNormalImage.height() / 2, mNormalImage.width(), mNormalImage.height());
		painter.drawImage(rectangle, mNormalImage, mNormalImage.rect());
	}
	else
	{
		QRect rectangle(halfWidth - mCheckedImage.width() / 2, halfHeight - mCheckedImage.height() / 2, mCheckedImage.width(), mCheckedImage.height());
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
	mIsPressed = false;
	if(mIsCheckable)
	{
		mIsChecked = !mIsChecked;
		emit clicked(mIsChecked);
	}
	else
	{
		emit clicked();
	}
	repaint();
}

void ViButton::mousePressEvent(QMouseEvent *event)
{
	mIsPressed = true;
	repaint();
}
