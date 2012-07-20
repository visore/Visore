#include <viaudiocodec.h>
#include <viaudioformat.h>

ViAudioCodec::ViAudioCodec()
{
	mShortName = "";
	mLongName = "";
	mCompression = ViAudio::UnknownCompression;
}

QString ViAudioCodec::shortName() const
{
	return mShortName;
}

QString ViAudioCodec::longName() const
{
	return mLongName;
}

QString ViAudioCodec::name() const
{
	return mShortName;
}

ViAudio::Compression ViAudioCodec::compression() const
{
	return mCompression;
}

QStringList ViAudioCodec::abbreviations() const
{
	return mAbbreviations;
}

QString ViAudioCodec::abbreviation() const
{
	if(mAbbreviations.size() > 0)
	{
		return mAbbreviations[0];
	}
	return "";
}

QStringList ViAudioCodec::extensions(const QString prefix) const
{
	QStringList result;
	for(int i = 0; i < mExtensions.size(); ++i)
	{
		result.append(prefix + mExtensions[i]);
	}
	return result;
}

QString ViAudioCodec::extension(const QString prefix) const
{
	if(mExtensions.size() > 0)
	{
		return prefix + mExtensions[0];
	}
	return "";
}

void ViAudioCodec::setShortName(const QString name)
{
	mShortName = name;
}

void ViAudioCodec::setLongName(const QString name)
{
	mLongName = name;
}

void ViAudioCodec::setCompression(const ViAudio::Compression compression)
{
	mCompression = compression;
}

void ViAudioCodec::addAbbreviation(const QString abbreviation)
{
	mAbbreviations.append(abbreviation);
}

void ViAudioCodec::addExtension(const QString extension)
{
	mExtensions.append(extension);
}

bool ViAudioCodec::operator == (const ViAudioCodec &other) const
{
	return mShortName.trimmed().toLower() == other.mShortName.trimmed().toLower();
}

bool ViAudioCodec::operator != (const ViAudioCodec &other) const
{
	return mShortName.trimmed().toLower() != other.mShortName.trimmed().toLower();
}
