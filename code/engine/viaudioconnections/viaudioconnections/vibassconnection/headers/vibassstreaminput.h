#ifndef VIBASSSTREAMINPUT_H
#define VIBASSSTREAMINPUT_H

#define BUFFERSIZE 1000

#include "vistreaminput.h"
#include "bass.h"
#include <QThread>

class ViBassStreamInput : public ViStreamInput
{
	Q_OBJECT

	public:
		ViBassStreamInput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, ViAudioDevice *device = NULL);
		~ViBassStreamInput();
		void start();
		void stop();
		void pause();
		void initialize();

	protected:
		void free();

	private:
		HRECORD mRecordHandle;
};

#endif
