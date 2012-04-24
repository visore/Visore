#include "viaudioformat.h"

ViAudioFormat::ViAudioFormat()
	: QAudioFormat()
{
	mBitRate = -1;
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
