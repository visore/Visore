#ifndef VIAUDIOOUTPUT_H
#define VIAUDIOOUTPUT_H

#include <QObject>
#include <QIODevice>
#include <QAudio>
#include <QAudioOutput>
#include <QAudioDeviceInfo>

#include "viessentials.h"
#include "viaudioinput.h"

const qint64 cBufferDurationUs = 10 * 1000000;
const int cLevelWindowUs = 0.1 * 1000000;

class ViAudioOutput : public QObject
{
    Q_OBJECT

	signals:
		void notify();
		void stateChanged(QAudio::State);

	private slots:
		virtual void stopPlayback() = 0;

	public:
		ViAudioOutput(ViAudioInput *input, QAudioDeviceInfo deviceInfo, QAudioFormat format);
		virtual void start() = 0;
		virtual void setFormat(QAudioFormat format);
		qint64 audioDuration(qint64 bytes);
		qint64 audioLength(qint64 microSeconds);
		qint64 secondsProcessed();
		virtual qint64 bufferLength() = 0; //m_file->size() when file.
		int levelBufferLength();
		QAudioFormat format();
		qint64 bufferPosition();
		void setBufferPosition(qint64 position);

	protected:
		void connectOutput();
		void disconnectOutput();

	protected:
		ViAudioInput *mAudioInput;
		QAudioOutput *mAudioOutput;
		QAudioFormat mFormat;
		QAudioDeviceInfo mDeviceInfo;
		int mLevelBufferLength;
		qint64 mBufferPosition;
		
};

#endif
