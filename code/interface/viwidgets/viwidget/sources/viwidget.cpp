#include "viwidget.h"

ViWidget::ViWidget(ViAudioEngine *engine, QWidget *parent)
	: QWidget(parent)
{
	mParent = parent;
	mEngine = engine;
}

void ViWidget::refresh()
{
}
