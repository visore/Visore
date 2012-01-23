#ifndef VIBASSFILEINPUT_H
#define VIBASSFILEINPUT_H

#include "vifileinput.h"
#include "bass.h"
#include <termios.h>
#include <math.h>
#include <QThread>

class ViBassFileInput;

class ViBassFileInputThread : public QThread
{
	public:
		ViBassFileInputThread(ViBassFileInput *parent, ViAudioBuffer *buffer, ViAudioMetaData *metaData, QString filePath);
		~ViBassFileInputThread();		
		void run();
		void readMetaData();
		void pause();

	private:
		int kbHit();
	
	private:
		bool mPaused;
		ViBassFileInput *mParent;
		ViAudioBuffer *mBuffer;
		QString mFilePath;
		ViAudioMetaData *mMetaData;
		HSTREAM mFileHandle;
};

class ViBassFileInput : public ViFileInput
{
	Q_OBJECT

	public:
		ViBassFileInput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, QString filePath = "");
		~ViBassFileInput();
		void start();
		void stop();
		void pause();

	private:
		ViBassFileInputThread *mThread;
};

#endif

