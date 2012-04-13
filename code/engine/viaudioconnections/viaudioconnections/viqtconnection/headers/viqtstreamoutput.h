#ifndef VIQTSTREAMOUTPUT_H
#define VIQTSTREAMOUTPUT_H

#define POSITION_CHECK_INTERVAL 100 //Check the position every n milliseconds

#include "vistreamoutput.h"
#include <QTimer>

class ViQtStreamOutput : public ViStreamOutput
{
	Q_OBJECT

	private slots:
		void checkPosition();

	public:
		ViQtStreamOutput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, ViAudioDevice *device = NULL);
		~ViQtStreamOutput();
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
};

#endif
