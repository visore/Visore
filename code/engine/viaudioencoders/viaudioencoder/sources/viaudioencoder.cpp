#include "viaudioencoder.h"

QSharedPointer<ViAudioEncoder> ViAudioEncoder::mInstance;

ViAudioEncoder::ViAudioEncoder()
{
	mName = "";
	mPath = "";
	mFormat = NULL;
}

ViAudioEncoder::~ViAudioEncoder()
{
	for(int i = 0; i < mParameters.size(); ++i)
	{
		if(mParameters[i] != NULL)
		{
			delete mParameters[i];
			mParameters[i] = NULL;
		}
	}
}

ViAudioEncoder* ViAudioEncoder::instance()
{
	return NULL;
}

QString ViAudioEncoder::name()
{
	return mName;
}

QString ViAudioEncoder::path()
{
	return mPath;
}

QList<ViEncoderParameter*> ViAudioEncoder::parameters()
{
	return mParameters;
}

ViAudioFormat* ViAudioEncoder::format()
{
	return mFormat;
}

ViEncoderParameter* ViAudioEncoder::parameter(QString name)
{
	for(int i = 0; i < mParameters.size(); ++i)
	{
		if(mParameters[i]->name() == name)
		{
			return mParameters[i];
		}
	}
	return NULL;
}

void ViAudioEncoder::setPath(QString path)
{
	mPath = path;
}
