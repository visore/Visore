#include <viflaccodec.h>

void ViFlacCodec::initialize()
{
	//Names
	setShortName("Free Lossless Audio Codec");
	setLongName("Xiph.Org Foundation Free Lossless Audio Codec");

	//Compression
	setCompression(ViAudio::LosslessCompression);

	//Abbreviations
	addAbbreviation("FLAC");

	//Extensions
	addExtension("flac");
}
