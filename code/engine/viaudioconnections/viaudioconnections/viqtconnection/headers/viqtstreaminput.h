#ifndef VIQTSTREAMINPUT_H
#define VIQTSTREAMINPUT_H

#include "vistreaminput.h"
#include <QAudioInput>
#include <QBuffer>

class ViQtStreamInput : public ViStreamInput
{
	Q_OBJECT

private slots:
void a();

	public:

		ViQtStreamInput();
		~ViQtStreamInput();
		void initialize();
		void free();

		void start();
		void stop();
		void pause();

		qreal volume();
		void setVolume(qreal volumeValue);
		void mute(bool value);
		
	private:

		QAudioInput *mAudioInput;
		QBuffer *mBufferDevice;
		ViAudioPosition mOldPosition;

		qreal mMuteVolume;
		bool mIsMute;

};

#endif
