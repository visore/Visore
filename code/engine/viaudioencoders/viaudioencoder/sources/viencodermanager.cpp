#include "viencodermanager.h"

QSharedPointer<ViEncoderManager> ViEncoderManager::mInstance;

ViEncoderManager::ViEncoderManager()
{
	mManager.searchPath(VIAUDIOENCODERSLOCATION);
}

ViEncoderManager* ViEncoderManager::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViEncoderManager>(new ViEncoderManager());
	}
	return mInstance.data();
}

QList<ViAudioEncoder*> ViEncoderManager::all()
{
	ViEncoderManager *manager = ViEncoderManager::instance();
	return manager->mManager.all();
}

QList<ViAudioEncoder*> ViEncoderManager::selected(QList<QString> names)
{
	ViEncoderManager *manager = ViEncoderManager::instance();
	return manager->mManager.selected(names);
}

ViAudioEncoder* ViEncoderManager::selected(QString name)
{
	ViEncoderManager *manager = ViEncoderManager::instance();
	return manager->mManager.selected(name);
}

QList<ViAudioEncoder*> ViEncoderManager::selected(ViAudioFormat *format)
{
	ViEncoderManager *manager = ViEncoderManager::instance();
	QList<ViAudioEncoder*> result;
	QList<ViAudioEncoder*> *all = manager->mManager.singletons();
	for(int i = 0; i < all->size(); ++i)
	{
		if(format == (*all)[i]->format())
		{
			result.append((*all)[i]);
		}
	}
	return result;
}
