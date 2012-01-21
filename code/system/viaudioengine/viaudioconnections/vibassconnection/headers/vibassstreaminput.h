#ifndef VIBASSSTREAMINPUT_H
#define VIBASSSTREAMINPUT_H

#define BUFFERSIZE 1000

#include "vistreaminput.h"
#include "bass.h"
#include <QThread>

class ViBassStreamInput;

class ViBassStreamInputThread : public QThread
{
	public:
		ViBassStreamInputThread(ViBassStreamInput *parent, ViAudioBuffer *buffer, ViAudioMetaData *metaData, ViAudioDevice *device);
		void run();
	
	private:
		ViBassStreamInput *mParent;
		ViAudioBuffer *mBuffer;
		ViAudioMetaData *mMetaData;
		ViAudioDevice *mDevice;
};

class ViBassStreamInput : public ViStreamInput
{
	Q_OBJECT

	public:
		ViBassStreamInput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, ViAudioDevice *device = NULL);
		~ViBassStreamInput();
		void start();

	private:
		ViBassStreamInputThread *mThread;
};

#endif
