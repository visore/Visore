#ifndef VIAUDIO_H
#define VIAUDIO_H

 #include <qaudio.h>

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
