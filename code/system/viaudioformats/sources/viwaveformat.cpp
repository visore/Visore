#include "viwaveformat.h"

ViWaveFormat::ViWaveFormat()
{
	mCompression = ViAudioFormat::Variable;

	mAbbreviation = "WAVE";

	mName = "Waveform Audio File Format";
	
	mExtensions.append("wav");
	mExtensions.append("wave");
}
