#include "vicodecmanager.h"

QSharedPointer<ViCodecManager> ViCodecManager::mInstance;

ViCodecManager::ViCodecManager()
{
	populate();
}

ViCodecManager* ViCodecManager::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViCodecManager>(new ViCodecManager());
	}
	return mInstance.data();
}

void ViCodecManager::populate()
{
	QList<QString> mp3Extensions;
	mp3Extensions << "mp3" << "bit";
	mCodecs.append(ViCodec(ViCodec::InputType, ViCodec::LossyCompression, "MP3", "Moving Picture Experts Group (MPEG) Audio Layer 3", mp3Extensions));
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
