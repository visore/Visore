#include "viabstractbutton.h"

ViAbstractButton::ViAbstractButton(QWidget *parent)
	: QWidget(parent)
{
	mIsHover = false;
	mIsCheckable = false;
	mIsChecked = false;
	mIsPressed = false;
	mIsEnabled = true;
}

void ViAbstractButton::setSize(int width, int height)
{
	setMinimumSize(width, height);
	setMaximumSize(width, height);
}

void ViAbstractButton::setCheckable(bool permanentlyChecked)
{
	mIsCheckable = true;
	mIsPermantlyCheckable = permanentlyChecked;
}

void ViAbstractButton::setChecked(bool checked)
{
	mIsChecked = checked;
	repaint();
}

void ViAbstractButton::enable()
{
	mIsEnabled = true;
	repaint();
}

void ViAbstractButton::disable()
{
	mIsEnabled = false;
	repaint();
}

void ViAbstractButton::enterEvent(QEvent *event)
{
	mIsHover = true;
	repaint();
}

void ViAbstractButton::leaveEvent(QEvent *event)
{
	mIsHover = false;
	repaint();
}

void ViAbstractButton::mouseReleaseEvent(QMouseEvent *event)
{
	mIsPressed = false;
	if(mIsEnabled)
	{
		if((mIsCheckable && !mIsPermantlyCheckable) || (mIsPermantlyCheckable && !mIsChecked))
		{
			mIsChecked = !mIsChecked;
			emit clicked(mIsChecked);
		}
		else if(!mIsPermantlyCheckable)
		{
			emit clicked();
		}
	}
	repaint();
}

void ViAbstractButton::mousePressEvent(QMouseEvent *event)
{
	mIsPressed = true;
	repaint();
}
