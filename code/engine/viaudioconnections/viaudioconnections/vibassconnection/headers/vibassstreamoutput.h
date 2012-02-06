#ifndef VIBASSSTREAMOUTPUT_H
#define VIBASSSTREAMOUTPUT_H

#define POSITION_CHECK_INTERVAL 100 //Check the position every n milliseconds

#include "vistreamoutput.h"
#include "bass.h"
#include <QTimer>

class ViBassStreamOutput;

class ViBassStreamOutputReceiver : public QObject
{
	Q_OBJECT

	public:
		ViBassStreamOutputReceiver(ViBassStreamOutput *parent, ViAudioBufferStream *stream, HSTREAM handle);
		void changeReceived(int size);

	private:
		ViBassStreamOutput *mParent;
		ViAudioBufferStream *mStream;
		HSTREAM mHandle;
};

class ViBassStreamOutput : public ViStreamOutput
{
	Q_OBJECT

	private slots:
		void checkPosition();

	public:
		ViBassStreamOutput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, ViAudioDevice *device = NULL);
		~ViBassStreamOutput();
		void start();
		void stop();
		void pause();
		void initialize();
		qint64 setPosition(ViAudioTransmission::ViTransmissionType type, qint64 position);
		qint64 position(ViAudioTransmission::ViTransmissionType type);
		void bufferChanged(int size);

	protected:
		void free();

	private:
		HSTREAM mHandle;
		ViBassStreamOutputReceiver *mReceiver;
		double mSecondsInByte;
		QTimer *mTimer;
		qint64 mOldPosition;
};

#endif
