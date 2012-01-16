#ifndef VISTREAMINPUT_H
#define VISTREAMINPUT_H

#include "viaudioinput.h"
#include <QAudioInput>
#include <QAudioDeviceInfo>
#include <QTimer>

class ViStreamInput : public ViAudioInput
{
	Q_OBJECT

	private slots:
		virtual void dataReady();
		virtual void stopRecording();

	public:
		ViStreamInput(QIODevice *device, QAudioDeviceInfo deviceInfo = QAudioDeviceInfo::defaultInputDevice(), QAudioFormat format = QAudioFormat());
		~ViStreamInput();
		virtual void start();
		virtual qint64 read(char *data, qint64 maxSize);

	private:
		QAudioInput *mAudioInput;

};

#endif
