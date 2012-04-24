#ifndef VIAUDIOOUTPUT_H
#define VIAUDIOOUTPUT_H

#include "viaudiotransmission.h"

class ViAudioOutput : public ViAudioTransmission
{
    Q_OBJECT

	public slots:

		virtual void bufferChanged(int size) = 0;
		void changeFormat(ViAudioFormat format); // Sets the format and re-initializes the transmission

	public:

		ViAudioOutput();

		virtual void initialize();
		virtual void free();

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;

};

#endif
