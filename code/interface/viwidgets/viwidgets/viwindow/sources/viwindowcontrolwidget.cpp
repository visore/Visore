#include "viwindowcontrolwidget.h"

ViWindowControlWidget::ViWindowControlWidget(QWidget *parent)
	: QWidget(parent)
{
	mType = ViWindowControlWidget::Top;
	mParent = NULL;
	mIsSelected = false;
}

void ViWindowControlWidget::setParameters(QWidget *parent, ViWindowControlWidget::ViWindowControlType type, int size)
{
	setMainParent(parent);
	setResizeType(type);
	setSize(size);
}

void ViWindowControlWidget::setResizeType(ViWindowControlWidget::ViWindowControlType type)
{
	mType = type;
	if(mType == ViWindowControlWidget::TopLeft || mType == ViWindowControlWidget::BottomRight)
	{
		setCursor(Qt::SizeFDiagCursor);
	}
	else if(mType == ViWindowControlWidget::TopRight || mType == ViWindowControlWidget::BottomLeft)
	{
		setCursor(Qt::SizeBDiagCursor);
	}
	else if(mType == ViWindowControlWidget::Top || mType == ViWindowControlWidget::Bottom)
	{
		setCursor(Qt::SizeVerCursor);
	}
	else if(mType == ViWindowControlWidget::Right || mType == ViWindowControlWidget::Left)
	{
		setCursor(Qt::SizeHorCursor);
	}
	else if(mType == ViWindowControlWidget::Move)
	{
		setCursor(Qt::OpenHandCursor);
	}
}

void ViWindowControlWidget::setMainParent(QWidget *parent)
{
	mParent = parent;
}

void ViWindowControlWidget::setSize(int size)
{
	if(mType == ViWindowControlWidget::Top
		|| mType == ViWindowControlWidget::TopLeft
		|| mType == ViWindowControlWidget::TopRight
		|| mType == ViWindowControlWidget::Bottom
		|| mType == ViWindowControlWidget::BottomLeft
		|| mType == ViWindowControlWidget::BottomRight
		|| mType == ViWindowControlWidget::Move)
	{
		setMinimumHeight(size);
		setMaximumHeight(size);
	}
	if(mType == ViWindowControlWidget::Right
		|| mType == ViWindowControlWidget::TopLeft
		|| mType == ViWindowControlWidget::TopRight
		|| mType == ViWindowControlWidget::Left
		|| mType == ViWindowControlWidget::BottomLeft
		|| mType == ViWindowControlWidget::BottomRight)
	{
		setMinimumWidth(size);
		setMaximumWidth(size);
	}
}

void ViWindowControlWidget::mousePressEvent(QMouseEvent *event)
{
	mIsSelected = true;
	mOldPosition = event->globalPos();
	if(mType == ViWindowControlWidget::Move)
	{
		setCursor(Qt::ClosedHandCursor);
	}
}

void ViWindowControlWidget::mouseReleaseEvent(QMouseEvent *event)
{
	mIsSelected = false;
	if(mType == ViWindowControlWidget::Move)
	{
		setCursor(Qt::OpenHandCursor);
	}
}

void ViWindowControlWidget::mouseMoveEvent(QMouseEvent *event)
{
	if(mIsSelected)
	{
		QPoint newPosition = event->globalPos();
		int xDifference = newPosition.x() - mOldPosition.x();
		int yDifference = newPosition.y() - mOldPosition.y();

		if(mType == ViWindowControlWidget::TopLeft || mType == ViWindowControlWidget::BottomLeft || mType == ViWindowControlWidget::Left)
		{
			mParent->move(mParent->pos().x() + xDifference, mParent->pos().y());
		}
		if(mType == ViWindowControlWidget::TopLeft || mType == ViWindowControlWidget::TopRight || mType == ViWindowControlWidget::Top)
		{
			mParent->move(mParent->pos().x(), mParent->pos().y() + yDifference);
		}
		else if(mType == ViWindowControlWidget::Move)
		{
			mParent->move(mParent->pos().x() + xDifference, mParent->pos().y() + yDifference);
			mOldPosition = newPosition;
		}

		if(mType == ViWindowControlWidget::Top || mType == ViWindowControlWidget::TopLeft || mType == ViWindowControlWidget::TopRight)
		{
			mParent->resize(mParent->width(), mParent->height() - yDifference);
			mOldPosition = newPosition;
		}
		if(mType == ViWindowControlWidget::Bottom || mType == ViWindowControlWidget::BottomLeft || mType == ViWindowControlWidget::BottomRight)
		{
			mParent->resize(mParent->width(), mParent->height() + yDifference);
			mOldPosition = newPosition;
		}
		if(mType == ViWindowControlWidget::TopLeft || mType == ViWindowControlWidget::Left || mType == ViWindowControlWidget::BottomLeft)
		{
			mParent->resize(mParent->width() - xDifference, mParent->height());
			mOldPosition = newPosition;
		}
		if(mType == ViWindowControlWidget::Right || mType == ViWindowControlWidget::TopRight || mType == ViWindowControlWidget::BottomRight)
		{
			mParent->resize(mParent->width() + xDifference, mParent->height());
			mOldPosition = newPosition;
		}
		mParent->repaint();
	}
}
