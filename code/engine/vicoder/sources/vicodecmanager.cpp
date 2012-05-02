#include "vicodecmanager.h"

QSharedPointer<ViCodecManager> ViCodecManager::mInstance;

ViCodecManager::ViCodecManager()
{
	populateCodecs();
	populateFormats();
}

ViCodecManager* ViCodecManager::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViCodecManager>(new ViCodecManager());
	}
	return mInstance.data();
}

void ViCodecManager::populateCodecs()
{
	QList<QString> mp3Extensions;
	mp3Extensions << "mp3" << "bit";
	mCodecs.append(ViCodec(ViCodec::InputType, ViCodec::LossyCompression, "MP3", "Moving Picture Experts Group (MPEG) Audio Layer 3", mp3Extensions));

	QList<QString> wavExtensions;
	wavExtensions << "wav" << "wave";
	mCodecs.append(ViCodec(ViCodec::InputOutputType, ViCodec::LosslessCompression, "WAV", "Waveform Audio File Format", wavExtensions));

	QList<QString> flacExtensions;
	flacExtensions << "flac";
	mCodecs.append(ViCodec(ViCodec::InputType, ViCodec::LosslessCompression, "FLAC", "Free Lossless Audio Codec", flacExtensions));

	QList<QString> ac3Extensions;
	ac3Extensions << "ac3";
	mCodecs.append(ViCodec(ViCodec::InputType, ViCodec::LossyCompression, "AC3", "Dolby Digital Audio Codec 3", ac3Extensions));
}

void ViCodecManager::populateFormats()
{
	mByteOrders.append(QAudioFormat::LittleEndian);
	mByteOrders.append(QAudioFormat::BigEndian);

	mSampleTypes.append(QAudioFormat::SignedInt);
	mSampleTypes.append(QAudioFormat::UnSignedInt);
	mSampleTypes.append(QAudioFormat::Float);

	mSampleRates.append(8000);
	mSampleRates.append(11025);
	mSampleRates.append(16000);
	mSampleRates.append(22050);
	mSampleRates.append(32000);
	mSampleRates.append(44056);
	mSampleRates.append(44100);
	mSampleRates.append(47250);
	mSampleRates.append(48000);
	mSampleRates.append(50000);
	mSampleRates.append(50400);
	mSampleRates.append(88200);
	mSampleRates.append(96000);

	mSampleSizes.append(8);
	mSampleSizes.append(16);
	mSampleSizes.append(24);
	mSampleSizes.append(32);
	mSampleSizes.append(64);

	mChannels.append(1);
	mChannels.append(2);
}

QList<ViCodec> ViCodecManager::all()
{
	return ViCodecManager::instance()->mCodecs;
}

QList<ViCodec> ViCodecManager::selected(ViCodec::Type type)
{
	ViCodecManager *manager = ViCodecManager::instance();
	QList<ViCodec> result;
	for(int i = 0; i < manager->mCodecs.size(); ++i)
	{
		if(manager->mCodecs[i].type() == type)
		{
			result.append(manager->mCodecs[i]);
		}
	}
	return result;
}

QList<ViCodec> ViCodecManager::selected(ViCodec::Compression compression)
{
	ViCodecManager *manager = ViCodecManager::instance();
	QList<ViCodec> result;
	for(int i = 0; i < manager->mCodecs.size(); ++i)
	{
		if(manager->mCodecs[i].compression() == compression)
		{
			result.append(manager->mCodecs[i]);
		}
	}
	return result;
}

ViCodec ViCodecManager::selected(QString abbreviation)
{
	ViCodecManager *manager = ViCodecManager::instance();
	for(int i = 0; i < manager->mCodecs.size(); ++i)
	{
		if(manager->mCodecs[i].abbreviation() == abbreviation)
		{
			return manager->mCodecs[i];
		}
	}
	return ViCodec();
}

QList<QAudioFormat::Endian> ViCodecManager::byteOrders()
{
	ViCodecManager *manager = ViCodecManager::instance();
	return manager->mByteOrders;
}

QList<QAudioFormat::SampleType> ViCodecManager::sampleTypes()
{
	ViCodecManager *manager = ViCodecManager::instance();
	return manager->mSampleTypes;
}

QList<qint32> ViCodecManager::sampleRates()
{
	ViCodecManager *manager = ViCodecManager::instance();
	return manager->mSampleRates;
}

QList<qint8> ViCodecManager::sampleSizes()
{
	ViCodecManager *manager = ViCodecManager::instance();
	return manager->mSampleSizes;
}

QList<qint8> ViCodecManager::channels()
{
	ViCodecManager *manager = ViCodecManager::instance();
	return manager->mChannels;
}

