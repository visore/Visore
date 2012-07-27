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

	//Channels
	addChannel(1, "Mono");
	addChannel(2, "Stereo");
	setDefaultChannel(2);
}
