#ifndef VIWAVEFORMAT_H
#define VIWAVEFORMAT_H

#include "viaudioformat.h"

class ViWaveFormat : public ViAudioFormat
{
	public:
		static ViAudioFormat* instance();

	public:
		ViWaveFormat();
};

#endif
