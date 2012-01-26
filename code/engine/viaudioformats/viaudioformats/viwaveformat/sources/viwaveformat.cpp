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

#ifdef __cplusplus
extern "C"
{
#endif

ViAudioFormat* createFormat()
{
   return new ViWaveFormat();
}

void deleteFormat(ViAudioFormat *format)
{
   delete format;
}

#ifdef __cplusplus
}
#endif
