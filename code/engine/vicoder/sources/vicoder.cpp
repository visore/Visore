#include "vicoder.h"

//Include pure-C headers
extern "C"
{
	#include "avutil.h"
	#include "avcodec.h"
	#include "avformat.h"
}

#include <iostream>
using namespace std;
#define AUDIO_INBUF_SIZE 320
#define AUDIO_REFILL_THRESH 4096


ViCoder::ViCoder()
	: QObject()
{
	mEncodingThread = new ViEncodingThread(this);
	mDecodingThread = new ViDecodingThread(this);
	ViObject::connect(mDecodingThread, SIGNAL(stateChanged(ViCoder::State)), this, SIGNAL(stateChanged(ViCoder::State)));
}

ViCoder::~ViCoder()
{
	if(mEncodingThread != NULL)
	{
		delete mEncodingThread;
		mEncodingThread = NULL;
	}
	if(mDecodingThread != NULL)
	{
		delete mDecodingThread;
		mDecodingThread = NULL;
	}
}

void ViCoder::encode(ViAudioBuffer *buffer, QFile *file, ViAudioFormat *format)
{
	mEncodingThread->setDevice(file);
	mEncodingThread->setFile(file->fileName());
	mEncodingThread->setBuffer(buffer);
	mEncodingThread->setFormat(format);
	mEncodingThread->start();
}

void ViCoder::decode(QString file, ViAudioBuffer *buffer, ViAudioFormat *format)
{
	mDecodingThread->setFile(file);
	mDecodingThread->setBuffer(buffer);
	mDecodingThread->setFormat(format);
	mDecodingThread->start();
}

void ViCoder::stop()
{
	mDecodingThread->quit();
}

ViCoderThread::ViCoderThread(QObject *parent)
	: QThread(parent)
{
	mBuffer = NULL;
	mFormat = NULL;
	mFile = "";
	mState = ViCoder::UninitializedState;
	mError = ViCoder::NoError;
	av_register_all();
	avcodec_register_all();
}

void ViCoderThread::setFormat(ViAudioFormat *format)
{
	mFormat = format;
}

void ViCoderThread::setBuffer(ViAudioBuffer *buffer)
{
	mBuffer = buffer;
}

void ViCoderThread::setFile(QString file)
{
	mFile = file;
}

ViCoder::State ViCoderThread::status()
{
	return mState;
}

ViCoder::Error ViCoderThread::error()
{
	return mError;
}

ViEncodingThread::ViEncodingThread(QObject *parent)
	: ViCoderThread(parent)
{
	mDevice = NULL;
}

ViEncodingThread::~ViEncodingThread()
{
	if(mFormat != NULL)
	{
		delete mFormat;
		mFormat = NULL;
	}
}

void ViEncodingThread::setDevice(QIODevice *device)
{
	mDevice = device;
}

void ViEncodingThread::setFormat(ViAudioFormat *format)
{
	mFormat = new ViAudioFormat(*format);
}

void ViEncodingThread::run()
{
	ViAudioBufferStream *readStream = mBuffer->createReadStream();
	mDevice->close();

	AVCodec *codec = NULL;
	AVCodecContext *codecContext= NULL;
	AVFrame *frame = NULL;
	AVFormatContext *formatContext = NULL;
	AVStream *stream = NULL;
	AVPacket packet;
	int sampleSize, frameSize, gotPacket;
	char *samples = NULL;

    CodecID codecId;
	QString codecName = mFormat->codec().abbreviation();
	if(codecName == "MP2")
	{
		codecId = CODEC_ID_MP2;
		codecName = "mp2";
	}
	else if(codecName == "MP3")
	{
		codecId = CODEC_ID_MP3;
		codecName = "mp3";
	}
	else if(codecName == "AAC")
	{
		codecId = CODEC_ID_AAC;
		codecName = "aac";
	}
	else if(codecName == "AC3")
	{
		codecId = CODEC_ID_AC3;
		codecName = "ac3";
	}
	else if(codecName == "VORBIS")
	{
		codecId = CODEC_ID_VORBIS;
		codecName = "ogg";
	}
	else if(codecName == "FLAC")
	{
		codecId = CODEC_ID_FLAC;
		codecName = "flac";
	}
	else if(codecName == "WAV")
	{
		if(mFormat->byteOrder() == QAudioFormat::LittleEndian)
		{
			if(mFormat->sampleType() == QAudioFormat::Float)
			{
				if(mFormat->sampleSize() == 64) codecId = CODEC_ID_PCM_F64LE;
				else codecId = CODEC_ID_PCM_F32LE;
			}
			else if(mFormat->sampleType() == QAudioFormat::UnSignedInt)
			{
				if(mFormat->sampleSize() == 8) codecId = CODEC_ID_PCM_U8;
				else if(mFormat->sampleSize() == 24) codecId = CODEC_ID_PCM_U24LE;
				else if(mFormat->sampleSize() == 32) codecId = CODEC_ID_PCM_U32LE;
				else codecId = CODEC_ID_PCM_U16LE;
			}
			else
			{
				if(mFormat->sampleSize() == 8) codecId = CODEC_ID_PCM_S8;
				else if(mFormat->sampleSize() == 24) codecId = CODEC_ID_PCM_S24LE;
				else if(mFormat->sampleSize() == 32) codecId = CODEC_ID_PCM_S32LE;
				else codecId = CODEC_ID_PCM_S16LE;
			}
		}
		else
		{
			if(mFormat->sampleType() == QAudioFormat::Float)
			{
				if(mFormat->sampleSize() == 64) codecId = CODEC_ID_PCM_F64BE;
				else codecId = CODEC_ID_PCM_F32BE;
			}
			else if(mFormat->sampleType() == QAudioFormat::UnSignedInt)
			{
				if(mFormat->sampleSize() == 8) codecId = CODEC_ID_PCM_U8;
				else if(mFormat->sampleSize() == 24) codecId = CODEC_ID_PCM_U24BE;
				else if(mFormat->sampleSize() == 32) codecId = CODEC_ID_PCM_U32BE;
				else codecId = CODEC_ID_PCM_U16LE;
			}
			else
			{
				if(mFormat->sampleSize() == 8) codecId = CODEC_ID_PCM_S8;
				else if(mFormat->sampleSize() == 24) codecId = CODEC_ID_PCM_S24BE;
				else if(mFormat->sampleSize() == 32) codecId = CODEC_ID_PCM_S32BE;
				else codecId = CODEC_ID_PCM_S16BE;
			}
		}
		codecName = "wav";
	}

	if(avformat_alloc_output_context2(&formatContext, NULL, codecName.toAscii().data(), mFile.toAscii().data()) < 0)
	{
		mError = ViCoder::NoEncoderError;
		goto END;
	}
	formatContext->oformat->audio_codec = codecId;
	stream = avformat_new_stream(formatContext, 0);
	if(stream == NULL)
	{
		mError = ViCoder::UnsupportedFormatError;
		goto END;
	}

	codecContext = stream->codec;
	codecContext->codec_id = formatContext->oformat->audio_codec;
	codecContext->codec_type = AVMEDIA_TYPE_AUDIO;

	if(mFormat->bitRate() > 0)
	{
		codecContext->bit_rate = mFormat->bitRate() * 1000;
	}
	codecContext->sample_rate = mFormat->sampleRate();

	codecContext->channels = mFormat->channelCount();

	if(mFormat->sampleSize() == 8)
	{
		codecContext->sample_fmt = AV_SAMPLE_FMT_U8;
	}
	else if(mFormat->sampleSize() == 32)
	{
		codecContext->sample_fmt = AV_SAMPLE_FMT_S32;
	}
	else if(mFormat->sampleType() == QAudioFormat::Float)
	{
		codecContext->sample_fmt = AV_SAMPLE_FMT_FLT;
	}
	else
	{
		codecContext->sample_fmt = AV_SAMPLE_FMT_S16;
	}

	codecContext->time_base.num = 1;
	codecContext->time_base.den = codecContext->sample_rate;

	codec = avcodec_find_encoder(codecContext->codec_id);
	if(avcodec_open2(codecContext, codec, NULL) < 0)
	{
		mError = ViCoder::UnsupportedCodecError;
		goto END;
	}

	if(avio_open(&formatContext->pb, mFile.toAscii().data(), AVIO_FLAG_WRITE) < 0)
	{cout<<mFile.toAscii().data()<<endl;
		mError = ViCoder::DeviceError;
		goto END;
	}

	if(avformat_write_header(formatContext, NULL) < 0)
	{
		mError = ViCoder::DeviceError;
		goto END;
	}

	if(!mDevice->open(QIODevice::Append))
	{
		mError = ViCoder::DeviceError;
		goto END;
	}

    frame = avcodec_alloc_frame();
	if(!frame)
	{
        mError = ViCoder::OutOfMemoryError;
		goto END;
    }

	frameSize = codecContext->frame_size;
	if(frameSize == 0)
	{
		frameSize = 1024;
	}
	sampleSize = frameSize * 2 * codecContext->channels;
	samples = new char[sampleSize];
	sampleSize = readStream->read(samples, sampleSize);

	while(sampleSize > 0)
	{
		frame->data[0] = (uint8_t*) samples;
		frame->nb_samples = frameSize;
		av_init_packet(&packet);
		packet.data = NULL;
		packet.size = 0;
		gotPacket = 0;
		if(avcodec_encode_audio2(codecContext, &packet, frame, &gotPacket) < 0)
		{
			mError = ViCoder::EncodingError;
			goto END;
		}
		else if(gotPacket)
		{
			//mDevice->write(reinterpret_cast<char*>(packet.data), packet.size);
			av_interleaved_write_frame(formatContext, &packet);
			sampleSize = readStream->read(samples, sampleSize);
		}
	}

	mDevice->close();

	if(av_write_trailer(formatContext) < 0)
	{
		mError = ViCoder::DeviceError;
		goto END;
	}

	END:

	if(codecContext != NULL)
	{
		avcodec_close(codecContext);
	}

	if(formatContext != NULL)
	{
		if(formatContext->streams[0] != NULL)
		{
			av_freep(&formatContext->streams[0]->codec);
			av_freep(&formatContext->streams[0]);
		}
		if(formatContext->pb != NULL)
		{
			avio_close(formatContext->pb);
		}
		av_free(formatContext);
	}

	if(frame != NULL)
	{
		av_free(frame);
	}

	if(samples != NULL)
	{
    	delete [] samples;
	}

	if(mError != ViCoder::NoError)
	{
		QFile file(mFile);
		if(file.exists())
		{
			file.remove();
		}
	}
}

ViDecodingThread::ViDecodingThread(QObject *parent)
	: ViCoderThread(parent)
{
}

void ViDecodingThread::setFormat(ViAudioFormat *format)
{
	ViCoderThread::setFormat(format);

	AVCodecContext *codecContext = NULL;
	AVFormatContext *formatContext = NULL;
	AVCodec *codec;
	int streamId;
	CodecID codecId;
	if(avformat_open_input(&formatContext, mFile.toAscii().data(), NULL, NULL) < 0)
	{
		mError = ViCoder::DeviceError;
		goto END;
	}
	else
	{
		streamId = avformat_find_stream_info(formatContext, NULL);
		if(streamId < 0)
		{
			streamId = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, -1, -1, &codec, 0);
		}
		if(streamId == AVERROR_STREAM_NOT_FOUND)
		{
			mError = ViCoder::NoStreamError;
			goto END;
		}
		else if(streamId == AVERROR_DECODER_NOT_FOUND)
		{
			mError = ViCoder::NoDecoderError;
			goto END;
		}
	}

	codecContext = formatContext->streams[streamId]->codec;
	format->setSampleRate(codecContext->sample_rate);
	format->setChannelCount(codecContext->channels);
	format->setByteOrder(QAudioFormat::LittleEndian);
	if(codecContext->sample_fmt == AV_SAMPLE_FMT_FLT
		|| codecContext->sample_fmt == AV_SAMPLE_FMT_DBL
		|| codecContext->sample_fmt == AV_SAMPLE_FMT_FLTP
		|| codecContext->sample_fmt == AV_SAMPLE_FMT_DBLP)
	{
		format->setSampleType(QAudioFormat::Float);
	}
	else if(codecContext->sample_fmt == AV_SAMPLE_FMT_U8
		|| codecContext->sample_fmt == AV_SAMPLE_FMT_U8P)
	{
		format->setSampleType(QAudioFormat::UnSignedInt);
	}
	else if(codecContext->sample_fmt == AV_SAMPLE_FMT_S32
		|| codecContext->sample_fmt == AV_SAMPLE_FMT_S32P )
	{
		format->setSampleType(QAudioFormat::SignedInt);
	}
	else if(codecContext->sample_fmt == AV_SAMPLE_FMT_S16
		|| codecContext->sample_fmt == AV_SAMPLE_FMT_S16P)
	{
		format->setSampleType(QAudioFormat::SignedInt);
	}
	else
	{
		format->setSampleType(QAudioFormat::SignedInt);
	}


	if(codecContext->sample_fmt == AV_SAMPLE_FMT_U8P
		|| codecContext->sample_fmt == AV_SAMPLE_FMT_U8)
	{
		format->setSampleSize(8);
	}
	else if(codecContext->sample_fmt == AV_SAMPLE_FMT_S16
		|| codecContext->sample_fmt == AV_SAMPLE_FMT_S16P)
	{
		format->setSampleSize(16);
	}
	else if(codecContext->sample_fmt == AV_SAMPLE_FMT_S32
		|| codecContext->sample_fmt == AV_SAMPLE_FMT_S32P
		|| codecContext->sample_fmt == AV_SAMPLE_FMT_FLT
		|| codecContext->sample_fmt == AV_SAMPLE_FMT_FLTP)
	{
		format->setSampleSize(32);
	}
	else if(codecContext->sample_fmt == AV_SAMPLE_FMT_DBLP
		|| codecContext->sample_fmt == AV_SAMPLE_FMT_DBL)
	{
		format->setSampleSize(64);
	}
	else
	{
		format->setSampleSize(16);
	}

	codecId = codecContext->codec_id;
	switch(codecId)
	{
		case CODEC_ID_PCM_S16LE: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_S16BE: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_U16LE: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_U16BE: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_S8: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_U8: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_MULAW: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_ALAW: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_S32LE: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_S32BE: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_U32LE: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_U32BE: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_S24LE: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_S24BE: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_U24LE: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_U24BE: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_S24DAUD: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_ZORK: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_S16LE_PLANAR: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_DVD: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_F32BE: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_F32LE: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_F64BE: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_F64LE: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_BLURAY: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_PCM_LXF: format->setCodec(ViCodecManager::selected("PCM")); break;
		case CODEC_ID_MP2: format->setCodec(ViCodecManager::selected("MP2")); break;
		case CODEC_ID_MP3: format->setCodec(ViCodecManager::selected("MP3")); break;
		case CODEC_ID_AAC: format->setCodec(ViCodecManager::selected("AAC")); break;
		case CODEC_ID_AC3: format->setCodec(ViCodecManager::selected("AC3")); break;
		case CODEC_ID_VORBIS: format->setCodec(ViCodecManager::selected("VORBIS")); break;
		case CODEC_ID_WMAV1: format->setCodec(ViCodecManager::selected("WMA")); break;
		case CODEC_ID_WMAV2: format->setCodec(ViCodecManager::selected("WMA")); break;
		case CODEC_ID_FLAC: format->setCodec(ViCodecManager::selected("FLAC")); break;
		case CODEC_ID_ALAC: format->setCodec(ViCodecManager::selected("ALAC")); break;
	}

	END:

	avcodec_close(codecContext);
	av_free(codecContext);
	av_free(formatContext);

	if(mError != ViCoder::NoError)
	{
		mState = ViCoder::FailureState;
	}
	else
	{
		mState = ViCoder::SuccessState;
	}

	emit stateChanged(mState);
}

void ViDecodingThread::run()
{
	mError = ViCoder::NoError;
	mState = ViCoder::ActiveState;
	emit stateChanged(mState);

	AVCodec *codec;
	AVCodecContext *codecContext = NULL;
	AVFormatContext *formatContext = NULL;
	AVPacket packet;
	AVFrame frame;
	
	ViAudioBufferStream *writeStream = mBuffer->createWriteStream();

	int streamId;
	int hasFrame;
	int result;

	if(avformat_open_input(&formatContext, mFile.toAscii().data(), NULL, NULL) < 0)
	{
		mError = ViCoder::DeviceError;
		goto END;
	}
	else
	{
		streamId = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, -1, -1, &codec, 0);
		if(streamId == AVERROR_STREAM_NOT_FOUND)
		{
			mError = ViCoder::NoStreamError;
			goto END;
		}
		else if(streamId == AVERROR_DECODER_NOT_FOUND)
		{
			mError = ViCoder::NoDecoderError;
			goto END;
		}
	}

	codecContext = formatContext->streams[streamId]->codec;
	if(avcodec_open2(codecContext, codec, NULL) < 0)
	{
		mError = ViCoder::UnsupportedCodecError;
		goto END;
	}

	av_init_packet(&packet);

	while(true)
	{ 
		if(av_read_frame(formatContext, &packet) < 0)
		{
			break;
		}
		avcodec_alloc_frame();
		avcodec_get_frame_defaults(&frame);
		hasFrame = 0;
		result = avcodec_decode_audio4(codecContext, &frame, &hasFrame, &packet);
		av_free_packet(&packet);
		if(result < 0)
		{
			mError = ViCoder::DecodingError;
			goto END;
		}
		if(hasFrame)
		{
			const int bps = av_get_bytes_per_sample(codecContext->sample_fmt);
			const int dataSize = frame.nb_samples * codecContext->channels * bps;
			writeStream->write(reinterpret_cast<char*>(frame.data[0]), dataSize);  
		}
	}

	END:

	avcodec_close(codecContext);
	av_free(codecContext);
	av_free(formatContext);

	if(mError != ViCoder::NoError)
	{
		mState = ViCoder::FailureState;
	}
	else
	{
		mState = ViCoder::SuccessState;
	}

	emit stateChanged(mState);
}