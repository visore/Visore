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

	enum Type
	{
		Target,
		Corrupted,
		Corrected
	};

	using namespace QAudio;
}

#endif
