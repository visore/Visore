#ifndef VIBASSFILEOUTPUT_H
#define VIBASSFILEOUTPUT_H

#include "vifileoutput.h"
#include "bass.h"
#include "bassenc.h"

class ViBassFileOutput;

class ViBassFileOutputReceiver : public QObject
{
	Q_OBJECT

	private slots:
		void changeReceived(int startIndex, int size);

	public:
		ViBassFileOutputReceiver(ViBassFileOutput *parent, ViAudioBuffer *buffer, HSTREAM handle);

	private:
		ViBassFileOutput *mParent;
		ViAudioBuffer *mBuffer;
		HSTREAM mHandle;
};

class ViBassFileOutput : public ViFileOutput
{
	Q_OBJECT

	public:
		ViBassFileOutput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, QString filePath = "");
		~ViBassFileOutput();
		void start();
		void stop();
		void pause();

	private:
		HSTREAM mHandle;
		ViBassFileOutputReceiver *mReceiver;
};

#endif
