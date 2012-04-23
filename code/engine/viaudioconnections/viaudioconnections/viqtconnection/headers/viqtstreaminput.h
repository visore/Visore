#ifndef VIQTSTREAMINPUT_H
#define VIQTSTREAMINPUT_H

#include "vistreaminput.h"
#include <QAudioRecorder>
#include <QAudioEncoderSettings>
#include <QAudioDeviceInfo>

class ViQtStreamInput : public ViStreamInput
{
	Q_OBJECT

	public:
		ViQtStreamInput(ViAudioFormat format, ViAudioBuffer *buffer = NULL, ViAudioDevice *device = NULL);
		~ViQtStreamInput();
		void start();
		void stop();
		void pause();
		void initialize();

	protected:
		void free();

	private:
		//QAudioRecorder *mRecorder;
};

#endif
