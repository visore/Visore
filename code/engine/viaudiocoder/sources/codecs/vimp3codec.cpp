#include <vimp3codec.h>

void ViMp3Codec::initialize()
{
	//Names
	setShortName("MPEG Audio Layer 3");
	setLongName("Moving Picture Experts Group Audio Layer 3");

	//Compression
	setCompression(ViAudio::LossyCompression);

	//Abbreviations
	addAbbreviation("MP3");
	addAbbreviation("MPEG3");
	addAbbreviation("MPA3");
	addAbbreviation("MPEGA3");

	//Extensions
	addExtension("mp3");
	addExtension("bit");
}
