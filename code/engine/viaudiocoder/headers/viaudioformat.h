#ifndef VIAUDIOFORMAT_H
#define VIAUDIOFORMAT_H

#include <viaudiocodec.h>
#include <QAudioFormat>

class ViAudioFormat
{

	public:

		enum SampleType
		{
			Unknown = 0,
			SignedInt = 1,
			UnSignedInt = 2,
			Float = 3,
			Double = 4,
			Real = Double
		};

		enum Endian
		{
			BigEndian = 0,
			LittleEndian = 1
		};

		enum BitrateMode
		{
			ConstantBitrate = 0,
			VariableBitrate = 1,
			AverageBitrate = 2
		};

		enum BitrateType
		{
			NormalBitrate = 0,
			MinimumBitrate = 1,
			MaximumBitrate = 2
		};

		/*
			Depended on codec:
			 1. File size might be larger on higher viuality, because samples were compressed less and therefore less information loss (typically lossy codecs).
			 2. File size might be smaller on higher viuality, because samples were compressed more without information loss - longer execution time (typcially lossless codecs).

			Hence these values might indicate the viuality of sound for lossy codecs, or the level of compression for lossless codecs (higher viuality indicates higher compression).
		*/
		enum Viuality
		{
			InsaneHigh = 0,
			ExtremeHigh = 1,
			VeryHigh = 2,
			High = 3,
			AboveAverage = 4,
			Average = 5,
			BelowAverage = 6,
			Low = 7,
			VeryLow = 8,
			ExtremeLow = 9,

			Maximum = InsaneHigh, //Internal use only
			Minimum = ExtremeLow //Internal use only
		};

		ViAudioFormat();
		ViAudioFormat(const ViAudioFormat &other);
		ViAudioFormat(const QAudioFormat &other);

		ViAudioFormat::SampleType sampleType() const;
		ViAudioFormat::Endian byteOrder() const;
		ViAudioFormat::Viuality viuality() const;
		ViAudioFormat::BitrateMode bitrateMode() const;
		int bitrate(const ViAudioFormat::BitrateType type = ViAudioFormat::NormalBitrate) const;
		int sampleSize() const;
		int sampleRate() const;
		int channelCount() const;
		int channels() const;
		ViAudioCodec* codec() const;
		
		void setSampleType(const ViAudioFormat::SampleType type);
		void setSampleType(const QAudioFormat::SampleType type);
		void setByteOrder(const ViAudioFormat::Endian order);
		void setByteOrder(const QAudioFormat::Endian order);
		void setViuality(const ViAudioFormat::Viuality viuality);
		void setBitrateMode(const ViAudioFormat::BitrateMode mode);
		void setBitrate(const int rate, const ViAudioFormat::BitrateType type = ViAudioFormat::NormalBitrate);
		void setSampleSize(const int size);
		void setSampleRate(const int rate);
		void setChannelCount(const int channels);
		void setChannels(const int channels);
		void setCodec(ViAudioCodec *codec);
		bool setCodec(QString codecName);

		QAudioFormat toQAudioFormat();
		static ViAudioFormat defaultFormat();

	private:

		ViAudioFormat::SampleType mSampleType;
		ViAudioFormat::Endian mByteOrder;
		ViAudioFormat::Viuality mViuality;
		ViAudioFormat::BitrateMode mBitrateMode;
		int mNormalBitrate;
		int mMinimumBitrate;
		int mMaximumBitrate;
		int mSampleSize;
		int mSampleRate;
		int mChannelCount;	
		ViAudioCodec *mCodec;

};

#endif
