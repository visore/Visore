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


ViCoder::ViCoder()
	: QObject()
{
	mEncodingThread = new ViEncodingThread(this);
	ViObject::connect(mEncodingThread, SIGNAL(stateChanged(ViCoder::State)), this, SIGNAL(stateChanged(ViCoder::State)));
}

ViCoder::~ViCoder()
{
	if(mEncodingThread != NULL)
	{
		delete mEncodingThread;
		mEncodingThread = NULL;
	}
}

ViCoder::State ViCoder::encode(QByteArray *inData, QByteArray *outData, ViAudioFormat *format)
{

}

void ViCoder::decode(QString file, ViAudioBuffer *buffer, ViAudioFormat *format)
{
	mEncodingThread->setFile(file);
	mEncodingThread->setBuffer(buffer);
	mEncodingThread->setFormat(format);
	mEncodingThread->start();
}

ViCoderThread::ViCoderThread(QObject *parent)
	: QThread(parent)
{
	mFormat = NULL;
	mState = ViCoder::UninitializedState;
	mError = ViCoder::NoError;
}

void ViCoderThread::setFormat(ViAudioFormat *format)
{
	mFormat = format;
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
	mFile = "";
	mBuffer = NULL;
	av_register_all();
	avcodec_register_all();
}

void ViEncodingThread::setFile(QString file)
{
	mFile = file;
}

void ViEncodingThread::setBuffer(ViAudioBuffer *buffer)
{
	mBuffer = buffer;
}

void ViEncodingThread::setFormat(ViAudioFormat *format)
{
	AVCodecContext *codecContext = NULL;
	AVFormatContext *formatContext = NULL;
	AVCodec *codec;
	int streamId;
	CodecID codecId;
	if(avformat_open_input(&formatContext, mFile.toAscii().data(), NULL, NULL) < 0)
	{
		mError = ViCoder::FileOpenError;
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
	format->setSampleRate(codecContext->sample_rate);
	format->setChannelCount(codecContext->channels);
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
		format->setSampleSize(8);
	}
	else if(codecContext->sample_fmt == AV_SAMPLE_FMT_S32
		|| codecContext->sample_fmt == AV_SAMPLE_FMT_S32P )
	{
		format->setSampleType(QAudioFormat::SignedInt);
		format->setSampleSize(32);
	}
	else if(codecContext->sample_fmt == AV_SAMPLE_FMT_S16
		|| codecContext->sample_fmt == AV_SAMPLE_FMT_S16P)
	{
		format->setSampleType(QAudioFormat::SignedInt);
		format->setSampleSize(16);
	}
	else
	{
		format->setSampleType(QAudioFormat::Unknown);
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

void ViEncodingThread::run()
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
		mError = ViCoder::FileOpenError;
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
