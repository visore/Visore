#ifndef VIAUDIO_H
#define VIAUDIO_H

#include <QAudio>

namespace ViAudio
{
	enum Mode
	{
		AudioInput,
		AudioOutput,
		AudioInputOutput
	};

	enum Input
	{
		Line,
		File,
		Project
	};

	using namespace QAudio;
}

#endif
