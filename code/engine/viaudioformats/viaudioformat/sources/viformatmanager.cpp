#include "viformatmanager.h"

QSharedPointer<ViFormatManager> ViFormatManager::mInstance;

ViFormatManager::ViFormatManager()
{
	mManager.searchPath(VIAUDIOFORMATSLOCATION);
}

ViFormatManager* ViFormatManager::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViFormatManager>(new ViFormatManager());
	}
	return mInstance.data();
}

QList<ViAudioFormat*> ViFormatManager::all()
{
	ViFormatManager *manager = ViFormatManager::instance();
	return manager->mManager.all();
}

QList<ViAudioFormat*> ViFormatManager::selected(QList<QString> names)
{
	ViFormatManager *manager = ViFormatManager::instance();
	return manager->mManager.selected(names);
}

ViAudioFormat* ViFormatManager::selected(QString name)
{
	ViFormatManager *manager = ViFormatManager::instance();
	return manager->mManager.selected(name);
}
