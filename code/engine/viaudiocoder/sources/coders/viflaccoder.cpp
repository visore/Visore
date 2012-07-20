#include <viflaccoder.h>
#include <viflaccodec.h>
#include <vichannelconverter.h>

ViFlacCoder::ViFlacCoder()
	: ViAbstractCoder(), QThread()
{
	mName = "Flac";
	addFileName("FLAC");
	addFileName("flac");
	addFileName("Flac");
	mSupportedCodecs.append(&ViFlacCodec::instance());

	mEncoder = NULL;
	mDecoder = NULL;
}

ViFlacCoder::~ViFlacCoder()
{
	if(mEncoder != NULL)
	{
		m_FLAC__stream_encoder_delete(mEncoder);
		mEncoder = NULL;
	}
	if(mDecoder != NULL)
	{
		m_FLAC__stream_decoder_delete(mDecoder);
		mDecoder = NULL;
	}
}

ViAudioCodec* ViFlacCoder::detectCodec(const QByteArray &data)
{
	if(	data.size() >= 4 &&
		(data[0] == 'f' || data[0] == 'F') &&
		(data[1] == 'l' || data[1] == 'L') &&
		(data[2] == 'a' || data[2] == 'A') &&
		(data[3] == 'c' || data[3] == 'C'))
	{
		return &ViFlacCodec::instance();
	}
	return NULL;
}

QByteArray& ViFlacCoder::header()
{
	return mHeader;
}

int ViFlacCoder::headerSize()
{
	return mHeader.size();
}

bool ViFlacCoder::initializeEncode()
{
	setError(ViCoder::NoError);

	mHeader.clear();
	mHeaderPosition = 0;

	int inputSampleSize = mInputFormat.sampleSize();
	int outputSampleSize = mOutputFormat.sampleSize();
	ViAudioFormat::SampleType sampleType = mInputFormat.sampleType();

	if(sampleType == ViAudioFormat::SignedInt)
	{
		if(inputSampleSize == outputSampleSize)
		{
			if(inputSampleSize == 8) encodePointer = &ViFlacCoder::encode8Normal;
			else if(inputSampleSize == 16) encodePointer = &ViFlacCoder::encode16Normal;
			else if(inputSampleSize == 32) encodePointer = &ViFlacCoder::encode32Normal;
			else
			{
				setError(ViCoder::OutputSampleSizeError);
				return false;
			}
		}
	}
	else
	{
		if(mConverter.initialize(inputSampleSize, sampleType, outputSampleSize, ViAudioFormat::SignedInt))
		{
			if(outputSampleSize == 8) encodePointer = &ViFlacCoder::encode8Convert;
			else if(outputSampleSize == 16) encodePointer = &ViFlacCoder::encode16Convert;
			else if(outputSampleSize == 32) encodePointer = &ViFlacCoder::encode32Convert;
			else
			{
				setError(ViCoder::OutputSampleSizeError);
				return false;
			}
		}
		else
		{
			setError(ViCoder::OutputSampleSizeError);
			return false;
		}
	}

	if(mEncoder != NULL)
	{
		m_FLAC__stream_encoder_delete(mEncoder);
	}
	if((mEncoder = m_FLAC__stream_encoder_new()) == NULL)
	{
		setError(ViCoder::EncoderInitializationError);
		return false;
	}

	FLAC__bool ok = true;
	ok &= m_FLAC__stream_encoder_set_verify(mEncoder, true);
	ok &= m_FLAC__stream_encoder_set_compression_level(mEncoder, ViAudioFormat::Minimum - mOutputFormat.viuality());
	ok &= m_FLAC__stream_encoder_set_channels(mEncoder, mOutputFormat.channelCount());
	ok &= m_FLAC__stream_encoder_set_bits_per_sample(mEncoder, mOutputFormat.sampleSize());
	ok &= m_FLAC__stream_encoder_set_sample_rate(mEncoder, mOutputFormat.sampleRate());

	if(ok)
	{
		flacWriteEncodePointer = &ViFlacCoder::flacWriteEncodeHeader;
		FLAC__StreamEncoderInitStatus initStatus = m_FLAC__stream_encoder_init_stream(mEncoder, flacWriteEncode, flacSeekEncode, flacTellEncode, NULL, this);
		if(initStatus == FLAC__STREAM_ENCODER_INIT_STATUS_INVALID_NUMBER_OF_CHANNELS)
		{
			setError(ViCoder::OutputChannelError);
		}
		else if(initStatus == FLAC__STREAM_ENCODER_INIT_STATUS_INVALID_BITS_PER_SAMPLE)
		{
			setError(ViCoder::OutputSampleSizeError);
		}
		else if(initStatus == FLAC__STREAM_ENCODER_INIT_STATUS_INVALID_SAMPLE_RATE)
		{
			setError(ViCoder::OutputSampleRateError);
		}
		else if(initStatus != FLAC__STREAM_ENCODER_INIT_STATUS_OK)
		{
			ok = false;
		}
		flacWriteEncodePointer = &ViFlacCoder::flacWriteEncodeData;
	}
	if(ok)
	{
		return true;
	}
	return false;
}

bool ViFlacCoder::finalizeEncode()
{
	if(mEncoder != NULL)
	{
		if(!m_FLAC__stream_encoder_finish(mEncoder))
		{
			setError(ViCoder::EncoderFinalizationError);
		}
		m_FLAC__stream_encoder_delete(mEncoder);
		mEncoder = NULL;
		return true;
	}
	return false;
}

void ViFlacCoder::encode(const void *input, int samples)
{
	(this->*encodePointer)(input, samples);
}

void ViFlacCoder::encode8Convert(const void *input, int samples)
{
	qbyte8s data[samples];
	mConverter.convert(input, data, samples);
	FLAC__int32 inputData[samples];
	for(int i = 0; i < samples; ++i)
	{
		inputData[i] = data[i];
	}
	if(!m_FLAC__stream_encoder_process_interleaved(mEncoder, inputData, samples / mInputFormat.channels()))
	{
		setError(ViCoder::EncodingError);
	}
}

void ViFlacCoder::encode16Convert(const void *input, int samples)
{
	qbyte16s data[samples];
	mConverter.convert(input, data, samples);
	FLAC__int32 inputData[samples];
	for(int i = 0; i < samples; ++i)
	{
		inputData[i] = data[i];
	}
	if(!m_FLAC__stream_encoder_process_interleaved(mEncoder, inputData, samples / mInputFormat.channels()))
	{
		setError(ViCoder::EncodingError);
	}
}

void ViFlacCoder::encode32Convert(const void *input, int samples)
{
	qbyte32s data[samples];
	mConverter.convert(input, data, samples);
	FLAC__int32 inputData[samples];
	for(int i = 0; i < samples; ++i)
	{
		inputData[i] = data[i];
	}
	if(!m_FLAC__stream_encoder_process_interleaved(mEncoder, inputData, samples / mInputFormat.channels()))
	{
		setError(ViCoder::EncodingError);
	}
}

void ViFlacCoder::encode8Normal(const void *input, int samples)
{
	qbyte8s *data = (qbyte8s*) input;
	FLAC__int32 inputData[samples];
	for(int i = 0; i < samples; ++i)
	{
		inputData[i] = data[i];
	}
	if(!m_FLAC__stream_encoder_process_interleaved(mEncoder, inputData, samples / mInputFormat.channels()))
	{
		setError(ViCoder::EncodingError);
	}
}

void ViFlacCoder::encode16Normal(const void *input, int samples)
{
	qbyte16s *data = (qbyte16s*) input;
	FLAC__int32 inputData[samples];
	for(int i = 0; i < samples; ++i)
	{
		inputData[i] = data[i];
	}
	if(!m_FLAC__stream_encoder_process_interleaved(mEncoder, inputData, samples / mInputFormat.channels()))
	{
		setError(ViCoder::EncodingError);
	}
}

void ViFlacCoder::encode32Normal(const void *input, int samples)
{
	qbyte32s *data = (qbyte32s*) input;
	FLAC__int32 inputData[samples];
	for(int i = 0; i < samples; ++i)
	{
		inputData[i] = data[i];
	}
	if(!m_FLAC__stream_encoder_process_interleaved(mEncoder, inputData, samples / mInputFormat.channels()))
	{
		setError(ViCoder::EncodingError);
	}
}

bool ViFlacCoder::initializeDecode()
{
	mIsPaused = false;
	mAtEnd = false;

	if(mDecoder != NULL)
	{
		m_FLAC__stream_decoder_delete(mDecoder);
	}
	if((mDecoder = m_FLAC__stream_decoder_new()) == NULL)
	{
		setError(ViCoder::DecoderInitializationError);
		return false;
	}

	if(m_FLAC__stream_decoder_init_stream(mDecoder, flacReadDecode, NULL, NULL, NULL, NULL, flacWriteDecode, flacMetadataDecode, flacErrorDecode, this) != FLAC__STREAM_DECODER_INIT_STATUS_OK)
	{
		setError(ViCoder::DecoderInitializationError);
		return false;
	}

	return true;
}

bool ViFlacCoder::finalizeDecode()
{
	mAtEnd = true;
	if(isPaused())
	{
		resume();
	}
	else if(!isRunning())
	{
		start();
	}
	while(isRunning());

	if(mDecoder != NULL)
	{
		if(!m_FLAC__stream_decoder_finish(mDecoder))
		{
			setError(ViCoder::DecoderFinalizationError);
		}
		m_FLAC__stream_decoder_delete(mDecoder);
		mDecoder = NULL;
		return true;
	}
	return false;
}

void ViFlacCoder::decode(const void *input, int size)
{
	mMutex.lock();
	mData.append((char*)input, size);
	mMutex.unlock();
	if(isRunning() && !isPaused())
	{
		mWaiter.lock();
		mWaitCondition.wait(&mWaiter);
		mWaiter.unlock();
	}
	if(isPaused())
	{
		resume();
	}
	else if(!isRunning())
	{
		start();
	}
}

void ViFlacCoder::run()
{
	if(!m_FLAC__stream_decoder_process_until_end_of_stream(mDecoder))
	{
		setError(ViCoder::DecodingError);
	}
	mWaitCondition.wakeAll();
}

void ViFlacCoder::pause()
{
	mPauser.lock();
	mIsPaused = true;
	mPauser.unlock();
}

void ViFlacCoder::resume()
{
	mPauser.lock();
	mIsPaused = false;
	mPauser.unlock();
	mPauseCondition.wakeAll();
}

void ViFlacCoder::wait()
{
	mPauser.lock();
	if(mIsPaused)
	{
		mWaitCondition.wakeAll();
		mPauseCondition.wait(&mPauser);
	}
	mPauser.unlock();
}

int ViFlacCoder::dataSize()
{
	mMutex.lock();
	int size = mData.size();
	mMutex.unlock();
	return size;
}

bool ViFlacCoder::isPaused()
{
	mMutex.lock();
	bool paused = mIsPaused;
	mMutex.unlock();
	return paused;
}

FLAC__StreamEncoderWriteStatus ViFlacCoder::flacWriteEncode(const FLAC__StreamEncoder *encoder, const FLAC__byte buffer[], size_t numberOfBytes, unsigned numberOfSamples, unsigned currentFrame, void *client)
{
	ViFlacCoder *coder = (ViFlacCoder*) client;
	coder->flacWriteEncodePointer(encoder, buffer, numberOfBytes, numberOfSamples, currentFrame, client);
}

FLAC__StreamEncoderWriteStatus ViFlacCoder::flacWriteEncodeHeader(const FLAC__StreamEncoder *encoder, const FLAC__byte buffer[], size_t numberOfBytes, unsigned numberOfSamples, unsigned currentFrame, void *client)
{
	ViFlacCoder *coder = (ViFlacCoder*) client;
	if(coder->mHeaderPosition < coder->mHeader.size())
	{
		coder->mHeader.replace(coder->mHeaderPosition, numberOfBytes, (char*) buffer, numberOfBytes);
	}
	else
	{
		coder->mHeader.append((char*) buffer, numberOfBytes);
		coder->mHeaderPosition += numberOfBytes;
	}
	return FLAC__STREAM_ENCODER_WRITE_STATUS_OK;
}

FLAC__StreamEncoderWriteStatus ViFlacCoder::flacWriteEncodeData(const FLAC__StreamEncoder *encoder, const FLAC__byte buffer[], size_t numberOfBytes, unsigned numberOfSamples, unsigned currentFrame, void *client)
{
	ViFlacCoder *coder = (ViFlacCoder*) client;
	qbyte *data = new qbyte[numberOfBytes];
	memcpy(data, buffer, numberOfBytes);
	emit coder->encoded(new ViSampleArray(data, numberOfBytes, numberOfSamples));
	return FLAC__STREAM_ENCODER_WRITE_STATUS_OK;
}

FLAC__StreamEncoderSeekStatus ViFlacCoder::flacSeekEncode(const FLAC__StreamEncoder *encoder, FLAC__uint64 absolute_byte_offset, void *client)
{
	ViFlacCoder *coder = (ViFlacCoder*) client;
	coder->mHeaderPosition = absolute_byte_offset + 4; // + 4, because the "fLaC" word at the start is not considered data
	coder->flacWriteEncodePointer = &ViFlacCoder::flacWriteEncodeHeader;
	return FLAC__STREAM_ENCODER_SEEK_STATUS_OK;
}

FLAC__StreamEncoderTellStatus ViFlacCoder::flacTellEncode(const FLAC__StreamEncoder *encoder, FLAC__uint64 *absolute_byte_offset, void *client)
{
	return FLAC__STREAM_ENCODER_TELL_STATUS_OK;
}

void ViFlacCoder::flacErrorDecode(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client)
{
	ViFlacCoder *coder = (ViFlacCoder*) client;
	coder->setError(ViCoder::DecodingError);
}

void ViFlacCoder::flacMetadataDecode(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client)
{
	ViFlacCoder *coder = (ViFlacCoder*) client;

	coder->mInputFormat.setChannels(metadata->data.stream_info.channels);
	coder->mInputFormat.setSampleSize(metadata->data.stream_info.bits_per_sample);
	coder->mInputFormat.setSampleRate(metadata->data.stream_info.sample_rate);
	coder->mInputFormat.setSampleType(ViAudioFormat::SignedInt);
	coder->mInputFormat.setCodec(&ViFlacCodec::instance());

	emit coder->formatChanged(coder->mInputFormat);
}

FLAC__StreamDecoderReadStatus ViFlacCoder::flacReadDecode(const FLAC__StreamDecoder *decoder, FLAC__byte buffer[], size_t *bytes, void *client)
{
	ViFlacCoder *coder = (ViFlacCoder*) client;
	while(!coder->mAtEnd && *bytes > coder->dataSize())
	{
		coder->pause();
		coder->wait();
	}
	if(coder->mAtEnd && *bytes > coder->dataSize())
	{
		*bytes = coder->dataSize();
	}
	coder->mMutex.lock();
	memcpy(buffer,coder->mData.data(), *bytes);
	coder->mData.remove(0, *bytes);
	coder->mMutex.unlock();
	return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
}

FLAC__StreamDecoderWriteStatus ViFlacCoder::flacWriteDecode(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client)
{
	if(frame->header.channels > 2)
	{
		return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
	}
	else if(frame->header.bits_per_sample == 8)
	{
		return flacWriteDecode8(decoder, frame, buffer, client);
	}
	else if(frame->header.bits_per_sample == 16)
	{
		return flacWriteDecode16(decoder, frame, buffer, client);
	}
	else if(frame->header.bits_per_sample == 32)
	{
		return flacWriteDecode32(decoder, frame, buffer, client);
	}
	else 
	{
		return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
	}
}

FLAC__StreamDecoderWriteStatus ViFlacCoder::flacWriteDecode8(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client)
{
	ViFlacCoder *coder = (ViFlacCoder*) client;
	int samples = frame->header.blocksize;
	int size = frame->header.channels * samples;
	qbyte8s *data = new qbyte8s[size];
	if(frame->header.channels == 1)
	{
		for(int i = 0; i < size; ++i)
		{
			data[i] = buffer[0][i];
		}
	}
	else if(frame->header.channels == 2)
	{
		qbyte8s left[samples];
		qbyte8s right[samples];
		for(int i = 0; i < samples; ++i)
		{
			left[i] = buffer[0][i];
			right[i] = buffer[1][i];
		}
		samples = ViChannelConverter<qbyte8s>::combineChannels(left, right, data, samples);
	}
	else
	{
		return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
	}

	emit coder->decoded(new ViSampleArray(data, size, samples));
	return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

FLAC__StreamDecoderWriteStatus ViFlacCoder::flacWriteDecode16(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client)
{
	ViFlacCoder *coder = (ViFlacCoder*) client;
	int samples = frame->header.blocksize;
	int size = frame->header.channels * samples;
	qbyte16s *data = new qbyte16s[size];
	if(frame->header.channels == 1)
	{
		for(int i = 0; i < size; ++i)
		{
			data[i] = buffer[0][i];
		}
	}
	else if(frame->header.channels == 2)
	{
		qbyte16s left[samples];
		qbyte16s right[samples];
		for(int i = 0; i < samples; ++i)
		{
			left[i] = buffer[0][i];
			right[i] = buffer[1][i];
		}
		samples = ViChannelConverter<qbyte16s>::combineChannels(left, right, data, samples);
	}
	else
	{
		return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
	}

	emit coder->decoded(new ViSampleArray(data, size, samples));
	return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

FLAC__StreamDecoderWriteStatus ViFlacCoder::flacWriteDecode32(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client)
{
	ViFlacCoder *coder = (ViFlacCoder*) client;
	int samples = frame->header.blocksize;
	int size = frame->header.channels * samples;
	qbyte32s *data = new qbyte32s[size];
	if(frame->header.channels == 1)
	{
		for(int i = 0; i < size; ++i)
		{
			data[i] = buffer[0][i];
		}
	}
	else if(frame->header.channels == 2)
	{
		qbyte32s left[samples];
		qbyte32s right[samples];
		for(int i = 0; i < samples; ++i)
		{
			left[i] = buffer[0][i];
			right[i] = buffer[1][i];
		}
		samples = ViChannelConverter<qbyte32s>::combineChannels(left, right, data, samples);
	}
	emit coder->decoded(new ViSampleArray(data, size, samples));
	return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

ViCoder::Error ViFlacCoder::initializeLibrary()
{
	QList<bool> loaded;
	int success = 0;
	int failure = 0;
	
	char *versionString;
	bool version = (versionString = (char*) mLibrary.resolve("FLAC__VERSION_STRING")) != NULL;
	if(version)
	{
		mVersion = QString(versionString);
	}
	loaded.append(version);

	loaded.append((m_FLAC__stream_encoder_new = (FLAC__StreamEncoder* (*)()) mLibrary.resolve("FLAC__stream_encoder_new")) != NULL);
	loaded.append((m_FLAC__stream_encoder_delete = (void (*)(FLAC__StreamEncoder*)) mLibrary.resolve("FLAC__stream_encoder_delete")) != NULL);
	loaded.append((m_FLAC__stream_encoder_init_stream = (FLAC__StreamEncoderInitStatus (*)(FLAC__StreamEncoder*, FLAC__StreamEncoderWriteCallback, FLAC__StreamEncoderSeekCallback, FLAC__StreamEncoderTellCallback, FLAC__StreamEncoderMetadataCallback, void*)) mLibrary.resolve("FLAC__stream_encoder_init_stream")) != NULL);
	loaded.append((m_FLAC__stream_encoder_finish = (FLAC__bool (*)(FLAC__StreamEncoder*)) mLibrary.resolve("FLAC__stream_encoder_finish")) != NULL);

	loaded.append((m_FLAC__stream_encoder_set_verify = (FLAC__bool (*)(FLAC__StreamEncoder*, FLAC__bool)) mLibrary.resolve("FLAC__stream_encoder_set_verify")) != NULL);
	loaded.append((m_FLAC__stream_encoder_set_compression_level = (FLAC__bool (*)(FLAC__StreamEncoder*, unsigned)) mLibrary.resolve("FLAC__stream_encoder_set_compression_level")) != NULL);
	loaded.append((m_FLAC__stream_encoder_set_channels = (FLAC__bool (*)(FLAC__StreamEncoder*, unsigned)) mLibrary.resolve("FLAC__stream_encoder_set_channels")) != NULL);
	loaded.append((m_FLAC__stream_encoder_set_bits_per_sample = (FLAC__bool (*)(FLAC__StreamEncoder*, unsigned)) mLibrary.resolve("FLAC__stream_encoder_set_bits_per_sample")) != NULL);
	loaded.append((m_FLAC__stream_encoder_set_sample_rate = (FLAC__bool (*)(FLAC__StreamEncoder*, unsigned)) mLibrary.resolve("FLAC__stream_encoder_set_sample_rate")) != NULL);

	loaded.append((m_FLAC__stream_encoder_process_interleaved = (FLAC__bool (*)(FLAC__StreamEncoder*, const FLAC__int32[], unsigned)) mLibrary.resolve("FLAC__stream_encoder_process_interleaved")) != NULL);

	loaded.append((m_FLAC__stream_decoder_new = (FLAC__StreamDecoder* (*)()) mLibrary.resolve("FLAC__stream_decoder_new")) != NULL);
	loaded.append((m_FLAC__stream_decoder_delete = (void (*)(FLAC__StreamDecoder*)) mLibrary.resolve("FLAC__stream_decoder_delete")) != NULL);
	loaded.append((m_FLAC__stream_decoder_finish = (FLAC__bool (*)(FLAC__StreamDecoder*)) mLibrary.resolve("FLAC__stream_decoder_finish")) != NULL);
	loaded.append((m_FLAC__stream_decoder_init_stream = (FLAC__StreamDecoderInitStatus (*)(FLAC__StreamDecoder*, FLAC__StreamDecoderReadCallback, FLAC__StreamDecoderSeekCallback, FLAC__StreamDecoderTellCallback, FLAC__StreamDecoderLengthCallback, FLAC__StreamDecoderEofCallback, FLAC__StreamDecoderWriteCallback, FLAC__StreamDecoderMetadataCallback, FLAC__StreamDecoderErrorCallback, void*)) mLibrary.resolve("FLAC__stream_decoder_init_stream")) != NULL);

	loaded.append((m_FLAC__stream_decoder_process_until_end_of_stream = (FLAC__bool (*)(FLAC__StreamDecoder*)) mLibrary.resolve("FLAC__stream_decoder_process_until_end_of_stream")) != NULL);

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
