#include "vitablewidget.h"
#include "vithememanager.h"
#include <QStyleOption>
#include <QPainter>

ViTableWidget::ViTableWidget(QWidget *parent)
	: QTableWidget(parent)
{
	QString gradient1 = "qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 " + ViThemeManager::color(2).name() + ", stop: 0.5 " + ViThemeManager::color(3).name() + ", stop: 1 " + ViThemeManager::color(2).name() + ")";

	QString gradient2 = "qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 " + ViThemeManager::color(13).name() + ", stop: 0.5 " + ViThemeManager::color(15).name() + ", stop: 1 " + ViThemeManager::color(13).name() + ")";

	setStyleSheet("\
		QTableView{\
			gridline-color: " + ViThemeManager::color(2).name() + ";\
			color: " + ViThemeManager::color(5).name() + ";\
			background-color: transparent;\
		}\
		\
		QHeaderView::section{\
			background: " + gradient1 + ";\
			color: " + ViThemeManager::color(13).name() + ";\
			border: 0px solid black;\
		}\
		\
		QTableCornerButton::section{\
			background: " + gradient1 + ";\
		}\
		\
		QTableWidget{\
			border: 0px solid black;\
			selection-background-color: " + gradient2 + ";\
		}\
	");

}

void ViTableWidget::paintEvent(QPaintEvent *event)
{
	QTableWidget::paintEvent(event);
	QStyleOption option;
	option.init(this);
	QPainter painter(viewport());
	style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}
