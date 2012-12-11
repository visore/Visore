#include <viwavecoder.h>
#include <viwavecodec.h>
#include <visamplesizeconverter.h>
#include <QDataStream>
#include <QTextStream>

ViWaveCoder::ViWaveCoder()
	: ViAbstractCoder()
{
	mName = "Wave";
	addFileName("");
	mSupportedCodecs.append(&ViWaveCodec::instance());
}

ViCoder::Error ViWaveCoder::load()
{
	setError(ViCoder::NoError);
	return error();
}

ViCoder::Error ViWaveCoder::load(QString filePath)
{
	setError(ViCoder::NoError);
	return error();
}

bool ViWaveCoder::unload()
{
	return true;
}

ViAudioCodec* ViWaveCoder::detectCodec(const QByteArray &data)
{
	QDataStream stream((QByteArray*) &data, QIODevice::ReadOnly);
	char data2[2];
	char data4[4];

	if(	stream.readRawData(data4, 4) < 4 || !(QString::fromLatin1(data4, 4).toLower() == "riff" || QString::fromLatin1(data4, 4).toLower() == "rifx") ||
		stream.readRawData(data4, 4) < 4 || toInt(data4) <= 0 ||
		stream.readRawData(data4, 4) < 4 || QString::fromLatin1(data4, 4).toLower() != "wave" ||
		stream.readRawData(data4, 4) < 4 || QString::fromLatin1(data4, 4).toLower() != "fmt ")
	{
		return NULL;
	}

	stream.skipRawData(4);
	if(	stream.readRawData(data2, 2) < 2 || data2[0] != 1 ||
		stream.readRawData(data2, 2) < 2 || toShort(data2) <= 0 ||
		stream.readRawData(data4, 4) < 4 || toInt(data4) <= 0 || 
		stream.readRawData(data4, 4) < 4 || toInt(data4) <= 0 || 
		stream.readRawData(data2, 2) < 2 || toShort(data2) <= 0 ||
		stream.readRawData(data2, 2) < 2 || toShort(data2) <= 0)
	{
		return NULL;
	}

	return &ViWaveCodec::instance();
}

QByteArray& ViWaveCoder::header()
{
	mHeader.clear();
	QDataStream stream((QByteArray*) &mHeader, QIODevice::WriteOnly);

	if(mOutputFormat.byteOrder() == ViAudioFormat::LittleEndian)
	{
		stream.setByteOrder(QDataStream::LittleEndian);
		stream << qint8('R') << qint8('I') << qint8('F') << qint8('F');
	}
	else if(mOutputFormat.byteOrder() == ViAudioFormat::BigEndian)
	{
		stream.setByteOrder(QDataStream::BigEndian);
		stream << qint8('R') << qint8('I') << qint8('F') << qint8('X');
	}
	stream << int((mSamples * mOutputFormat.sampleSize() / 8) - 8);
	stream << qint8('W') << qint8('A') << qint8('V') << qint8('E');
	
	stream << qint8('f') << qint8('m') << qint8('t') << qint8(' ');
	stream << int(16);
	stream << short(1);
	stream << short(mOutputFormat.channelCount());
	stream << int(mOutputFormat.sampleRate());
	stream << int(mOutputFormat.sampleRate() * mOutputFormat.channelCount() * mOutputFormat.sampleSize() / 8);
	stream << short(mOutputFormat.channelCount() * mOutputFormat.sampleSize() / 8);
	stream << short(mOutputFormat.sampleSize());

	stream << qint8('d') << qint8('a') << qint8('t') << qint8('a');
	stream << int(mSamples * mOutputFormat.sampleSize() / 8);

	return mHeader;
}

int ViWaveCoder::headerSize()
{
	return 44;
}

bool ViWaveCoder::initializeDecode()
{
	setError(ViCoder::NoError);
	decodePointer = &ViWaveCoder::decodeHeader;
	return true;
}

bool ViWaveCoder::finalizeDecode()
{
	return true;
}

void ViWaveCoder::decode(const void *input, int size)
{
	(this->*decodePointer)(input, size);
}

void ViWaveCoder::decodeHeader(const void *input, int size)
{
	if(size >= 44)
	{
		decodePointer = &ViWaveCoder::decodeData;

		QString string;
		QTextStream stream(&string);
		char *data = (char*) input;

		stream << data[0] << data [1] << data[2] << data[3];
		if(string.toLower() == "riff")
		{
			mInputFormat.setByteOrder(ViAudioFormat::LittleEndian);
		}
		else if(string.toLower() == "rifx")
		{
			mInputFormat.setByteOrder(ViAudioFormat::BigEndian);
		}

		string.clear();
		stream << data[22] << data[23];
		mInputFormat.setChannelCount(toShort(string.toLatin1().data()));

		string.clear();
		stream << data[24] << data[25] << data[26] << data[27];
		mInputFormat.setSampleRate(toInt(string.toLatin1().data()));

		string.clear();
		stream << data[34] << data[35];
		mInputFormat.setSampleSize(toShort(string.toLatin1().data()));

		mInputFormat.setSampleType(ViAudioFormat::SignedInt);
		mInputFormat.setCodec(&ViWaveCodec::instance());

		emit formatChanged(mInputFormat);
		decodeData(data + 44, size - 44);
	}
}

void ViWaveCoder::decodeData(const void *input, int size)
{
	qbyte *data = new qbyte[size];
	memcpy(data, input, size);
	emit decoded(new ViSampleArray(data, size, size / (mInputFormat.sampleSize() / 8)));
}

bool ViWaveCoder::initializeEncode()
{
	mSamples = 0;
	int inSize = mInputFormat.sampleSize();
	int outSize = mOutputFormat.sampleSize();
	ViAudioFormat::SampleType inType = mInputFormat.sampleType();
	ViAudioFormat::SampleType outType = mOutputFormat.sampleType();
	int inputChannels = mInputFormat.channelCount();
	int outputChannels = mOutputFormat.channelCount();

	if(!(inSize == 8 || inSize == 16 || inSize == 32))
	{
		setError(ViCoder::InputSampleSizeError);
		return false;
	}
	if(!(outSize == 8 || outSize == 16 || outSize == 32))
	{
		setError(ViCoder::OutputSampleSizeError);
		return false;
	}

	if(inType == ViAudioFormat::Unknown)
	{
		setError(ViCoder::InputSampleTypeError);
		return false;
	}
	if(outType == ViAudioFormat::Unknown)
	{
		setError(ViCoder::OutputSampleTypeError);
		return false;
	}

	if(!(inputChannels == 1 || inputChannels == 2))
	{
		setError(ViCoder::InputChannelError);
		return false;
	}
	if(!(outputChannels == 1 || outputChannels == 2))
	{
		setError(ViCoder::OutputChannelError);
		return false;
	}

	if(!mConverter.initialize(mInputFormat, mOutputFormat))
	{
		setError(ViCoder::EncoderInitializationError);
		return false;
	}

	setError(ViCoder::NoError);
	return true;
}

bool ViWaveCoder::finalizeEncode()
{
	return true;
}

void ViWaveCoder::encode(const void *input, int samples)
{
	int bytes;
	char *output = (char*) mConverter.convert(input, samples, bytes);
	mSamples += samples;
	emit encoded(new ViSampleArray(output, bytes, samples));
}

ViCoder::Error ViWaveCoder::initializeLibrary()
{
	setError(ViCoder::NoError);
	return error();
}

short ViWaveCoder::toShort(char data[])
{
	short result = 0;
	memcpy(&result, data, 2);
	return result;
}

int ViWaveCoder::toInt(char data[])
{
	int result = 0;
	memcpy(&result, data, 4);
	return result;
}
