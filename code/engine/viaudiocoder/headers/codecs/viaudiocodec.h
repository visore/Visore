#ifndef VIAUDIOCODEC_H
#define VIAUDIOCODEC_H

#include <QStringList>
#include <QByteArray>
#include <vicoder.h>
#include <viaudioformat.h>

namespace ViAudio
{

	enum Compression
	{
		UnknownCompression = 0,
		LossyCompression = 1,
		LosslessCompression = 2
	};

};

class ViAudioCodec
{

	public:

		QString shortName() const;
		QString longName() const;
		QString name() const;
		ViAudio::Compression compression() const;
		QStringList abbreviations() const;
		QString abbreviation() const;
		QStringList extensions(const QString prefix = "") const;
		QString extension(const QString prefix = "") const;

		bool operator == (const ViAudioCodec &other) const;
		bool operator != (const ViAudioCodec &other) const;

		ViFormatMap supportedSampleSizes();
		ViFormatMap supportedSampleRates();
		ViFormatMap supportedSampleType();
		ViFormatMap supportedEndianness();
		ViFormatMap supportedBitrates();
		ViFormatMap supportedBitrateModes();
		ViFormatMap supportedQualities();
		ViFormatMap supportedChannels();

	protected:

		ViAudioCodec();

		void setShortName(const QString name);
		void setLongName(const QString name);
		void setCompression(const ViAudio::Compression compression);
		void addAbbreviation(const QString abbreviation);
		void addExtension(const QString extension);

		void addSampleSize(int value, QString text);
		void addSampleRate(int value, QString text);
		void addSampleType(int value, QString text);
		void addEndianness(int value, QString text);
		void addBitrate(int value, QString text);
		void addBitrateMode(int value, QString text);
		void addQuality(int value, QString text);
		void addChannel(int value, QString text);

		void setDefaultSampleSize(int value);
		void setDefaultSampleRate(int value);
		void setDefaultSampleType(int value);
		void setDefaultEndianness(int value);
		void setDefaultBitrate(int value);
		void setDefaultBitrateMode(int value);
		void setDefaultQuality(int value);
		void setDefaultChannel(int value);

	private:

		QString mShortName;
		QString mLongName;
		ViAudio::Compression mCompression;
		QStringList mAbbreviations;
		QStringList mExtensions;

		ViFormatMap mSampleSizes;
		ViFormatMap mSampleRates;
		ViFormatMap mSampleTypes;
		ViFormatMap mEndianness;
		ViFormatMap mBitrates;
		ViFormatMap mBitrateModes;
		ViFormatMap mQualities;
		ViFormatMap mChannels;
		
};

template <typename T>
class ViAudioCodecHolder : public ViAudioCodec
{

	public:

		static T& instance()
		{
			static T instance;
			if(!instance.mWasInitialized)
			{
				mWasInitialized = true;
				ViAudioCodecHolder<T>& dummy = instance;
				dummy.initialize();
			}
			return instance;
		}

	protected:

		virtual void initialize() = 0;

	private:

		static bool mWasInitialized;
		
};

template <typename T>
bool ViAudioCodecHolder<T>::mWasInitialized = false;

#endif
