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

void ViAudioCodec::addSampleSize(int value, QString text)
{
	mSampleSizes.insert(value, text);
}

void ViAudioCodec::addSampleRate(int value, QString text)
{
	mSampleRates.insert(value, text);
}

void ViAudioCodec::addSampleType(int value, QString text)
{
	mSampleTypes.insert(value, text);
}

void ViAudioCodec::addEndianness(int value, QString text)
{
	mEndianness.insert(value, text);
}

void ViAudioCodec::addBitrate(int value, QString text)
{
	mBitrates.insert(value, text);
}

void ViAudioCodec::addBitrateMode(int value, QString text)
{
	mBitrateModes.insert(value, text);
}

void ViAudioCodec::addQuality(int value, QString text)
{
	mQualities.insert(value, text);
}

void ViAudioCodec::addChannel(int value, QString text)
{
	mChannels.insert(value, text);
}

void ViAudioCodec::setDefaultSampleSize(int value)
{
	mSampleSizes.setDefault(value);
}

void ViAudioCodec::setDefaultSampleRate(int value)
{
	mSampleRates.setDefault(value);
}

void ViAudioCodec::setDefaultSampleType(int value)
{
	mSampleTypes.setDefault(value);
}

void ViAudioCodec::setDefaultEndianness(int value)
{
	mEndianness.setDefault(value);
}

void ViAudioCodec::setDefaultBitrate(int value)
{
	mBitrates.setDefault(value);
}

void ViAudioCodec::setDefaultBitrateMode(int value)
{
	mBitrateModes.setDefault(value);
}

void ViAudioCodec::setDefaultQuality(int value)
{
	mQualities.setDefault(value);
}

void ViAudioCodec::setDefaultChannel(int value)
{
	mChannels.setDefault(value);
}

ViFormatMap ViAudioCodec::supportedSampleSizes()
{
	return mSampleSizes;
}

ViFormatMap ViAudioCodec::supportedSampleRates()
{
	return mSampleRates;
}

ViFormatMap ViAudioCodec::supportedSampleType()
{
	return mSampleTypes;
}

ViFormatMap ViAudioCodec::supportedEndianness()
{
	return mEndianness;
}

ViFormatMap ViAudioCodec::supportedBitrates()
{
	return mBitrates;
}

ViFormatMap ViAudioCodec::supportedBitrateModes()
{
	return mBitrateModes;
}

ViFormatMap ViAudioCodec::supportedQualities()
{
	return mQualities;
}

ViFormatMap ViAudioCodec::supportedChannels()
{
	return mChannels;
}
