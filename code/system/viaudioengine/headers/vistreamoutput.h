#ifndef VISTREAMOUTPUT_H
#define VISTREAMOUTPUT_H

#include "viaudiooutput.h"

class ViStreamOutput : public ViAudioOutput
{
	Q_OBJECT

	private slots:
		virtual void stopPlayback();

	public:
		ViStreamOutput(ViAudioInput *input, QAudioDeviceInfo deviceInfo = QAudioDeviceInfo::defaultOutputDevice(), QAudioFormat format = QAudioFormat());
		~ViStreamOutput();
		virtual void start();
		virtual void setFormat(QAudioFormat format);
		virtual qint64 bufferLength();
		//virtual qint64 read(char *data, qint64 maxSize);
};

#endif
