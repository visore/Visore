#include "vimp3format.h"

ViMp3Format::ViMp3Format()
{
	mCompression = ViAudioFormat::Lossy;

	mAbbreviation = "MP3";

	mName = "Moving Picture Experts Group (MPEG) Audio Layer 3";
	
	mExtensions.append("mp3");
	mExtensions.append("bit");
}
