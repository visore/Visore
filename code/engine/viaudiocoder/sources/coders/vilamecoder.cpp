#include <vilamecoder.h>
#include <vichannelconverter.h>
#include <vimp3codec.h>

#include <iostream>
using namespace std;

#define MINIMUM_HEADER_FRAMES 5

#define LAME_XING_BITRATE1 128
#define LAME_XING_BITRATE2 64
#define LAME_XING_BITRATE25 32

ViLameCoder::ViLameCoder()
	: ViAbstractCoder()
{
	mName = "Lame";
	addFileName("mp3lame");
	addFileName("lame");
	mSupportedCodecs.append(&ViMp3Codec::instance());

	mLameEncoder = NULL;
	mLameDecoder = NULL;
}

ViAudioCodec* ViLameCoder::detectCodec(const QByteArray &data)
{
	/*

	Check for MP3 frame header
	The following is in binary:
	
	MP3 sync word: 111111111111
	Version (Mpeg): 1
	Layer (3): 01
	Error protection (yes or no): 0 or 1

	Hence the total:
	Binary: 1111111111111010 or 1111111111111011
	Hex: FFFA or FFFB
	Char: 255_250 or 255_251

	*/

	//First check if file contains Xing header

	for(int i = 0; i < data.size() - 4; ++i)
	{
		if((data[i] == 'X' || data[i] == 'x') && (data[i + 1] == 'I' || data[i + 1] == 'i') && (data[i + 2] == 'N' || data[i + 2] == 'n') && (data[i + 3] == 'G' || data[i + 3] == 'g'))
		{
			return &ViMp3Codec::instance();
		}
	}

	QList<int> syncPositions;
	char sync1(255);
	char sync2(250);
	char sync3(251);

	int end = data.size() - 1;

	for(int i = 0; i < end; ++i)
	{
		if(data[i] == sync1 && (data[i + 1] == sync2 || data[i + 1] == sync3))
		{
			syncPositions.append(i);

			if(seviuentialFrames(syncPositions) >= MINIMUM_HEADER_FRAMES)
			{
				return &ViMp3Codec::instance();
			}
		}
	}
	return NULL;
}

QByteArray& ViLameCoder::header()
{
	return mHeader;
}

int ViLameCoder::headerSize()
{
	return mHeader.size();
}

bool ViLameCoder::initializeEncode()
{
	setError(ViCoder::NoError);

	mLameEncoder = m_lame_init();

	m_lame_set_in_samplerate(mLameEncoder, mInputFormat.sampleRate());
	m_lame_set_num_channels(mLameEncoder, mInputFormat.channelCount());

	if(mOutputFormat.bitrateMode() == ViAudioFormat::VariableBitrate)
	{
		m_lame_set_VBR(mLameEncoder, vbr_default);
	}
	else if(mOutputFormat.bitrateMode() == ViAudioFormat::AverageBitrate)
	{
		m_lame_set_VBR(mLameEncoder, vbr_abr);
	}
	else
	{
		m_lame_set_VBR(mLameEncoder, vbr_off);
	}
	m_lame_set_VBR_vi(mLameEncoder, mOutputFormat.viuality());
	m_lame_set_VBR_mean_bitrate_kbps(mLameEncoder, mOutputFormat.bitrate(ViAudioFormat::NormalBitrate));
	m_lame_set_VBR_min_bitrate_kbps(mLameEncoder, mOutputFormat.bitrate(ViAudioFormat::MinimumBitrate));
	m_lame_set_VBR_max_bitrate_kbps(mLameEncoder, mOutputFormat.bitrate(ViAudioFormat::MaximumBitrate));

	m_lame_set_viuality(mLameEncoder, mOutputFormat.viuality());
	m_lame_set_out_samplerate(mLameEncoder, mOutputFormat.sampleRate());
	if(mOutputFormat.channelCount() == 1)
	{
		m_lame_set_mode(mLameEncoder, MONO);
	}
	else
	{
		m_lame_set_mode(mLameEncoder, STEREO);
	}

	if(m_lame_init_params(mLameEncoder) == -1)
	{
		setError(ViCoder::EncoderInitializationError);
	}

	//Lame header is added to buffer in lame_init_params. We clear the buffer and add it to header
	int lameHeaderSize = 7200;
	unsigned char lameHeader[lameHeaderSize];
	lameHeaderSize = m_lame_encode_flush(mLameEncoder, lameHeader, lameHeaderSize);
	mHeader.clear();
	mHeader.append((char*) lameHeader, lameHeaderSize);

	int inputSampleSize = mInputFormat.sampleSize();
	int outputSampleSize = mOutputFormat.sampleSize();
	ViAudioFormat::SampleType sampleType = mInputFormat.sampleType();

	if(outputSampleSize != 16 && outputSampleSize != 32)
	{
		if(outputSampleSize < 16) outputSampleSize = 16;
		else outputSampleSize = 32;
	}

	if(sampleType == ViAudioFormat::Float)
	{
		if(mConverter.initialize(16, ViAudioFormat::Float, outputSampleSize, ViAudioFormat::SignedInt))
		{
			if(outputSampleSize == 16) encodePointer = &ViLameCoder::encode16Convert;
			else encodePointer = &ViLameCoder::encode32Convert;
		}
		else
		{
			setError(ViCoder::OutputSampleSizeError);
			return false;
		}
	}
	else if(sampleType == ViAudioFormat::Real)
	{
		if(mConverter.initialize(32, ViAudioFormat::Real, outputSampleSize, ViAudioFormat::SignedInt))
		{
			if(outputSampleSize == 16) encodePointer = &ViLameCoder::encode16Convert;
			else encodePointer = &ViLameCoder::encode32Convert;
		}
		else
		{
			setError(ViCoder::OutputSampleSizeError);
			return false;
		}
	}
	else if(sampleType == ViAudioFormat::SignedInt)
	{
		if(inputSampleSize == 8)
		{
			if(mConverter.initialize(8, ViAudioFormat::SignedInt, outputSampleSize, ViAudioFormat::SignedInt))
			{
				if(outputSampleSize == 16) encodePointer = &ViLameCoder::encode16Convert;
				else encodePointer = &ViLameCoder::encode32Convert;
			}
			else
			{
				setError(ViCoder::OutputSampleSizeError);
				return false;
			}
		}
		else if(inputSampleSize == 16)
		{
			if(outputSampleSize == 16)
			{
				encodePointer = &ViLameCoder::encode16Normal;
			}
			else if(mConverter.initialize(16, ViAudioFormat::SignedInt, outputSampleSize, ViAudioFormat::SignedInt))
			{
				encodePointer = &ViLameCoder::encode32Convert;
			}
			else
			{
				setError(ViCoder::OutputSampleSizeError);
				return false;
			}
		}
		else if(inputSampleSize == 32)
		{
			if(outputSampleSize == 32)
			{
				encodePointer = &ViLameCoder::encode32Normal;
			}
			else if(mConverter.initialize(32, ViAudioFormat::SignedInt, outputSampleSize, ViAudioFormat::SignedInt))
			{
				encodePointer = &ViLameCoder::encode16Convert;
			}
			else
			{
				setError(ViCoder::OutputSampleSizeError);
				return false;
			}
		}
	}
	else if(sampleType == ViAudioFormat::UnSignedInt)
	{
		if(inputSampleSize == 8)
		{
			if(mConverter.initialize(8, ViAudioFormat::UnSignedInt, outputSampleSize, ViAudioFormat::SignedInt))
			{
				if(outputSampleSize == 16) encodePointer = &ViLameCoder::encode16Convert;
				else encodePointer = &ViLameCoder::encode32Convert;
			}
			else
			{
				setError(ViCoder::OutputSampleSizeError);
				return false;
			}
		}
		else if(inputSampleSize == 16)
		{
			if(mConverter.initialize(16, ViAudioFormat::UnSignedInt, outputSampleSize, ViAudioFormat::SignedInt))
			{
				if(outputSampleSize == 16) encodePointer = &ViLameCoder::encode16Convert;
				else encodePointer = &ViLameCoder::encode32Convert;
			}
			else
			{
				setError(ViCoder::OutputSampleSizeError);
				return false;
			}
		}
		else if(inputSampleSize == 32)
		{
			if(mConverter.initialize(32, ViAudioFormat::UnSignedInt, outputSampleSize, ViAudioFormat::SignedInt))
			{
				if(outputSampleSize == 16) encodePointer = &ViLameCoder::encode16Convert;
				else encodePointer = &ViLameCoder::encode32Convert;
			}
			else
			{
				setError(ViCoder::OutputSampleSizeError);
				return false;
			}
		}
	}

	return true;
}

bool ViLameCoder::finalizeEncode()
{
	mHeader.clear();
	if(mLameEncoder != NULL)
	{
		int bytes = 4192;
		char *data = new char[bytes];
		int bytesWritten = m_lame_get_lametag_frame(mLameEncoder, (unsigned char*) data, bytes);
		if(bytesWritten > bytes) //Buffer (data) too small
		{
			delete [] data;
			data = new char[bytesWritten];
			bytesWritten = m_lame_get_lametag_frame(mLameEncoder, (unsigned char*) data, bytesWritten);
		}
		mHeader.append(data, bytesWritten);
		delete [] data;
	}
	if(m_lame_close(mLameEncoder) < 0)
	{
		setError(ViCoder::EncoderFinalizationError);
	}
	return true;
}

void ViLameCoder::encode(const void *input, int samples)
{
	(this->*encodePointer)(input, samples);
}

void ViLameCoder::encode16Convert(const void *input, int samples)
{
	if(samples == 0)
	{
		m_lame_encode_flush(mLameEncoder, (unsigned char*) input, samples);
	}
	else
	{
		short data[samples];
		mConverter.convert(input, data, samples);

		int bytes = samples * 2;
		qbyte *output = new qbyte[bytes];
		bytes = m_lame_encode_buffer_interleaved(mLameEncoder, data, samples / 2, output, bytes);
		if(bytes < 0)
		{
			setError(ViCoder::EncodingError);
		}
		else
		{
			emit encoded(new ViSampleArray(output, bytes, samples));
		}
	}
}

void ViLameCoder::encode32Convert(const void *input, int samples)
{
	if(samples == 0)
	{
		m_lame_encode_flush(mLameEncoder, (unsigned char*) input, samples);
	}
	else
	{
		int data[samples];
		int left[samples / 2];
		int right[samples / 2];
		mConverter.convert(input, data, samples);
		ViChannelConverter<int>::splitChannels(data, left, right, samples);

		int bytes = samples * 4;
		qbyte *output = new qbyte[bytes];
		bytes = m_lame_encode_buffer_int(mLameEncoder, left, right, samples / 2, output, bytes);
		if(bytes < 0)
		{
			setError(ViCoder::EncodingError);
		}
		else
		{
			emit encoded(new ViSampleArray(output, bytes, samples));
		}
	}
}

void ViLameCoder::encode16Normal(const void *input, int samples)
{
	if(samples == 0)
	{
		m_lame_encode_flush(mLameEncoder, (unsigned char*) input, samples);
	}
	else
	{
		int bytes = samples * 2;
		qbyte *output = new qbyte[bytes];
		bytes = m_lame_encode_buffer_interleaved(mLameEncoder, (short int*) input, samples / 2, output, bytes);
		if(bytes < 0)
		{
			setError(ViCoder::EncodingError);
		}
		else
		{
			emit encoded(new ViSampleArray(output, bytes, samples));
		}
	}
}

void ViLameCoder::encode32Normal(const void *input, int samples)
{
	if(samples == 0)
	{
		m_lame_encode_flush(mLameEncoder, (unsigned char*) input, samples);
	}
	else
	{
		int left[samples / 2];
		int right[samples / 2];
		ViChannelConverter<int>::splitChannels(input, left, right, samples);

		int bytes = samples * 4;
		qbyte *output = new qbyte[bytes];
		bytes = m_lame_encode_buffer_int(mLameEncoder, left, right, samples / 2, output, bytes);
		if(bytes < 0)
		{
			setError(ViCoder::EncodingError);
		}
		else
		{
			emit encoded(new ViSampleArray(output, bytes, samples));
		}
	}
}

bool ViLameCoder::initializeDecode()
{
	setError(ViCoder::NoError);

	mLameDecoder = m_hip_decode_init();
	mTotalBitrate = 0;
	mMinimumBitrate = INT_MAX;
	mMaximumBitrate = INT_MIN;
	mBitrateCounter = 0;

	mFormatWasDetected = false;

	return true;
}

bool ViLameCoder::finalizeDecode()
{
	m_hip_decode_exit(mLameDecoder);
	return true;
}

void ViLameCoder::decode(const void *input, int size)
{
	mp3data_struct mp3Header;
	//TODO: still don't know what size the buffers should actually be
	short left[size * 100];
	short right[size * 100];
	int samples = m_hip_decode_headers(mLameDecoder, (unsigned char*) input, size, left, right, &mp3Header);
	if(samples > 0)
	{
		mTotalBitrate += mp3Header.bitrate;
		++mBitrateCounter;
		if(mp3Header.bitrate < mMinimumBitrate)
		{
			mMinimumBitrate = mp3Header.bitrate;
			mInputFormat.setBitrate(mMinimumBitrate, ViAudioFormat::MinimumBitrate);
		}
		else if(mp3Header.bitrate > mMaximumBitrate)
		{
			mMaximumBitrate = mp3Header.bitrate;
			mInputFormat.setBitrate(mMaximumBitrate, ViAudioFormat::MaximumBitrate);
		}
		mInputFormat.setBitrate(mTotalBitrate / mBitrateCounter, ViAudioFormat::NormalBitrate);

		if(!mFormatWasDetected)
		{
			mFormatWasDetected = true;
			mInputFormat.setSampleRate(mp3Header.samplerate);
			mInputFormat.setChannelCount(mp3Header.stereo);
			mInputFormat.setSampleSize(16);
			mInputFormat.setSampleType(ViAudioFormat::SignedInt);
			mInputFormat.setCodec(&ViMp3Codec::instance());
			emit formatChanged(mInputFormat);
		}
		short *stereo = new short[samples * 2];
		samples = ViChannelConverter<short>::combineChannels(left, right, stereo, samples);
		emit decoded(new ViSampleArray(stereo, samples, samples));
	}
}

int ViLameCoder::seviuentialFrames(QList<int> positions)
{
	int end = positions.size() - 1;
	int difference = 0;
	int newDifference = 0;
	int seviuential = 0;

	for(int i = 0; i < end; ++i)
	{
		newDifference = positions[i + 1] - positions[i];
		if(newDifference == difference)
		{
			++seviuential;
		}
		//If same pattern coincidentally appears in frame, ignore it.
		//Else if position is greater than previous frame size
		else if(newDifference > difference)
		{
			difference = newDifference;
			seviuential = 0;
		}
	}
	return seviuential;
}

ViCoder::Error ViLameCoder::initializeLibrary()
{
	QList<bool> loaded;
	int success = 0;
	int failure = 0;
	
	bool version = (m_get_lame_short_version = (const char* (*)()) mLibrary.resolve("get_lame_short_version")) != NULL;
	if(version)
	{
		mVersion = QString(m_get_lame_short_version());
	}
	loaded.append(version);

	loaded.append((m_lame_get_version = (int (*)(lame_t)) mLibrary.resolve("lame_get_version")) != NULL);

loaded.append((m_InitVbrTag = (int (*)(lame_global_flags*)) mLibrary.resolve("InitVbrTag")) != NULL);

	//Decode

	loaded.append((m_hip_decode_init = (hip_t (*)()) mLibrary.resolve("hip_decode_init")) != NULL);
	loaded.append((m_hip_decode_exit = (int (*)(hip_t)) mLibrary.resolve("hip_decode_exit")) != NULL);

	loaded.append((m_hip_decode = (int (*)(hip_t, unsigned char*, int, short[], short[])) mLibrary.resolve("hip_decode")) != NULL);
	loaded.append((m_hip_decode_headers = (int (*)(hip_t, unsigned char*, int, short[], short[], mp3data_struct*)) mLibrary.resolve("hip_decode_headers")) != NULL);

	//Encode

	loaded.append((m_lame_init = (lame_t (*)()) mLibrary.resolve("lame_init")) != NULL);
	loaded.append((m_lame_init_params = (int (*)(lame_t)) mLibrary.resolve("lame_init_params")) != NULL);
	loaded.append((m_lame_close = (int (*)(lame_t)) mLibrary.resolve("lame_close")) != NULL);
	loaded.append((m_lame_get_lametag_frame = (int (*)(lame_t, unsigned char*, int)) mLibrary.resolve("lame_get_lametag_frame")) != NULL);

	loaded.append((m_lame_set_in_samplerate = (int (*)(lame_t, int)) mLibrary.resolve("lame_set_in_samplerate")) != NULL);
	loaded.append((m_lame_set_num_channels = (int (*)(lame_t, int)) mLibrary.resolve("lame_set_num_channels")) != NULL);

	loaded.append((m_lame_set_VBR = (int (*)(lame_t, vbr_mode)) mLibrary.resolve("lame_set_VBR")) != NULL);
	loaded.append((m_lame_set_VBR_vi = (int (*)(lame_t, int)) mLibrary.resolve("lame_set_VBR_vi")) != NULL);
	loaded.append((m_lame_set_VBR_mean_bitrate_kbps = (int (*)(lame_t, int)) mLibrary.resolve("lame_set_VBR_mean_bitrate_kbps")) != NULL);
	loaded.append((m_lame_set_VBR_min_bitrate_kbps = (int (*)(lame_t, int)) mLibrary.resolve("lame_set_VBR_min_bitrate_kbps")) != NULL);
	loaded.append((m_lame_set_VBR_max_bitrate_kbps = (int (*)(lame_t, int)) mLibrary.resolve("lame_set_VBR_max_bitrate_kbps")) != NULL);

	loaded.append((m_lame_set_viuality = (int (*)(lame_t, int)) mLibrary.resolve("lame_set_viuality")) != NULL);
	loaded.append((m_lame_set_out_samplerate = (int (*)(lame_t, int)) mLibrary.resolve("lame_set_out_samplerate")) != NULL);
	loaded.append((m_lame_set_mode = (int (*)(lame_t, MPEG_mode)) mLibrary.resolve("lame_set_mode")) != NULL);

	loaded.append((m_lame_encode_flush = (int (*)(lame_t, unsigned char*, int)) mLibrary.resolve("lame_encode_flush")) != NULL);
	loaded.append((m_lame_encode_buffer_interleaved = (int (*)(lame_t, short int[], int, unsigned char*, int)) mLibrary.resolve("lame_encode_buffer_interleaved")) != NULL);
	loaded.append((m_lame_encode_buffer_int = (int (*)(lame_t, int[], int[], int, unsigned char*, int)) mLibrary.resolve("lame_encode_buffer_int")) != NULL);

	for(int i = 0; i < loaded.size(); ++i)
	{
		if(loaded[i])
		{
			++success;
		}
		else
		{
			++failure;
		}
	}

	if(success == loaded.size())
	{
		return ViCoder::NoError;
	}
	else if(failure == loaded.size())
	{
		return ViCoder::LibraryFileError;
	}
	return ViCoder::LibraryVersionError;
}
