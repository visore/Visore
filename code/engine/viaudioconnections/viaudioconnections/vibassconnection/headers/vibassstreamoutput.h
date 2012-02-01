#ifndef VIBASSSTREAMOUTPUT_H
#define VIBASSSTREAMOUTPUT_H

#include "vistreamoutput.h"
#include "bass.h"

class ViBassStreamOutput;

class ViBassStreamOutputReceiver : public QObject
{
	Q_OBJECT

	public:
		ViBassStreamOutputReceiver(ViBassStreamOutput *parent, ViAudioBufferStream *stream, HSTREAM handle);
		void changeReceived(int startIndex, int size);

	private:
		ViBassStreamOutput *mParent;
		ViAudioBufferStream *mStream;
		HSTREAM mHandle;
};

class ViBassStreamOutput : public ViStreamOutput
{
	Q_OBJECT

	public:
		ViBassStreamOutput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, ViAudioDevice *device = NULL);
		~ViBassStreamOutput();
		void start();
		void stop();
		void pause();
		qint64 setPosition(ViAudioTransmission::ViTransmissionType type, qint64 position);
		qint64 position(ViAudioTransmission::ViTransmissionType type);
		void bufferChanged(int startIndex, int size);

	private:
		HSTREAM mHandle;
		ViBassStreamOutputReceiver *mReceiver;
		double mSecondsInByte;
};

#endif
