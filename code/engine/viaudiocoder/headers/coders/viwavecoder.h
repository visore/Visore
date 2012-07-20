#ifndef VIWAVECODER_H
#define VIWAVECODER_H

#include <viabstractcoder.h>
#include <visampleconverter.h>

class ViWaveCoder : public ViAbstractCoder
{

	public:

		ViWaveCoder();

		ViCoder::Error load();
		ViCoder::Error load(QString filePath);
		bool unload();

		ViAudioCodec* detectCodec(const QByteArray &data);
		QByteArray& header();
		int headerSize();

		bool initializeEncode();
		bool finalizeEncode();
		void encode(const void *input, int samples);

		bool initializeDecode();
		bool finalizeDecode();
		void decode(const void *input, int size);

	protected:

		void decodeHeader(const void *input, int size);
		void decodeData(const void *input, int size);

		ViCoder::Error initializeLibrary();
		short toShort(char data[]);
		int toInt(char data[]);

	private:

		void (ViWaveCoder::*decodePointer)(const void *input, int size);

		ViSampleConverter mConverter;
		qint64 mSamples;

};

#endif
