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

void ViCoder::encode(ViAudioBuffer *buffer, QIODevice *device, ViAudioFormat *format)
{
	mEncodingThread->setDevice(device);
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

     AVCodec *codec;
     AVCodecContext *codecContext= NULL;
    int frame_size, i, j, ret, got_packet;
     FILE *f;
 FILE *f2;
     char *samples;
    AVFrame *frame;
     float t, tincr;
   AVPacket packet;
	int sampleSize, s;
qint64 tt = 0;


	CodecID mCodecId;
	/*QString codecName = mFormat->codec().abbreviation();
	if(codecName == "MP2") mCodecId = CODEC_ID_MP2;
	else if(codecName == "MP3") mCodecId = CODEC_ID_MP3;
	else if(codecName == "AAC") mCodecId = CODEC_ID_AAC;
	else if(codecName == "AC3") mCodecId = CODEC_ID_AC3;
	else if(codecName == "DTS") mCodecId = CODEC_ID_DTS;
	else if(codecName == "VORBIS") mCodecId = CODEC_ID_VORBIS;
	else if(codecName == "WMA") mCodecId = CODEC_ID_WMAV2;
	else if(codecName == "FLAC") mCodecId = CODEC_ID_FLAC;
	else if(codecName == "ALAC") mCodecId = CODEC_ID_ALAC;*/

mCodecId = CODEC_ID_FLAC;



	if(!mDevice->isOpen())
	{
		if(!mDevice->open(QIODevice::WriteOnly))
		{
			mError = ViCoder::DeviceOpenError;
			goto END;
		}
	}

	codec = avcodec_find_encoder(mCodecId);
	if(!codec)
	{
		mError = ViCoder::NoEncoderError;
		goto END;
	}

	codecContext = avcodec_alloc_context3(codec);
codecContext->sample_fmt = AV_SAMPLE_FMT_S16;
	codecContext->sample_rate = 44100;
	codecContext->channels = 2;

	/*if(mFormat->bitRate() > 0)
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
	}*/

	if(avcodec_open2(codecContext, codec, NULL) < 0)
	{
		mError = ViCoder::UnsupportedCodecError;
		goto END;
	}



	frame_size = codecContext->frame_size;
    frame = avcodec_alloc_frame();
    if (!frame) {
        fprintf(stderr, "Could not allocate frame.\n");
        exit(1);
    }
    cout<<"frame: "<<frame_size<<endl;
if(frame_size == 0)
{
	frame_size = 1024;
}
f = fopen("/home/visore/test.flac", "wb");
f2 = fopen("/home/visore/a.wav", "rb");

	sampleSize = frame_size * 2 * codecContext->channels;
	samples = new char[sampleSize];
	sampleSize = readStream->read(samples, sampleSize);
//sampleSize = fread(samples, 1, sampleSize, f2);

cout<<"*-*: "<<samples[0]<<samples[1]<<samples[2]<<samples[3]<<endl;

	tt+=sampleSize;
	

	while(sampleSize > 0)
	{
//cout<<"Writing: "<<sampleSize<<endl;
		frame->data[0] = (uint8_t*)samples;
		frame->nb_samples = sampleSize / (2 * codecContext->channels);
		av_init_packet(&packet);
		packet.data = NULL;
		packet.size = 0;
		got_packet = 0;
		ret = avcodec_encode_audio2(codecContext, &packet, frame, &got_packet);
		if (ret < 0) {
				cout<<"Audio encoding failed"<<endl;
		}
		    if (got_packet) {
		       //fwrite(packet.data, 1, packet.size, f);
				fwrite(samples, 1, sampleSize, f);
				//mDevice->write(reinterpret_cast<char*>(packet.data), packet.size);
		        av_destruct_packet(&packet);
		   }
		//sampleSize = readStream->read(samples, sampleSize);
		sampleSize = fread(samples, 1, sampleSize, f2);
		tt+=sampleSize;
	}
//42056684
	mDevice->close();

	fclose(f);
cout<<"pp: "<<tt<<endl;
	END:
	

	av_free(frame);
    delete [] samples;
	avcodec_close(codecContext);
	av_free(codecContext);

cout<<"err: "<<mError<<endl;
}

ViDecodingThread::ViDecodingThread(QObject *parent)
	: ViCoderThread(parent)
{
	mFile = "";
}

void ViDecodingThread::setFile(QString file)
{
	mFile = file;
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
		mError = ViCoder::DeviceOpenError;
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
		mError = ViCoder::DeviceOpenError;
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
