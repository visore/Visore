#ifndef VIQTSTREAMOUTPUT_H
#define VIQTSTREAMOUTPUT_H

#define POSITION_CHECK_INTERVAL 100 //Check the position every n milliseconds

#include "vistreamoutput.h"
#include <QAudioOutput>
#include <QBuffer>

class ViQtStreamOutput : public ViStreamOutput
{
	Q_OBJECT

	private slots:
		void checkPosition();

	public:

		ViQtStreamOutput();
		~ViQtStreamOutput();
		void initialize();
		void free();

		void start();
		void stop();
		void pause();

		bool setPosition(ViAudioPosition position);
		ViAudioPosition position();

		qreal volume();
		void setVolume(qreal volumeValue);
		void mute(bool value);	

	private:

		QAudioOutput *mAudioOutput;
		QBuffer *mBufferDevice;
		ViAudioPosition mOldPosition;

		qreal mMuteVolume;
		bool mIsMute;
};

#endif
