#include "viwidgettoolbar.h"

ViWidgetToolbar::ViWidgetToolbar(Qt::Alignment align, QWidget *parent)
	: ViWidget(parent)
{
	mCentralWidget = new QWidget(this);
	mAlign = align;
	if(mAlign & Qt::AlignTop || mAlign & Qt::AlignBottom)
	{
		mLayout = new QHBoxLayout(mCentralWidget);
	}
	else if(mAlign & Qt::AlignRight || mAlign & Qt::AlignLeft || mAlign & Qt::AlignCenter)
	{
		mLayout = new QVBoxLayout(mCentralWidget);
	}

	setObjectName("toolbar");
	setStyleSheet
	("\
		QWidget#toolbar{\
			border-radius: 10px;\
			border-style: solid;\
			border-color: rgb(" + QString::number(ViThemeManager::color(1).red()) + ", " + QString::number(ViThemeManager::color(1).green()) + ", " + QString::number(ViThemeManager::color(1).blue()) + ");\
			border-width: 3px;\
			background: rgba(" + QString::number(ViThemeManager::color(1).red()) + ", " + QString::number(ViThemeManager::color(1).green()) + ", " + QString::number(ViThemeManager::color(1).blue()) + ", 175);\
		}\
		.QToolButton{\
			border-radius: 7px;\
			border-style: solid;\
			border-width: 2px;\
			border-color: transparent;\
			padding: 5px;\
			background: transparent;\
		}\
		.QToolButton:hover{\
			border-color: rgb(" + QString::number(ViThemeManager::color(15).red()) + ", " + QString::number(ViThemeManager::color(15).green()) + ", " + QString::number(ViThemeManager::color(15).blue()) + ");\
		}\
		.QToolButton:pressed{\
			background: rgba(" + QString::number(ViThemeManager::color(15).red()) + ", " + QString::number(ViThemeManager::color(15).green()) + ", " + QString::number(ViThemeManager::color(15).blue()) + ", 80);\
		}\
	");
	mLayout->setContentsMargins(5, 5, 5, 5);
	hide();
}

ViWidgetToolbar::~ViWidgetToolbar()
{
	delete mLayout;
	delete mCentralWidget;
}

void ViWidgetToolbar::refresh()
{
	static qint8 offset = 5;
	mCentralWidget->adjustSize();
	adjustSize();

	if(mAlign & Qt::AlignLeft)
	{
		move(offset, pos().y());
	}
	else if(mAlign & Qt::AlignRight)
	{
		move(mParent->width() - width() - offset, pos().y());
	}
	else if(mAlign & Qt::AlignCenter)
	{
		move((mParent->width() / 2) - (width() / 2), pos().y());
	}

	if(mAlign & Qt::AlignTop)
	{
		move(pos().x(), offset);
	}
	else if(mAlign & Qt::AlignBottom)
	{
		move(pos().x(), mParent->height() - height() - offset);
	}
	else if(mAlign & Qt::AlignCenter)
	{
		move(pos().x(), (mParent->height() / 2) - (height() / 2));
	}
}

void ViWidgetToolbar::addWidget(QWidget *widget)
{
	mLayout->addWidget(widget);
	refresh();
}

void ViWidgetToolbar::show()
{
	refresh();
	ViWidget::show();
}

void ViWidgetToolbar::hide()
{
	refresh();
	ViWidget::hide();
}

void ViWidgetToolbar::setVisible(bool visible)
{
	refresh();
	ViWidget::setVisible(visible);
}
