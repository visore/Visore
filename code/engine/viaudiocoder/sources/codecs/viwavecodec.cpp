#include <viwavecodec.h>

void ViWaveCodec::initialize()
{
	//Names
	setShortName("Waveform");
	setLongName("Resource Interchange File Format Waveform");

	//Compression
	setCompression(ViAudio::LosslessCompression);

	//Abbreviations
	addAbbreviation("WAVE");
	addAbbreviation("WAV");
	addAbbreviation("RIFF");
	addAbbreviation("RIFFWAV");
	addAbbreviation("RIFFWAVE");

	//Extensions
	addExtension("wav");
	addExtension("wave");
	addExtension("riff");
}
