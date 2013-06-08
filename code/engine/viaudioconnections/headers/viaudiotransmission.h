#ifndef VIAUDIOTRANSMISSION_H
#define VIAUDIOTRANSMISSION_H

#include <QAudio>
#include <vibuffer.h>
#include <viaudioformat.h>
#include <viaudioposition.h>
#include <vilogger.h>

class ViAudioTransmission : public QObject
{
    Q_OBJECT

	signals:

		void stateChanged(QAudio::State state);
		void started();
		void finished();
		void bufferChanged();

	public slots:

		virtual void setState(QAudio::State state);

	public:

		ViAudioTransmission();
		~ViAudioTransmission();

		virtual void setBuffer(ViBuffer *buffer);
		virtual ViBuffer* buffer();
		virtual void clear();

		virtual ViAudioFormat format();
		virtual void setFormat(ViAudioFormat format);

		virtual QAudio::State state();

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;

	protected:

		ViBuffer *mBuffer;
		ViBufferStreamPointer mStream;
		QAudio::State mState;
};

#endif
