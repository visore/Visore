#include <viaudioformat.h>
#include <viaudiomanager.h>
#include <viaudiocodec.h>

ViFormatMap::ViFormatMap()
	: QMap<int, QString>()
{
	mDefaultValue = -1;
}

void ViFormatMap::setDefault(int value)
{
	mDefaultValue = value;
}

int ViFormatMap::defaultValue()
{
	return mDefaultValue;
}

QString ViFormatMap::defaultName()
{
	return value(mDefaultValue);
}

ViAudioBitrate::ViAudioBitrate(const ViAudioBitrate::Mode mode, const int normal, const int minimum, const int maximum)
{
	mMode = mode;
	mNormal = normal;
	mMinimum = minimum;
	mMaximum = maximum;
}

void ViAudioBitrate::setMode(const ViAudioBitrate::Mode mode)
{
	mMode = mode;
}

void ViAudioBitrate::setRate(const int rate, const ViAudioBitrate::Type type)
{
	if(type == ViAudioBitrate::Normal)
	{
		setNormal(rate);
	}
	else if(type == ViAudioBitrate::Minimum)
	{
		setMinimum(rate);
	}
	else if(type == ViAudioBitrate::Maximum)
	{
		setMaximum(rate);
	}
}

void ViAudioBitrate::setNormal(const int rate)
{
	mNormal = rate;
}

void ViAudioBitrate::setMinimum(const int rate)
{
	mMinimum = rate;
}

void ViAudioBitrate::setMaximum(const int rate)
{
	mMaximum = rate;
}

ViAudioBitrate::Mode ViAudioBitrate::mode() const
{
	return mMode;
}

int ViAudioBitrate::rate(const ViAudioBitrate::Type type) const
{
	if(type == ViAudioBitrate::Normal)
	{
		return normal();
	}
	else if(type == ViAudioBitrate::Minimum)
	{
		return minimum();
	}
	else if(type == ViAudioBitrate::Maximum)
	{
		return maximum();
	}
}

int ViAudioBitrate::normal() const
{
	return mNormal;
}

int ViAudioBitrate::minimum() const
{
	return mMinimum;
}

int ViAudioBitrate::maximum() const
{
	return mMaximum;
}

QString ViAudioBitrate::toString()
{
	QString result = "";

	if(mMode == ViAudioBitrate::Unknown) result += "Unknown";
	else if(mMode == ViAudioBitrate::Constant) result += "Constant";
	else if(mMode == ViAudioBitrate::Variable) result += "Variable";
	else if(mMode == ViAudioBitrate::Average) result += "Average";
	result += " Bitrate ";

	result += "(normal: " + QString::number(mNormal) + ", minimum: " + QString::number(mMinimum) + ", maximum: " + QString::number(mMaximum) + ")";

	return result;
}

ViAudioFormat::ViAudioFormat()
{
	mSampleType = ViAudioFormat::Unknown;
	mByteOrder = ViAudioFormat::LittleEndian;
	mQuality = ViAudioFormat::Average;
	mSampleSize = 0;
	mSampleRate = 0;
	mChannelCount = 0;
	mCodec = NULL;
}

ViAudioFormat::ViAudioFormat(const ViAudioFormat &other)
{
	mSampleType = other.mSampleType;
	mByteOrder = other.mByteOrder;
	mQuality = other.mQuality;
	mBitrate = other.mBitrate;
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
	
	mQuality = ViAudioFormat::Average;
	mSampleSize = other.sampleSize();
	mSampleRate = other.sampleRate();
	mChannelCount = other.channelCount();
	mCodec = NULL;
}

bool ViAudioFormat::isValid(bool includingCodec)
{
	ViFormatMap sizes = supportedSampleSizes();
	if(!sizes.contains(mSampleSize))
	{
		return false;
	}
	ViFormatMap rates = supportedSampleRates();
	if(!rates.contains(mSampleRate))
	{
		return false;
	}
	ViFormatMap types = supportedSampleTypes();
	if(!types.contains(mSampleType))
	{
		return false;
	}
	ViFormatMap endianness = supportedEndianness();
	if(!endianness.contains(mByteOrder))
	{
		return false;
	}
	ViFormatMap channels = supportedChannels();
	if(!channels.contains(mChannelCount))
	{
		return false;
	}
	if(includingCodec && !hasCodec())
	{
		return false;
	}
	return true;
}

bool ViAudioFormat::hasCodec()
{
	return mCodec != NULL;
}

ViAudioFormat::SampleType ViAudioFormat::sampleType() const
{
	return mSampleType;
}

ViAudioFormat::Endian ViAudioFormat::byteOrder() const
{
	return mByteOrder;
}

ViAudioFormat::Quality ViAudioFormat::quality() const
{
	return mQuality;
}

ViAudioBitrate ViAudioFormat::bitrate() const
{
	return mBitrate;
}

ViAudioBitrate::Mode ViAudioFormat::bitrateMode() const
{
	return mBitrate.mode();
}

int ViAudioFormat::bitrate(const ViAudioBitrate::Type type) const
{
	if(type == ViAudioBitrate::Minimum)
	{
		return mBitrate.minimum();
	}
	else if(type == ViAudioBitrate::Maximum)
	{
		return mBitrate.maximum();
	}
	else
	{
		return mBitrate.normal();
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

void ViAudioFormat::setQuality(const ViAudioFormat::Quality quality)
{
	mQuality = quality;
}

void ViAudioFormat::setBitrate(const ViAudioBitrate bitrate)
{
	mBitrate = bitrate;
}

void ViAudioFormat::setBitrateMode(const ViAudioBitrate::Mode mode)
{
	mBitrate.setMode(mode);
}

void ViAudioFormat::setBitrate(const int rate, const ViAudioBitrate::Type type)
{
	if(type == ViAudioBitrate::Minimum)
	{
		mBitrate.setMinimum(rate);
	}
	else if(type == ViAudioBitrate::Maximum)
	{
		mBitrate.setMaximum(rate);
	}
	else
	{
		mBitrate.setNormal(rate);
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
	mCodec = ViAudioManager::codec(codecName, ViAudioManager::Supported);
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

	format.setCodec("audio/pcm");
	
	return format;
}

ViAudioFormat ViAudioFormat::defaultFormat()
{
	ViAudioFormat format;
	format.setSampleSize(supportedSampleSizes().defaultValue());
	format.setSampleRate(supportedSampleRates().defaultValue());
	format.setSampleType(ViAudioFormat::SampleType(supportedSampleTypes().defaultValue()));
	format.setChannelCount(supportedChannels().defaultValue());
	format.setBitrate(ViAudioBitrate(ViAudioBitrate::Mode(supportedBitrateModes().defaultValue()), supportedBitrates().defaultValue()));
	format.setByteOrder(ViAudioFormat::Endian(supportedEndianness().defaultValue()));
	format.setQuality(ViAudioFormat::Quality(supportedQualities().defaultValue()));
	format.setCodec(ViAudioManager::codec("WAVE"));
	return format;
}

ViFormatMap ViAudioFormat::supportedSampleSizes()
{
	static ViFormatMap map;
	if(map.isEmpty())
	{
		map.insert(8, "8 bit");
		map.insert(16, "16 bit");
		map.insert(32, "32 bit");
		map.setDefault(16);
	}
	return map;
}

ViFormatMap ViAudioFormat::supportedSampleRates()
{
	static ViFormatMap map;
	if(map.isEmpty())
	{
		map.insert(8000, "8000 Hz");
		map.insert(11025, "11025 Hz");
		map.insert(16000, "16000 Hz");
		map.insert(22050, "22050 Hz");
		map.insert(32000, "32000 Hz");
		map.insert(44056, "44056 Hz");
		map.insert(44100, "44100 Hz");
		map.insert(47250, "47250 Hz");
		map.insert(48000, "48000 Hz");
		map.insert(50000, "50000 Hz");
		map.insert(50400, "50400 Hz");
		map.insert(88200, "88200 Hz");
		map.insert(96000, "96000 Hz");
		map.setDefault(44100);
	}
	return map;
}

ViFormatMap ViAudioFormat::supportedSampleTypes()
{
	static ViFormatMap map;
	if(map.isEmpty())
	{
		map.insert(ViAudioFormat::SignedInt, "Signed Integer");
		map.insert(ViAudioFormat::UnSignedInt, "Unsigned Integer");
		map.insert(ViAudioFormat::Float, "Float");
		map.setDefault(ViAudioFormat::SignedInt);
	}
	return map;
}

ViFormatMap ViAudioFormat::supportedEndianness()
{
	static ViFormatMap map;
	if(map.isEmpty())
	{
		map.insert(ViAudioFormat::LittleEndian, "Little Endian");
		map.insert(ViAudioFormat::BigEndian, "Big Endian");
		map.setDefault(ViAudioFormat::LittleEndian);
	}
	return map;
}

ViFormatMap ViAudioFormat::supportedBitrates()
{
	static ViFormatMap map;
	if(map.isEmpty())
	{
		map.insert(8, "8 kbps");
		map.insert(16, "16 kbps");
		map.insert(24, "24 kbps");
		map.insert(32, "32 kbps");
		map.insert(40, "40 kbps");
		map.insert(48, "48 kbps");
		map.insert(56, "56 kbps");
		map.insert(64, "64 kbps");
		map.insert(72, "72 kbps");
		map.insert(80, "80 kbps");
		map.insert(88, "88 kbps");
		map.insert(96, "96 kbps");
		map.insert(104, "104 kbps");
		map.insert(112, "112 kbps");
		map.insert(120, "120 kbps");
		map.insert(128, "128 kbps");
		map.insert(136, "136 kbps");
		map.insert(144, "144 kbps");
		map.insert(152, "152 kbps");
		map.insert(160, "160 kbps");
		map.insert(168, "168 kbps");
		map.insert(176, "176 kbps");
		map.insert(184, "184 kbps");
		map.insert(192, "192 kbps");
		map.insert(200, "200 kbps");
		map.insert(208, "208 kbps");
		map.insert(216, "216 kbps");
		map.insert(224, "224 kbps");
		map.insert(232, "232 kbps");
		map.insert(240, "240 kbps");
		map.insert(248, "248 kbps");
		map.insert(256, "256 kbps");
		map.insert(264, "264 kbps");
		map.insert(272, "272 kbps");
		map.insert(280, "280 kbps");
		map.insert(288, "288 kbps");
		map.insert(296, "296 kbps");
		map.insert(304, "304 kbps");
		map.insert(312, "312 kbps");
		map.insert(320, "320 kbps");
		map.insert(330, "330 kbps");
		map.insert(340, "340 kbps");
		map.insert(350, "350 kbps");
		map.insert(360, "360 kbps");
		map.insert(370, "370 kbps");
		map.insert(380, "380 kbps");
		map.insert(390, "390 kbps");
		map.insert(400, "400 kbps");
		map.insert(410, "410 kbps");
		map.insert(420, "420 kbps");
		map.insert(430, "430 kbps");
		map.insert(440, "440 kbps");
		map.insert(450, "450 kbps");
		map.insert(460, "460 kbps");
		map.insert(470, "470 kbps");
		map.insert(480, "480 kbps");
		map.insert(490, "490 kbps");
		map.insert(500, "500 kbps");
		map.setDefault(192);
	}
	return map;
}

ViFormatMap ViAudioFormat::supportedBitrateModes()
{
	static ViFormatMap map;
	if(map.isEmpty())
	{
		map.insert(ViAudioBitrate::Constant, "Constant Bitrate");
		map.insert(ViAudioBitrate::Variable, "Variable Bitrate");
		map.insert(ViAudioBitrate::Average, "Average Bitrate");
		map.setDefault(ViAudioBitrate::Variable);
	}
	return map;
}

ViFormatMap ViAudioFormat::supportedQualities()
{
	static ViFormatMap map;
	if(map.isEmpty())
	{
		map.insert(ViAudioFormat::InsaneHigh, "Insane High");
		map.insert(ViAudioFormat::ExtremeHigh, "Extreme High");
		map.insert(ViAudioFormat::VeryHigh, "Very High");
		map.insert(ViAudioFormat::High, "High");
		map.insert(ViAudioFormat::AboveAverage, "Above Average");
		map.insert(ViAudioFormat::Average, "Average");
		map.insert(ViAudioFormat::BelowAverage, "Below Average");
		map.insert(ViAudioFormat::Low, "Low");
		map.insert(ViAudioFormat::VeryLow, "Very Low");
		map.insert(ViAudioFormat::ExtremeLow, "Extreme Low");
		map.setDefault(ViAudioFormat::High);
	}
	return map;
}

ViFormatMap ViAudioFormat::supportedChannels()
{
	static ViFormatMap map;
	if(map.isEmpty())
	{
		map.insert(1, "Mono");
		map.insert(2, "Stereo");
		map.setDefault(2);
	}
	return map;
}

QString ViAudioFormat::toString()
{
	QString result = "";

	result += "Sample Type: ";
	if(mSampleType == ViAudioFormat::SignedInt) result += "Signed Integer";
	else if(mSampleType == ViAudioFormat::SignedInt) result += "Signed Integer";
	else if(mSampleType == ViAudioFormat::UnSignedInt) result += "Unsigned Integer";
	else if(mSampleType == ViAudioFormat::Float) result += "Float";
	else if(mSampleType == ViAudioFormat::Real) result += "Real";
	result += "\n";

	result += "Sample Size: " + QString::number(mSampleSize) + "\n";

	result += "Sample Rate: " + QString::number(mSampleRate) + "\n";

	result += "Channel Count: " + QString::number(mChannelCount) + "\n";

	result += "Byte Order: ";
	if(mByteOrder == ViAudioFormat::BigEndian) result += "Big Endian";
	else if(mByteOrder == ViAudioFormat::LittleEndian) result += "Little Endian";
	result += "\n";

	result += "Codec: " + mCodec->toString() + "\n";

	result += "Bitrate: " + mBitrate.toString() + "\n";

	result += "Quality: ";
	if(mQuality == ViAudioFormat::InsaneHigh) result += "Insane High";
	else if(mQuality == ViAudioFormat::ExtremeHigh) result += "Extreme High";
	else if(mQuality == ViAudioFormat::VeryHigh) result += "Very High";
	else if(mQuality == ViAudioFormat::High) result += "High";
	else if(mQuality == ViAudioFormat::AboveAverage) result += "Above Average";
	else if(mQuality == ViAudioFormat::Average) result += "Average";
	else if(mQuality == ViAudioFormat::BelowAverage) result += "Below Average";
	else if(mQuality == ViAudioFormat::Low) result += "Low";
	else if(mQuality == ViAudioFormat::VeryLow) result += "Very Low";
	else if(mQuality == ViAudioFormat::ExtremeLow) result += "Extreme Low";
	result += "\n";

	return result;
}
