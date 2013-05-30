#include "vitablewidget.h"
#include "vithememanager.h"
#include <QStyleOption>
#include <QPainter>
#include <QHeaderView>
#include <QHBoxLayout>

ViTableRow::ViTableRow()
{
	mCount = 0;
}

void ViTableRow::add(int value)
{
	add(QString::number(value));
}

void ViTableRow::add(double value, int decimalPlaces)
{
	add(QString::number(value, 'f', decimalPlaces));
}
		
void ViTableRow::add(QString text)
{
	mStrings[mCount] = text;
	++mCount;
}

void ViTableRow::add(QWidget *widget)
{
	mWidgets[mCount] = widget;
	++mCount;
}

int ViTableRow::count()
{
	return mCount;
}

void ViTableRow::addToTable(ViTableWidget *table)
{
	int row = table->rowCount();
	table->setRowCount(row + 1);
	for(int i = 0; i < mCount; ++i)
	{
		if(mWidgets.contains(i))
		{
			table->setItem(row, i, mWidgets[i]); 
		}
		else if(mStrings.contains(i))
		{
			table->setItem(row, i, mStrings[i]); 
		}
	}
}

ViTableWidget::ViTableWidget(QWidget *parent)
	: QTableWidget(parent)
{
	QString gradient1 = "qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 " + ViThemeManager::color(ViThemeColors::ButtonNormalColor1).name() + ", stop: 0.5 " + ViThemeManager::color(ViThemeColors::ButtonNormalColor2).name() + ", stop: 1 " + ViThemeManager::color(ViThemeColors::ButtonNormalColor1).name() + ")";

	QString gradient2 = "qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 " + ViThemeManager::color(ViThemeColors::ButtonSelectedColor1).name() + ", stop: 0.5 " + ViThemeManager::color(ViThemeColors::ButtonSelectedColor2).name() + ", stop: 1 " + ViThemeManager::color(ViThemeColors::ButtonSelectedColor1).name() + ")";

	setStyleSheet("\
		QTableView{\
			gridline-color: " + ViThemeManager::color(ViThemeColors::BorderColor2).name() + ";\
			color: " + ViThemeManager::color(ViThemeColors::ButtonTextColor2).name() + ";\
			background-color: transparent;\
		}\
		\
		QHeaderView::section{\
			background: " + gradient1 + ";\
			color: " + ViThemeManager::color(ViThemeColors::ButtonTextColor1).name() + ";\
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

	verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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

void ViTableWidget::addRow(ViTableRow *row)
{
	row->addToTable(this);
}

void ViTableWidget::addRow(QString text)
{
	ViTableRow row;
	row.add(text);
	addRow(&row);
}

void ViTableWidget::setItem(int row, int column, QString text)
{
	setItem(row, column, new QTableWidgetItem(text));
}

void ViTableWidget::setItem(int row, int column, QWidget *widget, Qt::Alignment alignment)
{
	QWidget *container = new QWidget();
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addWidget(widget, 0, alignment);
	layout->setContentsMargins(0, 0, 0, 0);
	container->setLayout(layout);
	setCellWidget(row, column, container);
}

void ViTableWidget::setItem(int row, int column, QTableWidgetItem *item)
{
	QTableWidget::setItem(row, column, item);
}

int ViTableWidget::widgetRow(QWidget *widget)
{
	for(int row = 0; row < rowCount(); ++row)
	{
		for(int column = 0; column < columnCount(); ++column)
		{
			if(widget == cellWidget(row, column))
			{
				return row;
			}
		}
	}
	return -1;
}

void ViTableWidget::removeRow(int row)
{
	QTableWidget::removeRow(row);
}

void ViTableWidget::removeRow(QWidget *widget)
{
	removeRow(widgetRow(widget));
}

void ViTableWidget::removeSelectedRows()
{
	QModelIndexList selection = selectedIndexes();
	while(!selection.isEmpty())
	{
		removeRow(selection[0].row());
		selection = selectedIndexes();
	}
}

void ViTableWidget::paintEvent(QPaintEvent *event)
{
	QTableWidget::paintEvent(event);
	QStyleOption option;
	option.init(this);
	QPainter painter(viewport());
	style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}
