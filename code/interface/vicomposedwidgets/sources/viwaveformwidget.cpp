#include "viwaveformwidget.h"

ViWaveFormWidget::ViWaveFormWidget(ViAudioEngine *engine, ViAudioBuffer::ViAudioBufferType type, QWidget *parent)
	: QWidget(parent)
{
	mWidget = new ViWaveWidget(engine, type, this);
	parent->setObjectName("parent");
	parent->setStyleSheet("\
		.QWidget#parent{\
			border-radius: 10px;\
			border-style: solid;\
			border-color: rgb(" + QString::number(ViThemeManager::color(14).red()) + ", " + QString::number(ViThemeManager::color(14).green()) + ", " + QString::number(ViThemeManager::color(14).blue()) + ");\
			border-width: 5px;\
		}\
	");
	parent->setContentsMargins(-4, -4, -4, -4);
}

ViWaveFormWidget::~ViWaveFormWidget()
{
	delete mWidget;
}
