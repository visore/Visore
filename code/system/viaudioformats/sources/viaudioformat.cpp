#include "viaudioformat.h"

ViAudioFormat::ViAudioFormatCompression ViAudioFormat::compression()
{
	return mCompression;
}

QString ViAudioFormat::abbreviation()
{
	return mAbbreviation;
}

QString ViAudioFormat::name()
{
	return mName;
}

QList<QString> ViAudioFormat::extensions()
{
	return mExtensions;
}

QList<QString> ViAudioFormat::starExtensions()
{
	QList<QString> list;
	for(int i = 0; i < mExtensions.size(); ++i)
	{
		list.append("*." + mExtensions[i]);
	}
	return list;
}
