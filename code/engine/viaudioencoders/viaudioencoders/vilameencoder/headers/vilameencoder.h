#ifndef VILAMEENCODER_H
#define VILAMEENCODER_H

#include "viaudioencoder.h"

class ViLameEncoder : public ViAudioEncoder
{
	public:
		static ViAudioEncoder* instance();
		QString string(QString filePath);

	protected:
		ViLameEncoder();
};

#endif
