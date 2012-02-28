#ifndef VIWAVEFORMWIDGET_H
#define VIWAVEFORMWIDGET_H

#include "viwavewidget.h"

class ViWaveFormWidget : public QWidget
{
	public:
		ViWaveFormWidget(ViAudioEngine *engine, ViAudioBuffer::ViAudioBufferType type, QWidget *parent = 0);
		~ViWaveFormWidget();

	private:
		ViWaveWidget *mWidget;
};

#endif
