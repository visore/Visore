#include "viwidgettoolbar.h"

ViWidgetToolbar::ViWidgetToolbar(ViWidgetToolbar::ViWidgetToolbarAlign align, ViAudioEngine *engine, QWidget *parent)
	: ViWidget(engine, parent)
{
	mCentralWidget = new QWidget(this);
	mAlign = align;
	if(mAlign == ViWidgetToolbar::Top || mAlign == ViWidgetToolbar::Bottom)
	{
		mLayout = new QHBoxLayout(mCentralWidget);
	}
	else if(mAlign == ViWidgetToolbar::Right || mAlign == ViWidgetToolbar::Left)
	{
		mLayout = new QVBoxLayout(mCentralWidget);
	}
	setStyleSheet
	(".QWidget{\
		border-radius: 10px;\
		border-style: solid;\
		border-color: rgb(" + QString::number(ViThemeManager::color(1).red()) + ", " + QString::number(ViThemeManager::color(1).green()) + ", " + QString::number(ViThemeManager::color(1).blue()) + ");\
		border-width: 3px;\
		background: rgba(" + QString::number(ViThemeManager::color(2).red()) + ", " + QString::number(ViThemeManager::color(2).green()) + ", " + QString::number(ViThemeManager::color(2).blue()) + ", 80);\
	}");
	mLayout->setContentsMargins(10, 10, 10, 10);
	hide();
}

ViWidgetToolbar::~ViWidgetToolbar()
{
	delete mLayout;
	delete mCentralWidget;
	for(int i = 0; i < mButtons.size(); ++i)
	{
		delete mButtons[i];
	}
}

void ViWidgetToolbar::refresh()
{
	static qint8 offset = 5;
	mCentralWidget->adjustSize();
	adjustSize();
	if(mAlign == ViWidgetToolbar::Top)
	{
		move((mParent->width() / 2) - (width() / 2), offset);
	}
	else if(mAlign == ViWidgetToolbar::Bottom)
	{
		move((mParent->width() / 2) - (width() / 2), mParent->height() - height() - offset);
	}
	else if(mAlign == ViWidgetToolbar::Left)
	{
		move(offset, (mParent->height() / 2) - (height() / 2));
	}
	else if(mAlign == ViWidgetToolbar::Right)
	{
		move(mParent->width() - width() - offset, (mParent->height() / 2) - (height() / 2));
	}
}

void ViWidgetToolbar::addButton(QString text, QIcon icon)
{
cout<<icon.name().toAscii().data()<<endl;


	QToolButton *button = new QToolButton(mCentralWidget);
	button->setIcon(icon);
	button->setText(text);
	button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	mLayout->addWidget(button);
	mButtons.append(button);
	refresh();
}
