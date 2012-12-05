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

	/*******************************************************************/
	/*******************************************************************/
	/*******************************************************************/

	//Sample Types
	addSampleSize(8, "8 bit");
	addSampleSize(16, "16 bit");
	setDefaultSampleSize(16);

	//Sample Rates
	addSampleRate(8000, "8000 Hz");
	addSampleRate(11025, "11025 Hz");
	addSampleRate(16000, "16000 Hz");
	addSampleRate(22050, "22050 Hz");
	addSampleRate(32000, "32000 Hz");
	addSampleRate(44056, "44056 Hz");
	addSampleRate(44100, "44100 Hz");
	addSampleRate(47250, "47250 Hz");
	addSampleRate(48000, "48000 Hz");
	addSampleRate(50000, "50000 Hz");
	addSampleRate(50400, "50400 Hz");
	addSampleRate(88200, "88200 Hz");
	addSampleRate(96000, "96000 Hz");
	setDefaultSampleRate(44100);

	//Sample Types
	addSampleType(ViAudioFormat::SignedInt, "Signed Integer");
	setDefaultSampleType(ViAudioFormat::SignedInt);

	//Endianness
	addEndianness(ViAudioFormat::LittleEndian, "Little Endian");
	setDefaultEndianness(ViAudioFormat::LittleEndian);

	//Qualities
	addQuality(ViAudioFormat::InsaneHigh, "Insane High");
	addQuality(ViAudioFormat::ExtremeHigh, "Extreme High");
	addQuality(ViAudioFormat::VeryHigh, "Very High");
	addQuality(ViAudioFormat::High, "High");
	addQuality(ViAudioFormat::AboveAverage, "Above Average");
	addQuality(ViAudioFormat::Average, "Average");
	addQuality(ViAudioFormat::BelowAverage, "Below Average");
	addQuality(ViAudioFormat::Low, "Low");
	addQuality(ViAudioFormat::VeryLow, "Very Low");
	addQuality(ViAudioFormat::ExtremeLow, "Extreme Low");
	setDefaultQuality(ViAudioFormat::High);

	//Channels
	addChannel(1, "Mono");
	addChannel(2, "Stereo");
	setDefaultChannel(2);
}
