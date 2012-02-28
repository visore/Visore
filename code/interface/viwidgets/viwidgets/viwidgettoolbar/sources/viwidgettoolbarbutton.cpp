#include "viwidgettoolbarbutton.h"

ViWidgetToolbarButton::ViWidgetToolbarButton(qint8 id, QWidget *parent)
	: QToolButton(parent)
{
	mId = id;
	setStyleSheet
	("\
		QToolButton{\
			border-radius: 7px;\
			border-style: solid;\
			border-width: 2px;\
			border-color: transparent;\
			padding: 5px;\
			background: transparent;\
		}\
		QToolButton:hover{\
			border-color: rgb(" + QString::number(ViThemeManager::color(15).red()) + ", " + QString::number(ViThemeManager::color(15).green()) + ", " + QString::number(ViThemeManager::color(15).blue()) + ");\
		}\
		QToolButton:pressed{\
			background: rgba(" + QString::number(ViThemeManager::color(15).red()) + ", " + QString::number(ViThemeManager::color(15).green()) + ", " + QString::number(ViThemeManager::color(15).blue()) + ", 80);\
		}\
	");
	ViObject::connect(this, SIGNAL(clicked()), this, SLOT(clickWithId()));
}

ViWidgetToolbarButton::~ViWidgetToolbarButton()
{
	ViObject::disconnect(this, SIGNAL(clicked()), this, SLOT(clickWithId()));
}

void ViWidgetToolbarButton::clickWithId()
{
	emit clickedWithId(mId);
}
