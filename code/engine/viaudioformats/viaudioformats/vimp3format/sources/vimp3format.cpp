#include "vimp3format.h"

ViMp3Format::ViMp3Format()
	: ViAudioFormat()
{
	mCompression = ViAudioFormat::Lossy;

	mAbbreviation = "MP3";

	mName = "Moving Picture Experts Group (MPEG) Audio Layer 3";
	
	mExtensions.append("mp3");
	mExtensions.append("bit");
}

#ifdef __cplusplus
extern "C"
{
#endif

ViAudioFormat* createFormat()
{
   return new ViMp3Format();
}

void deleteFormat(ViAudioFormat *format)
{
   delete format;
}

#ifdef __cplusplus
}
#endif
