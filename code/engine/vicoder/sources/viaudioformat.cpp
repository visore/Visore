#include "viaudioformat.h"

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
