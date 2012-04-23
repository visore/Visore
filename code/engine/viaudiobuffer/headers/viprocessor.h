#ifndef VIPROCESSOR_H
#define VIPROCESSOR_H

#include <QObject>
#include <QMutex>
#include <QThread>
#include "viaudiobuffer.h"
#include "viprocessormanager.h"
#include "viobject.h"

class ViProcessorThread : public QThread
{
	Q_OBJECT

	signals:
		void changed();

	public:
		ViProcessorThread(ViAudioBuffer *buffer);
		virtual void update(int size);
		virtual void run() = 0;
		
	protected:
		ViAudioBuffer *mBuffer;
		QList<int> mSizes;
		QMutex mSizesMutex;
		ViAudioBufferStream *mReadStream;
		ViAudioBufferStream *mWriteStream;
};

class ViProcessor : public QObject
{
	Q_OBJECT

	signals:
		void changed();

	public:
		ViProcessor();
		~ViProcessor();
		int id();
		virtual void initialize(ViAudioBuffer *buffer);
		virtual void update(int size);
		
	protected:
		ViProcessorThread *mThread;
		int mId;
};

#endif
