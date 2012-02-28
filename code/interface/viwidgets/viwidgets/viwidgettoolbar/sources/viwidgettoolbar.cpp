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
	("\
		.QWidget{\
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
	for(int i = 0; i < mButtons.size(); ++i)
	{
		ViObject::disconnect(mButtons[i], SIGNAL(clickedWithId(qint8)), this, SIGNAL(buttonClicked(qint8)));
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

void ViWidgetToolbar::addButton(QString text, QIcon icon, ViWidget *widget, const char *function)
{
	QToolButton *button = new QToolButton(mCentralWidget);
	ViObject::connect(button, SIGNAL(clicked()), widget, function);
	button->setIcon(icon);
	button->setText(text);
	button->setToolButtonStyle(Qt::ToolButtonIconOnly);
	mLayout->addWidget(button);
	mButtons.append(button);
	refresh();
}
