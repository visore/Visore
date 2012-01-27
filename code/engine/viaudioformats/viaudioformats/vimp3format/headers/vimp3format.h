#ifndef VIMP3FORMAT_H
#define VIMP3FORMAT_H

#include "viaudioformat.h"

class ViMp3Format : public ViAudioFormat
{
	public:
		static ViAudioFormat* instance();

	protected:
		ViMp3Format();
};

#endif
