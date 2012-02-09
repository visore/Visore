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

ViAudioFormat* ViMp3Format::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViMp3Format>(new ViMp3Format()).dynamicCast<ViAudioFormat>();
	}
	return mInstance.data();
}

#ifdef __cplusplus
extern "C"
{
#endif

ViAudioFormat* createFormat()
{
   return ViMp3Format::instance();
}

#ifdef __cplusplus
}
#endif
