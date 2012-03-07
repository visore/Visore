#include "viwidget.h"

ViWidget::ViWidget(QWidget *parent)
	: QWidget(parent)
{
	mParent = parent;
	mEngine = NULL;
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
