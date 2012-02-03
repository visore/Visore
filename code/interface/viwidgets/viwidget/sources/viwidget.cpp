#include "viwidget.h"

ViWidget::ViWidget(ViAudioEngine *engine, QWidget *parent)
	: QWidget(parent)
{
	mEngine = engine;
}
