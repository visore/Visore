#include "vicodecs.h"

//Include pure-C headers
extern "C"
{
	#include "avutil.h"
	#include "avcodec.h"
	#include "avformat.h"
}

#include <iostream>
using namespace std;

QSharedPointer<ViCodecs> ViCodecs::mInstance;

ViCodecs::ViCodecs()
	: ViSingleton()
{
	mEncodingThread = new ViEncodingThread(this);
}

ViCodecs::~ViCodecs()
{
	if(mEncodingThread != NULL)
	{
		delete mEncodingThread;
		mEncodingThread = NULL;
	}
}

ViCodecs* ViCodecs::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViCodecs>(new ViCodecs());
	}
	return mInstance.data();
}

ViCodecs::Status ViCodecs::encode(QByteArray *inData, QByteArray *outData)
{

}

void ViCodecs::decode(QString file, ViAudioBuffer *buffer)
{
	ViCodecs *instance = ViCodecs::instance();
	instance->mEncodingThread->setFile(file);
	instance->mEncodingThread->setBuffer(buffer);
	instance->mEncodingThread->start();
}

ViCodecsThread::ViCodecsThread(QObject *parent)
	: QThread(parent)
{
	mStatus = ViCodecs::Uninitialized;
	mError = ViCodecs::None;
}

ViCodecs::Status ViCodecsThread::status()
{
	return mStatus;
}

ViCodecs::Error ViCodecsThread::error()
{
	return mError;
}

ViEncodingThread::ViEncodingThread(QObject *parent)
	: ViCodecsThread(parent)
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

void ViEncodingThread::run()
{
	mError = ViCodecs::None;
	mStatus = ViCodecs::Started;

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
		mError = ViCodecs::FileOpen;
		goto END;
	}
	else
	{
		streamId = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, -1, -1, &codec, 0);
		if(streamId == AVERROR_STREAM_NOT_FOUND)
		{
			mError = ViCodecs::NoStream;
			goto END;
		}
		else if(streamId == AVERROR_DECODER_NOT_FOUND)
		{
			mError = ViCodecs::NoDecoder;
			goto END;
		}
	}

	codecContext = formatContext->streams[streamId]->codec;
	if(avcodec_open2(codecContext, codec, NULL) < 0)
	{
		mError = ViCodecs::UnsupportedCodec;
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
			mError = ViCodecs::Decoding;
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

	if(mError != ViCodecs::None)
	{
		mStatus = ViCodecs::Failure;
	}
	else
	{
		mStatus = ViCodecs::Success;
	}
}
