#ifndef VIAUDIOTRANSMISSION_H
#define VIAUDIOTRANSMISSION_H

#include <qaudio.h>
#include "viaudiobuffer.h"
#include "viaudioformat.h"
#include "viaudioposition.h"
#include "vilogger.h"

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

		virtual void setBuffer(ViAudioBuffer *buffer);
		virtual ViAudioBuffer* buffer();

		virtual ViAudioFormat format();
		virtual void setFormat(ViAudioFormat format);

		virtual QAudio::State state();

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;

	protected:

		ViAudioBuffer *mBuffer;
		ViAudioBufferStream *mStream;
		QAudio::State mState;
};

#endif
