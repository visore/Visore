#ifndef VIAUDIOTRANSMISSION_H
#define VIAUDIOTRANSMISSION_H

#include <qaudio.h>
#include "viaudiobuffer.h"
#include "vierror.h"
#include "viaudioformat.h"
#include "viaudioposition.h"

class ViAudioTransmission : public QObject, public ViError
{
    Q_OBJECT

	public:

		ViAudioTransmission();
		~ViAudioTransmission();

		virtual void initialize() = 0;
		virtual void free() = 0;

		virtual void setBuffer(ViAudioBuffer *buffer);
		virtual ViAudioBuffer* buffer();

		virtual ViAudioFormat format();
		virtual void setFormat(ViAudioFormat format);

		virtual void setState(QAudio::State state);
		virtual QAudio::State state();

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;

	protected:

		ViAudioBuffer *mBuffer;
		ViAudioBufferStream *mStream;
		ViAudioFormat mFormat;
		QAudio::State mState;
};

#endif
