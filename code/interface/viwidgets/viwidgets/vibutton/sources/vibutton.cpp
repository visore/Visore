#include "vibutton.h"

ViButton::ViButton(QWidget *parent)
	: QWidget(parent)
{
	mIsHover = false;
	mIsCheckable = false;
	mIsChecked = false;
	mIsPressed = false;
	setGlow(ViThemeManager::color(14));
	mHasGradient = false;
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

void ViButton::setHoverIcon(QImage image)
{
	mHoverImage = image;
	repaint();
}

void ViButton::setSelectedIcon(QImage image)
{
	mSelectedImage = image;
	repaint();
}

void ViButton::setGlow(QColor color, ViGradientCreator::ViGradientType type, int x, int y, int width, int height)
{
	mHasGradient = true;
	mGlowColor = color;
	mGlowType = type;
	mGlowX = x;
	mGlowY = y;
	mGlowWidth = width;
	mGlowHeight = height;
}

void ViButton::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	int halfHeight = height() / 2;
	int halfWidth = width() / 2;

	if(mIsHover && mHasGradient)
	{
		int glowX = mGlowX;
		int glowY = mGlowY;
		int glowWidth = mGlowWidth;
		int glowHeight = mGlowHeight;
		if(glowX < 0)
		{
			glowX = rect().x() + 3;
		}
		if(glowY < 0)
		{
			glowY = rect().y() + 3;
		}
		if(glowWidth < 0)
		{
			glowWidth = width() - 6;
		}
		if(glowHeight < 0)
		{
			glowHeight = height() - 6;
		}
		QImage gradient = ViGradientCreator::createGradient(mGlowType, glowWidth, glowHeight, mGlowColor);
		painter.drawImage(QRect(glowX, glowY, glowWidth, glowHeight), gradient, gradient.rect());
	}

	if(mIsHover && mHoverImage.width() > 0)
	{
		QRect rectangle(halfWidth - mHoverImage.width() / 2, halfHeight - mHoverImage.height() / 2, mHoverImage.width(), mHoverImage.height());
		painter.drawImage(rectangle, mHoverImage, mHoverImage.rect());
	}
	else if(mIsPressed)
	{
		int smallerWidth = (mSelectedImage.width() - mSelectedImage.width() * 0.02);
		int smallerHeight = (mSelectedImage.height() - mSelectedImage.height() * 0.02);
		QRect rectangle(halfWidth - smallerWidth / 2, halfHeight - smallerHeight / 2, smallerWidth, smallerHeight);
		painter.drawImage(rectangle, mSelectedImage, mSelectedImage.rect());
	}
	else if(!mIsChecked || !mIsCheckable)
	{
		QRect rectangle(halfWidth - mNormalImage.width() / 2, halfHeight - mNormalImage.height() / 2, mNormalImage.width(), mNormalImage.height());
		painter.drawImage(rectangle, mNormalImage, mNormalImage.rect());
	}
	else
	{
		QRect rectangle(halfWidth - mSelectedImage.width() / 2, halfHeight - mSelectedImage.height() / 2, mSelectedImage.width(), mSelectedImage.height());
		painter.drawImage(rectangle, mSelectedImage, mSelectedImage.rect());
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
