#ifndef VIAUDIOCODEC_H
#define VIAUDIOCODEC_H

#include <QStringList>
#include <QByteArray>
#include <viaudioinfo.h>
#include <vicoder.h>

class ViAudioFormat;

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

	protected:

		ViAudioCodec();

		void setShortName(const QString name);
		void setLongName(const QString name);
		void setCompression(const ViAudio::Compression compression);
		void addAbbreviation(const QString abbreviation);
		void addExtension(const QString extension);

	private:

		QString mShortName;
		QString mLongName;
		ViAudio::Compression mCompression;
		QStringList mAbbreviations;
		QStringList mExtensions;
		
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
