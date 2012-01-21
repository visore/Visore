#ifndef VIBASSFILEINPUT_H
#define VIBASSFILEINPUT_H

#define BUFFERSIZE 1000

#include "vifileinput.h"
#include "bass.h"
#include <termios.h>
#include <QThread>

class ViBassFileInput;

class ViBassFileInputThread : public QThread
{
	public:
		ViBassFileInputThread(ViBassFileInput *parent, ViAudioBuffer *buffer, ViAudioMetaData *metaData, QString filePath);
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

	public:
		ViBassFileInput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, QString filePath = "");
		~ViBassFileInput();
		void start();

	private:
		ViBassFileInputThread *mThread;
};

#endif

