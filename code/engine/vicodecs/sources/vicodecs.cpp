#include "vicodecs.h"

//Include pure-C headers
extern "C"
{
	#include "avutil.h"
	#include "avcodec.h"
	#include "avformat.h"
}

#include <QDataStream>

#include <iostream>
using namespace std;

#define BUFFER_SIZE 20480
#define AUDIO_REFILL_THRESHOLD 4096

QSharedPointer<ViCodecs> ViCodecs::mInstance;

ViCodecs::ViCodecs()
	: ViSingleton()
{
	av_register_all();
	avcodec_register_all();
}

ViCodecs* ViCodecs::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViCodecs>(new ViCodecs());
	}
	return mInstance.data();
}

int ViCodecs::codecId(QString file)
{
	ViCodecs::instance();
	AVFormatContext *context = NULL;
	int id = -1;
	if(avformat_open_input(&context, file.toAscii().data(), NULL, NULL) == 0)
	{
		for(int i = 0; i < context->nb_streams; ++i)
		{
			if(context->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
			{
				id = context->streams[i]->codec->codec_id;
				break;
			}
		}
	}
	avformat_close_input(&context);
	av_free(context);
	return id;
}

ViCodecs::Status ViCodecs::encode(QByteArray *iData, QByteArray *outData)
{

}

ViCodecs::Status ViCodecs::decode(QByteArray *inData, QByteArray *outData, int ffmpegCodec)
{
	ViCodecs::instance();
	AVCodec *codec;
	AVCodecContext *context = NULL;
	AVPacket packet;
	AVFrame frame;
	quint8 buffer[BUFFER_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
	int length;

	av_init_packet(&packet);

int id;
AVFormatContext *context2 = NULL;

context2 = avformat_alloc_context();
codec = avcodec_find_decoder(CODEC_ID_MP3);
//context2->bit_rate = 160000;

context = avcodec_alloc_context3(codec);
cout<<"oo1o"<<endl;



	context->sample_rate = 44100;
	context->channels = 2;
	context->bit_rate = 160000;
context->sample_fmt = AV_SAMPLE_FMT_S16;
context->frame_size = 1;

//avformat_new_stream (context2,codec);
//context2->streams[0]->codec = context;
id =0;
cout<<"ooo:"<<id<<endl;
/*if(avformat_open_input(&context2, "/home/visore/data/ev.mp3", NULL, NULL) == 0)
	{

		id =av_find_best_stream(context2, AVMEDIA_TYPE_AUDIO, -1, -1, &codec, 0);
	}*/
   //if ((avformat_find_stream_info(context2, NULL)) < 0) {
       /* av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");
        return ret;*/
   // }

//context = context2->streams[id]->codec;

	/*codec = avcodec_find_decoder(CODEC_ID_VORBIS );
	if(!codec)
	{
		return ViCodecs::UnsupportedCodecError;
	}*/

	//context = avcodec_alloc_context3(codec);
	/*context->sample_rate = 44100;
	context->channels = 2;
	context->bit_rate = 160000;*/
/*context->sample_fmt = AV_SAMPLE_FMT_S16;
context->frame_size = 1;*/




	if(avcodec_open2(context, codec, NULL) < 0)
	{
		return ViCodecs::CodecOpenError;
	}

	QDataStream readStream(inData, QIODevice::ReadOnly);
	QDataStream writeStream(outData, QIODevice::WriteOnly);

	packet.data = buffer;


 FILE *f, *outfile;
f = fopen("/home/visore/data/ev.mp3", "rb");
if (!f) {
        fprintf(stderr, "could not open \n");
    }

outfile = fopen("/home/visore/b.wav", "wb");
if (!outfile) {
 av_free(context);

}


packet.size = fread(buffer, 1, BUFFER_SIZE, f);



int ret;
 int got_frame;

 while (1) { 
        /*if ((ret = av_read_frame(context2, &packet)) < 0)
            break;*/
avcodec_alloc_frame();
cout<<"ooo22"<<id<<endl;
    cout<<"ppp"<<endl;
            avcodec_get_frame_defaults(&frame);
            got_frame = 0;cout<<"pp2p"<<endl;
            ret = avcodec_decode_audio4(context, &frame, &got_frame, &packet);
            av_free_packet(&packet);
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Error decoding audio\n");
                break;
		}
   if (got_frame)
	{
 const int bps = av_get_bytes_per_sample(context->sample_fmt);
                const int decoded_data_size = frame.nb_samples * context->channels * bps;
          writeStream.writeRawData(reinterpret_cast<char*>(frame.data[0]), decoded_data_size);  
	}
}



























	//packet.size = readStream.readRawData(reinterpret_cast<char*>(buffer), BUFFER_SIZE);

	/*while(packet.size > 0)
	{cout<<"*"<<endl;
		int hasFrame = 0;

		if (!frame)
		{
			if(!(frame = avcodec_alloc_frame()))
			{
   				return ViCodecs::OutOfMemoryError;
			}
		}
		else
		{
			avcodec_get_frame_defaults(frame);
		}

		length = avcodec_decode_audio4(context, frame, &hasFrame, &packet);

		if(length < 0)
		{
			return ViCodecs::DecodingError;
		}
		if(hasFrame)
		{
			int dataSize = av_samples_get_buffer_size(NULL, context->channels, frame->nb_samples, context->sample_fmt, 1);
			writeStream.writeRawData(reinterpret_cast<char*>(frame->data[0]), dataSize);
//fwrite(frame->data[0], 1, dataSize, outfile);
		}
		//packet.size -= length;
		//length = readStream.readRawData(reinterpret_cast<char*>(buffer), BUFFER_SIZE);
		//if(length > 0)
		//{
		//	packet.size += length;
		//}
packet.size -= length;
       packet.data += length;
       packet.dts =
       packet.pts = AV_NOPTS_VALUE;
       if (packet.size < AUDIO_REFILL_THRESHOLD) {

             memmove(buffer, packet.data, packet.size);
            packet.data =buffer;
          length = fread(packet.data + packet.size, 1,
                       BUFFER_SIZE - packet.size, f);
            if (length > 0)
                packet.size += length;
        }
	}*/

	//avcodec_close(context);
	//av_free(context);
	//av_free(frame);

	return ViCodecs::Success;
}
