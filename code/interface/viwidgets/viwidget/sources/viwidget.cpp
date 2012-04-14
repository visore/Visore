#include "viwidget.h"

ViWidget::ViWidget(QWidget *parent)
	: QWidget(parent)
{
	mParent = parent;
	mEngine = NULL;
	mStyle = style();
}

ViWidget::~ViWidget()
{
	delete mStyle;
}

void ViWidget::refresh()
{
}

void ViWidget::setEngine(ViAudioEngine *engine)
{
	mEngine = engine;
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
