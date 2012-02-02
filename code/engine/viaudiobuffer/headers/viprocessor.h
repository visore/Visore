#ifndef VIPROCESSOR_H
#define VIPROCESSOR_H

#include <QObject>
#include <QThread>
#include "viaudiobuffer.h"
#include "viprocessormanager.h"

class ViProcessorThread : public QThread
{
	Q_OBJECT

	public:
		ViProcessorThread(ViAudioBuffer *buffer);
		virtual void update(int size) = 0;
		
	protected:
		ViAudioBuffer *mBuffer;
		ViAudioBufferStream *mReadStream;
		ViAudioBufferStream *mWriteStream;
};

class ViProcessor : public QObject
{
	Q_OBJECT

	public:
		ViProcessor();
		~ViProcessor();
		int id();
		void update(int size);
		virtual void initialize(ViAudioBuffer *buffer) = 0;
		
	protected:
		ViProcessorThread *mThread;
		int mId;
};

#endif
