#include "viwidget.h"
#include <QStyleOption>
#include <QPainter>

ViWidget::ViWidget(QWidget *parent)
	: QWidget(parent), ViId()
{
	mParent = parent;
	mEngine = ViAudioEngine::instance();
	mStyleSheet = "";
	setObjectName("viwidget" + id());
m="";
}

void ViWidget::refresh()
{
	repaint();
}

/*void ViWidget::setStyleSheet(QString styleSheet)
{
	mStyleSheet = styleSheet;
}

void ViWidget::addStyleSheet(QString styleSheet)
{
	mStyleSheet += styleSheet;
}

QString ViWidget::styleSheet()
{
	return mStyleSheet;
}*/

ViAudioEngine* ViWidget::engine()
{
	return mEngine;
}

void ViWidget::paintEvent(QPaintEvent *event)
{
//if(m!="")cout<<objectName().toAscii().data()<<endl;
	/*if(mStyleSheet != "")
	{
		setStyleSheet("QWidget#" + objectName() + "{" + mStyleSheet + "}\n\n" + styleSheet());
	}*/
	QStyleOption option;
	option.init(this);
	QPainter painter(this);
	style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
	QWidget::paintEvent(event);
}

void ViWidget::setSize(int width, int height)
{
	setMinimumSize(width, height);
	setMaximumSize(width, height);
}

void ViWidget::setBorder(QColor color, int thickness, int roundness)
{
	//mStyleSheet = "border-style: solid; border-color: " + color.name() + "; border-width: " + QString::number(thickness) + "px; border-radius: " + QString::number(roundness) + "px;";
	setStyleSheet("QWidget#" + objectName() + "{border-style: solid; border-color: " + color.name() + "; border-width: " + QString::number(thickness) + "px; border-radius: " + QString::number(roundness) + "px;}");
m =  objectName();
}
