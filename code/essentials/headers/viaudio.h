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
		UnknownType,
		TargetType,
		CorruptedType,
		CorrectedType,
		TemporaryType,
		TempType = TemporaryType,
		AllTypes = TargetType | CorruptedType | CorrectedType | TemporaryType
	};

	using namespace QAudio;
}

#endif
