#include "viwidget.h"

ViWidget::ViWidget(QWidget *parent)
	: QWidget(parent)
{
	mParent = parent;
	mEngine = ViAudioEngine::instance();
	mStyle = style();
}

void ViWidget::refresh()
{
}

ViAudioEngine* ViWidget::engine()
{
	return mEngine;
}

void ViWidget::setStyleSheet(QString styleSheet)
{
	QWidget::setStyleSheet(styleSheet);
	mStyle = style();
}

void ViWidget::paintEvent(QPaintEvent *event)
{
	QStyleOption option;
	option.init(this);
	QPainter painter(this);
	mStyle->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
	QWidget::paintEvent(event);
}
