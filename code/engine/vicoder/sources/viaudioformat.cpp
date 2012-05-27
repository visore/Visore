#include "viaudioformat.h"
#include "vicodecmanager.h"

ViAudioFormat::ViAudioFormat()
	: QAudioFormat()
{
	mBitRate = -1;
}

ViAudioFormat::ViAudioFormat(const ViAudioFormat &format)
	: QAudioFormat(format)
{
	mCodec = ViCodec(format.mCodec);
	mBitRate = format.mBitRate;
}

ViAudioFormat ViAudioFormat::defaultFormat()
{
	ViAudioFormat result;
	result.setBitRate(128);
	result.setSampleSize(16);
	result.setSampleRate(44100);
	result.setSampleType(QAudioFormat::SignedInt);
	result.setCodec(ViCodecManager::selected("WAV"));
	result.setChannelCount(2);
	result.setByteOrder(QAudioFormat::LittleEndian);
	return result;
}

void ViAudioFormat::setCodec(ViCodec codec)
{
	mCodec = codec;
	QAudioFormat::setCodec("audio/" + mCodec.abbreviation().toLower());
}

ViCodec ViAudioFormat::codec()
{
	return mCodec;
}

void ViAudioFormat::setBitRate(int bitRate)
{
	mBitRate = bitRate;
}

int ViAudioFormat::bitRate()
{
	return mBitRate;
}
