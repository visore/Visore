#include "viaudioformat.h"

void ViAudioFormat::setCodec(ViCodec codec)
{
	mCodec = codec;
	QAudioFormat::setCodec("audio/" + mCodec.abbreviation().toLower());
}

ViCodec ViAudioFormat::codec()
{
	return mCodec;
}

