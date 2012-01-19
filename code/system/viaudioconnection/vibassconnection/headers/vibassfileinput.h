#ifndef VIBASSFILEINPUT_H
#define VIBASSFILEINPUT_H

#define BUFFERSIZE 1000

#include "viaudiometadata.h"
#include "vifileinput.h"
#include "bass.h"
#include <termios.h>
#include <QThread>

#include <QMutex>

class ViBassFileInput;

class ViBassFileInputThread : public QThread
{
	public:
		ViBassFileInputThread(ViBassFileInput *parent, ViAudioBuffer *buffer, QString filePath, ViAudioMetaData *metaData);
		void run();

	private:
		int kbHit();
		void readMetaData(DWORD handle);
	
	private:
		ViBassFileInput *mParent;
		ViAudioBuffer *mBuffer;
		QString mFilePath;
		ViAudioMetaData *mMetaData;
};

class ViBassFileInput : public ViFileInput
{
	Q_OBJECT

	private slots:
		void ha(ViAudioBufferChunk* b, int size);

	public:
		ViBassFileInput();
		~ViBassFileInput();
		void start();

	private:
		ViBassFileInputThread *mThread;
		ViAudioMetaData mMetaData;
HSTREAM hh;
QMutex mm;
bool bb;
};

#endif

