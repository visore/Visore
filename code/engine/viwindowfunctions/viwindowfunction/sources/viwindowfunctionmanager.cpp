#include "viwindowfunctionmanager.h"

QSharedPointer<ViWindowFunctionManager> ViWindowFunctionManager::mInstance;

ViWindowFunctionManager::ViWindowFunctionManager()
{
	mManager.searchPath(VIWINDOWFUNCTIONSLOCATION);
}

ViWindowFunctionManager* ViWindowFunctionManager::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViWindowFunctionManager>(new ViWindowFunctionManager());
	}
	return mInstance.data();
}

QList<ViWindowFunction*> ViWindowFunctionManager::all()
{
	ViWindowFunctionManager *manager = ViWindowFunctionManager::instance();
	return manager->mManager.all();
}

QList<ViWindowFunction*> ViWindowFunctionManager::selected(QList<QString> names)
{
	ViWindowFunctionManager *manager = ViWindowFunctionManager::instance();
	return manager->mManager.selected(names);
}

ViWindowFunction* ViWindowFunctionManager::selected(QString name)
{
	ViWindowFunctionManager *manager = ViWindowFunctionManager::instance();
	return manager->mManager.selected(name);
}
