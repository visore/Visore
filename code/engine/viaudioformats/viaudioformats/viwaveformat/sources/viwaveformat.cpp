#include "viwaveformat.h"

ViWaveFormat::ViWaveFormat()
	: ViAudioFormat()
{
	mCompression = ViAudioFormat::Variable;

	mAbbreviation = "WAVE";

	mName = "Waveform Audio File Format";
	
	mExtensions.append("wav");
	mExtensions.append("wave");
}

ViAudioFormat* ViWaveFormat::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViWaveFormat>(new ViWaveFormat()).dynamicCast<ViAudioFormat>();
	}
	return mInstance.data();
}


#ifdef __cplusplus
extern "C"
{
#endif

ViAudioFormat* create()
{
   return ViWaveFormat::instance();
}

#ifdef __cplusplus
}
#endif
