#ifndef VIAUDIO_H
#define VIAUDIO_H

namespace ViAudio
{
	enum Mode
	{
		AudioInput = 0,
		AudioOutput = 1,
		AudioInputOutput = 2
	};

	enum Input
	{
		Line = 0,
		File = 1,
		Project = 2
	};

	using namespace QAudio;
}

#endif
