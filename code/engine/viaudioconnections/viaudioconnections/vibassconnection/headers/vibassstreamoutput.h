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
		bool setPosition(ViAudioPosition position);
		ViAudioPosition position();
		void bufferChanged(int size);
		qreal volume();
		void setVolume(qreal volume);
		void mute(bool value);

	protected:
		void free();

	private:
		HSTREAM mHandle;
		ViBassStreamOutputReceiver *mReceiver;
		double mSecondsInByte;
		QTimer *mTimer;
		ViAudioPosition mOldPosition;
		qreal mPreMuteVolume;
		bool mIsMute;
};

#endif
