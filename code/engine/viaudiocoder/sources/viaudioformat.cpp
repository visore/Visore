#include <viaudioformat.h>
#include <viaudiomanager.h>

ViAudioFormat::ViAudioFormat()
{
	mSampleType = ViAudioFormat::Unknown;
	mByteOrder = ViAudioFormat::LittleEndian;
	mViuality = ViAudioFormat::Average;
	mBitrateMode = ViAudioFormat::ConstantBitrate;
	mNormalBitrate = 256;
	mMinimumBitrate = 256;
	mMaximumBitrate = 256;
	mSampleSize = 16;
	mSampleRate = 44100;
	mChannelCount = 2;
	mCodec = NULL;
}

ViAudioFormat::ViAudioFormat(const ViAudioFormat &other)
{
	mSampleType = other.mSampleType;
	mByteOrder = other.mByteOrder;
	mViuality = other.mViuality;
	mBitrateMode = other.mBitrateMode;
	mNormalBitrate = other.mNormalBitrate;
	mMinimumBitrate = other.mMinimumBitrate;
	mMaximumBitrate = other.mMaximumBitrate;
	mSampleSize = other.mSampleSize;
	mSampleRate = other.mSampleRate;
	mChannelCount = other.mChannelCount;
	mCodec = other.mCodec;
}

ViAudioFormat::ViAudioFormat(const QAudioFormat &other)
{
	if(other.sampleType() == QAudioFormat::SignedInt)
	{
		setSampleType(ViAudioFormat::SignedInt);
	}
	else if(other.sampleType() == QAudioFormat::UnSignedInt)
	{
		setSampleType(ViAudioFormat::UnSignedInt);
	}
	else if(other.sampleType() == QAudioFormat::Float)
	{
		setSampleType(ViAudioFormat::Float);
	}
	else
	{
		setSampleSize(ViAudioFormat::Unknown);
	}

	if(other.byteOrder() == QAudioFormat::BigEndian)
	{
		setByteOrder(ViAudioFormat::BigEndian);
	}
	else
	{
		setByteOrder(ViAudioFormat::LittleEndian);
	}
	
	mViuality = ViAudioFormat::Average;
	mBitrateMode = ViAudioFormat::ConstantBitrate;
	mNormalBitrate = 256;
	mMinimumBitrate = 256;
	mMaximumBitrate = 256;
	mSampleSize = other.sampleSize();
	mSampleRate = other.sampleRate();
	mChannelCount = other.channelCount();
	mCodec = NULL;
}

ViAudioFormat::SampleType ViAudioFormat::sampleType() const
{
	return mSampleType;
}

ViAudioFormat::Endian ViAudioFormat::byteOrder() const
{
	return mByteOrder;
}

ViAudioFormat::Viuality ViAudioFormat::viuality() const
{
	return mViuality;
}

ViAudioFormat::BitrateMode ViAudioFormat::bitrateMode() const
{
	return mBitrateMode;
}

int ViAudioFormat::bitrate(const ViAudioFormat::BitrateType type) const
{
	if(type == ViAudioFormat::MinimumBitrate)
	{
		return mMinimumBitrate;
	}
	else if(type == ViAudioFormat::MaximumBitrate)
	{
		return mMaximumBitrate;
	}
	else
	{
		return mNormalBitrate;
	}
}

int ViAudioFormat::sampleSize() const
{
	return mSampleSize;
}

int ViAudioFormat::sampleRate() const
{
	return mSampleRate;
}

int ViAudioFormat::channelCount() const
{
	return mChannelCount;
}

int ViAudioFormat::channels() const
{
	return mChannelCount;
}

ViAudioCodec* ViAudioFormat::codec() const
{
	return mCodec;
}
		
void ViAudioFormat::setSampleType(const ViAudioFormat::SampleType type)
{
	mSampleType = type;
}

void ViAudioFormat::setSampleType(const QAudioFormat::SampleType type)
{
	if(type == QAudioFormat::SignedInt)
	{
		mSampleType = ViAudioFormat::SignedInt;
	}
	else if(type == QAudioFormat::UnSignedInt)
	{
		mSampleType = ViAudioFormat::UnSignedInt;
	}
	else if(type == QAudioFormat::Float)
	{
		mSampleType = ViAudioFormat::Float;
	}
	else
	{
		mSampleType = ViAudioFormat::Unknown;
	}
}

void ViAudioFormat::setByteOrder(const ViAudioFormat::Endian order)
{
	mByteOrder = order;
}

void ViAudioFormat::setByteOrder(const QAudioFormat::Endian order)
{
	if(order == QAudioFormat::BigEndian)
	{
		mByteOrder = ViAudioFormat::BigEndian;
	}
	else
	{
		mByteOrder = ViAudioFormat::LittleEndian;
	}
}

void ViAudioFormat::setViuality(const ViAudioFormat::Viuality viuality)
{
	mViuality = viuality;
}

void ViAudioFormat::setBitrateMode(const ViAudioFormat::BitrateMode mode)
{
	mBitrateMode = mode;
}

void ViAudioFormat::setBitrate(const int rate, const ViAudioFormat::BitrateType type)
{
	if(type == ViAudioFormat::MinimumBitrate)
	{
		mMinimumBitrate = rate;
	}
	else if(type == ViAudioFormat::MaximumBitrate)
	{
		mMaximumBitrate = rate;
	}
	else
	{
		mNormalBitrate = rate;
	}
}

void ViAudioFormat::setSampleSize(const int size)
{
	mSampleSize = size;
}

void ViAudioFormat::setSampleRate(const int rate)
{
	mSampleRate = rate;
}

void ViAudioFormat::setChannelCount(const int channels)
{
	mChannelCount = channels;
}

void ViAudioFormat::setChannels(const int channels)
{
	mChannelCount = channels;
}

void ViAudioFormat::setCodec(ViAudioCodec *codec)
{
	mCodec = codec;
}

bool ViAudioFormat::setCodec(QString codecName)
{
	mCodec = ViAudioManager::instance().codec(codecName, ViAudioManager::Supported);
	return mCodec != NULL;
}

QAudioFormat ViAudioFormat::toQAudioFormat()
{
	QAudioFormat format;
	format.setSampleSize(mSampleSize);
	format.setSampleRate(mSampleRate);
	format.setChannelCount(mChannelCount);

	if(mSampleType == SignedInt)
	{
		format.setSampleType(QAudioFormat::SignedInt);
	}
	else if(mSampleType == UnSignedInt)
	{
		format.setSampleType(QAudioFormat::UnSignedInt);
	}
	else if(mSampleType == Float)
	{
		format.setSampleType(QAudioFormat::Float);
	}
	else
	{
		format.setSampleType(QAudioFormat::Unknown);
	}
	
	if(mByteOrder == ViAudioFormat::BigEndian)
	{
		format.setByteOrder(QAudioFormat::BigEndian);
	}
	else
	{
		format.setByteOrder(QAudioFormat::LittleEndian);
	}
	
	return format;
}
