#include "vicodec.h"

ViCodec::ViCodec()
{
	mType = ViCodec::UndefinedType;
	mCompression = ViCodec::UndefinedCompression;
	mAbbreviation = "";
	mName = "";
}

ViCodec::ViCodec(ViCodec::Type type, ViCodec::Compression compression, QString abbreviation, QString name, QList<QString> extensions)
{
	mType = type;
	mCompression = compression;
	mAbbreviation = abbreviation;
	mName = name;
	mExtensions = extensions;
}

ViCodec::Type ViCodec::type()
{
	return mType;
}

ViCodec::Compression ViCodec::compression()
{
	return mCompression;
}

QString ViCodec::abbreviation()
{
	return mAbbreviation;
}

QString ViCodec::name()
{
	return mName;
}

QList<QString> ViCodec::extensions()
{
	return mExtensions;
}

QList<QString> ViCodec::starExtensions()
{
	QList<QString> list;
	for(int i = 0; i < mExtensions.size(); ++i)
	{
		list.append("*." + mExtensions[i]);
	}
	return list;
}

bool ViCodec::operator ==(const ViCodec &other) const
{
	return mAbbreviation == other.mAbbreviation;
}
