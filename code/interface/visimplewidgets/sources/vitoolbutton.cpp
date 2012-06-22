#include "vitoolbutton.h"

ViToolButton::ViToolButton(QWidget *parent)
	: ViAbstractButton(parent)
{
	mIsHover = false;
	mIsPressed = false;
	setSize(64, 64);
	setChecked(false);
}

void ViToolButton::setIcon(ViThemeImage image, ViThemeImage::State state)
{
	if(state == ViThemeImage::Normal)
	{
		mNormalImage = image.image();
	}
	else if(state == ViThemeImage::Selected)
	{
		mSelectedImage = image.image();
	}
	else if(state == ViThemeImage::Disabled)
	{
		mDisbaledImage = image.image();
	}
	else if(state == ViThemeImage::Hovered)
	{
		mHoveredImage = image.image();
	}
	repaint();
}

void ViToolButton::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	if(!isEnabled() && !mDisbaledImage.isNull())
	{
		painter.drawImage(QRect((width() / 2) - (mDisbaledImage.width() / 2), (height() / 2) - (mDisbaledImage.height() / 2), mDisbaledImage.width(), mDisbaledImage.height()), mDisbaledImage, mDisbaledImage.rect());
	}
	else if(mIsPressed && !mSelectedImage.isNull())
	{
		painter.drawImage(QRect((width() / 2) - (mSelectedImage.width() / 2), (height() / 2) - (mSelectedImage.height() / 2), mSelectedImage.width(), mSelectedImage.height()), mSelectedImage, mSelectedImage.rect());
	}
	else if(mIsHover && !mHoveredImage.isNull())
	{
		painter.drawImage(QRect((width() / 2) - (mHoveredImage.width() / 2), (height() / 2) - (mHoveredImage.height() / 2), mHoveredImage.width(), mHoveredImage.height()), mHoveredImage, mHoveredImage.rect());
	}
	else
	{
		painter.drawImage(QRect((width() / 2) - (mNormalImage.width() / 2), (height() / 2) - (mNormalImage.height() / 2), mNormalImage.width(), mNormalImage.height()), mNormalImage, mNormalImage.rect());
	}
}
