#include "viaudiocodecs.h"

extern "C"
{
	#include "libavcodec/avcodec.h"
}

void ViAudioCodecs::initlialize()
{
	avcodec_init();
	avcodec_register_all();
}

ViAudioCodecs::Result ViAudioCodecs::encode(ViAudioBuffer *bufferIn, ViAudioBuffer *bufferOut)
{
	ViAudioCodecs::initlialize();
	AVCodec *codec = avcodec_find_encoder(CODEC_ID_PCM_S16LE);
	if(!codec)
	{
		return ViAudioCodecs::CodecNotFound;
	}

	AVCodecContext *context = avcodec_alloc_context3(codec);
	//context->bit_rate = 64000;
	context->sample_rate = 22050;
	context->channels = 1;
	context->sample_fmt = AV_SAMPLE_FMT_S16;

	if(avcodec_open2(context, codec, NULL) < 0)
	{
		return ViAudioCodecs::CodecNotOpened;
	}

	static const int OUTPUT_BUFFER_SIZE = 102400;
	ViAudioBufferStream *readStream = bufferIn->createReadStream();
	ViAudioBufferStream *writeStream = bufferOut->createWriteStream();

	qint64 samplesRead = 0;
	while(samplesRead < readStream->bufferSize())
	{
		samplesRead += OUTPUT_BUFFER_SIZE;
		uint8_t *outputData = new uint8_t[OUTPUT_BUFFER_SIZE];
		ViAudioBufferChunk inChunk;
		readStream->read(&inChunk, OUTPUT_BUFFER_SIZE);
		int size = avcodec_encode_audio(context, outputData, OUTPUT_BUFFER_SIZE, reinterpret_cast<const short int*>(inChunk.data()));
		if(size < 0)
		{
			ViAudioCodecs::EncodingProblem;
			return;
		}
		ViAudioBufferChunk outChunk(reinterpret_cast<char*>(outputData));
		writeStream->write(&outChunk, size);
	}

	avcodec_close(context);
	av_free(context);
	return ViAudioCodecs::Success;
}

ViAudioCodecs::Result ViAudioCodecs::encode(ViAudioBuffer *bufferIn, QString outputFilePath)
{
	ViAudioBuffer outBuffer;
	ViAudioCodecs::Result result = ViAudioCodecs::encode(bufferIn, &outBuffer);
	if(result == ViAudioCodecs::Success)
	{
		QFile file(outputFilePath);
		if(!file.open(QIODevice::WriteOnly))
		{
			return ViAudioCodecs::FileNotOpened;
		}
		file.write(*outBuffer.data());	
	}
	return result;
}
