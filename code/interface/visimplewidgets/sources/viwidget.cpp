#include "viwidget.h"
#include <QStyleOption>
#include <QPainter>

ViWidget::ViWidget(QWidget *parent)
	: QWidget(parent)
{
	mParent = parent;
	mEngine = ViAudioEngine::instance();
}

void ViWidget::refresh()
{
}

ViAudioEngine* ViWidget::engine()
{
	return mEngine;
}

void ViWidget::paintEvent(QPaintEvent *event)
{
	QStyleOption option;
	option.init(this);
	QPainter painter(this);
	style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
	QWidget::paintEvent(event);
}
