#ifndef VIBASSSTREAMOUTPUT_H
#define VIBASSSTREAMOUTPUT_H

#include "vistreamoutput.h"
#include "bass.h"

class ViBassStreamOutput : public ViStreamOutput
{
	Q_OBJECT

	private slots:
		void changeReceived(int startIndex, int size);

	public:
		ViBassStreamOutput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, ViAudioDevice *device = NULL);
		~ViBassStreamOutput();
		void start();

	private:
		HSTREAM mHandle;
};

#endif
