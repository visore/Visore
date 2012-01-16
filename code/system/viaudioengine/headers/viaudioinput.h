#ifndef VIAUDIOINPUT_H
#define VIAUDIOINPUT_H

#include <QObject>
#include <QIODevice>
#include <QAudioFormat>
#include "viessentials.h"

class ViAudioInput : public QObject
{
    Q_OBJECT

	private slots:
		//virtual void dataReady() = 0;
		//virtual void stopRecording() = 0;

	public:
		//virtual void start() = 0;
		//virtual qint64 read(char *data, qint64 maxSize) = 0;
		ViAudioInput(QAudioFormat format = QAudioFormat());
		~ViAudioInput();
		QIODevice* device();
		QAudioFormat format();
		qint64 size();

	protected:
		QIODevice *mDevice;
		QAudioFormat mFormat;
		
};

#endif
