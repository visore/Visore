#include "vitablewidget.h"
#include "vithememanager.h"
#include <QStyleOption>
#include <QPainter>
#include <QHeaderView>

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
		QHeaderView{\
			background-color: transparent;\
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

	verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setSelectionBehavior(QAbstractItemView::SelectRows);
}

void ViTableWidget::adjustHeight()
{
	int totalHeight = horizontalHeader()->height();
	for(int i = 0; i < rowCount(); i++)
	{
		totalHeight += rowHeight(i);
	}
	resize(width(), totalHeight);
}


void ViTableWidget::paintEvent(QPaintEvent *event)
{
	QTableWidget::paintEvent(event);
	QStyleOption option;
	option.init(this);
	QPainter painter(viewport());
	style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}
