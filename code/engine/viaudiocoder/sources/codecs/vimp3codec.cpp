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

	/*******************************************************************/
	/*******************************************************************/
	/*******************************************************************/

	//Sample Types
	addSampleSize(8, "8 bit");
	addSampleSize(16, "16 bit");
	addSampleSize(32, "32 bit");
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

	//Bitrates
	addBitrate(8, "8 kbps");
	addBitrate(16, "16 kbps");
	addBitrate(24, "24 kbps");
	addBitrate(32, "32 kbps");
	addBitrate(40, "40 kbps");
	addBitrate(48, "48 kbps");
	addBitrate(56, "56 kbps");
	addBitrate(64, "64 kbps");
	addBitrate(72, "72 kbps");
	addBitrate(80, "80 kbps");
	addBitrate(88, "88 kbps");
	addBitrate(96, "96 kbps");
	addBitrate(104, "104 kbps");
	addBitrate(112, "112 kbps");
	addBitrate(120, "120 kbps");
	addBitrate(128, "128 kbps");
	addBitrate(136, "136 kbps");
	addBitrate(144, "144 kbps");
	addBitrate(152, "152 kbps");
	addBitrate(160, "160 kbps");
	addBitrate(168, "168 kbps");
	addBitrate(176, "176 kbps");
	addBitrate(184, "184 kbps");
	addBitrate(192, "192 kbps");
	addBitrate(200, "200 kbps");
	addBitrate(208, "208 kbps");
	addBitrate(216, "216 kbps");
	addBitrate(224, "224 kbps");
	addBitrate(232, "232 kbps");
	addBitrate(240, "240 kbps");
	addBitrate(248, "248 kbps");
	addBitrate(256, "256 kbps");
	addBitrate(264, "264 kbps");
	addBitrate(272, "272 kbps");
	addBitrate(280, "280 kbps");
	addBitrate(288, "288 kbps");
	addBitrate(296, "296 kbps");
	addBitrate(304, "304 kbps");
	addBitrate(312, "312 kbps");
	addBitrate(320, "320 kbps");
	addBitrate(330, "330 kbps");
	addBitrate(340, "340 kbps");
	addBitrate(350, "350 kbps");
	addBitrate(360, "360 kbps");
	addBitrate(370, "370 kbps");
	addBitrate(380, "380 kbps");
	addBitrate(390, "390 kbps");
	addBitrate(400, "400 kbps");
	addBitrate(410, "410 kbps");
	addBitrate(420, "420 kbps");
	addBitrate(430, "430 kbps");
	addBitrate(440, "440 kbps");
	addBitrate(450, "450 kbps");
	addBitrate(460, "460 kbps");
	addBitrate(470, "470 kbps");
	addBitrate(480, "480 kbps");
	addBitrate(490, "490 kbps");
	addBitrate(500, "500 kbps");
	setDefaultBitrate(192);

	//Bitrate Modes
	addBitrateMode(ViAudioBitrate::Constant, "Constant Bitrate");
	addBitrateMode(ViAudioBitrate::Variable, "Variable Bitrate");
	addBitrateMode(ViAudioBitrate::Average, "Average Bitrate");
	setDefaultBitrateMode(ViAudioBitrate::Variable);

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
