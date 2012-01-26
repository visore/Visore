#ifndef VIBASSSTREAMOUTPUT_H
#define VIBASSSTREAMOUTPUT_H

#include "vistreamoutput.h"
#include "bass.h"

class ViBassStreamOutput;

class ViBassStreamOutputReceiver : public QObject
{
	Q_OBJECT

	private slots:
		void changeReceived(int startIndex, int size);

	public:
		ViBassStreamOutputReceiver(ViBassStreamOutput *parent, ViAudioBuffer *buffer, HSTREAM handle);

	private:
		ViBassStreamOutput *mParent;
		ViAudioBuffer *mBuffer;
		HSTREAM mHandle;
};

class ViBassStreamOutput : public ViStreamOutput
{
	public:
		ViBassStreamOutput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, ViAudioDevice *device = NULL);
		~ViBassStreamOutput();
		void start();
		void stop();
		void pause();
		qint64 setPosition(ViAudioTransmission::ViTransmissionType type, qint64 position);
		qint64 position(ViAudioTransmission::ViTransmissionType type);

	private:
		HSTREAM mHandle;
		ViBassStreamOutputReceiver *mReceiver;
		double mSecondsInByte;
};

#endif
