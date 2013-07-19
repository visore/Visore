#ifndef VIAUDIOFORMAT_H
#define VIAUDIOFORMAT_H

#include <QAudioFormat>
#include <QMap>

class ViAudioCodec;

class ViFormatMap : public QMap<int, QString>
{
	
	public:

		ViFormatMap();
		void setDefault(int value);
		int defaultValue();
		QString defaultName();

	private:

		int mDefaultValue;

};

class ViAudioBitrate
{

	public:

		enum Type
		{
			Normal,
			Minimum,
			Maximum
		};

		enum Mode
		{
			Unknown,
			Constant,
			Variable,
			Average
		};

		ViAudioBitrate(const ViAudioBitrate::Mode mode = ViAudioBitrate::Unknown, const int normal = 0, const int minimum = 0, const int maximum = 0);

		void setMode(const ViAudioBitrate::Mode mode);
		void setRate(const int rate, const ViAudioBitrate::Type type = ViAudioBitrate::Normal);
		void setNormal(const int rate);
		void setMinimum(const int rate);
		void setMaximum(const int rate);

		ViAudioBitrate::Mode mode() const;
		int rate(const ViAudioBitrate::Type type = ViAudioBitrate::Normal) const;
		int normal() const;
		int minimum() const;
		int maximum() const;

		QString toString();

		static QString toString(const ViAudioBitrate::Mode &mode);

	private:

		ViAudioBitrate::Mode mMode;
		int mNormal;
		int mMinimum;
		int mMaximum;

};

class ViAudioFormat
{

	public:

		enum SampleType
		{
			Unknown,
			SignedInt,
			UnSignedInt,
			Float,
			Double,
			Real = Double
		};

		enum Endian
		{
			BigEndian,
			LittleEndian
		};

		/*
			Depended on codec:
			 1. File size might be larger on higher quality, because samples were compressed less and therefore less information loss (typically lossy codecs).
			 2. File size might be smaller on higher quality, because samples were compressed more without information loss - longer execution time (typcially lossless codecs).

			Hence these values might indicate the quality of sound for lossy codecs, or the level of compression for lossless codecs (higher quality indicates higher compression).
		*/
		enum Quality
		{
			None,
			InsaneHigh,
			ExtremeHigh,
			VeryHigh,
			High,
			AboveAverage,
			Average,
			BelowAverage,
			Low,
			VeryLow,
			ExtremeLow,

			Maximum = InsaneHigh, //Internal use only
			Minimum = ExtremeLow //Internal use only
		};

		ViAudioFormat();
		ViAudioFormat(const ViAudioFormat &other);
		ViAudioFormat(const QAudioFormat &other);

		bool isValid(bool includingCodec = false) const;
		bool hasCodec() const;

		ViAudioFormat::SampleType sampleType() const;
		ViAudioFormat::Endian byteOrder() const;
		ViAudioFormat::Quality quality() const;
		ViAudioBitrate bitrate() const;
		ViAudioBitrate::Mode bitrateMode() const;
		int bitrate(const ViAudioBitrate::Type type) const;
		int sampleSize() const;
		int sampleRate() const;
		int channelCount() const;
		int channels() const;
		ViAudioCodec* codec() const;
		
		void setSampleType(const ViAudioFormat::SampleType type);
		void setSampleType(const QAudioFormat::SampleType type);
		void setByteOrder(const ViAudioFormat::Endian order);
		void setByteOrder(const QAudioFormat::Endian order);
		void setQuality(const ViAudioFormat::Quality quality);
		void setBitrate(const ViAudioBitrate bitrate);
		void setBitrateMode(const ViAudioBitrate::Mode mode);
		void setBitrate(const int rate, const ViAudioBitrate::Type type = ViAudioBitrate::Normal);
		void setSampleSize(const int size);
		void setSampleRate(const int rate);
		void setChannelCount(const int channels);
		void setChannels(const int channels);
		void setCodec(ViAudioCodec *codec);
		bool setCodec(QString codecName);

		QAudioFormat toQAudioFormat();
		static ViAudioFormat defaultFormat();

		static ViFormatMap supportedSampleSizes();
		static ViFormatMap supportedSampleRates();
		static ViFormatMap supportedSampleTypes();
		static ViFormatMap supportedEndianness();
		static ViFormatMap supportedBitrates();
		static ViFormatMap supportedBitrateModes();
		static ViFormatMap supportedQualities();
		static ViFormatMap supportedChannels();

		QString toString();

		static QString toString(const ViAudioFormat::SampleType &type);
		static QString toString(const ViAudioFormat::Endian &order);
		static QString toString(const ViAudioFormat::Quality &quality);
		static QString toStringChannels(const int &channels);

	private:

		ViAudioFormat::SampleType mSampleType;
		ViAudioFormat::Endian mByteOrder;
		ViAudioFormat::Quality mQuality;
		ViAudioBitrate mBitrate;
		int mSampleSize;
		int mSampleRate;
		int mChannelCount;	
		ViAudioCodec *mCodec;

};

#endif
