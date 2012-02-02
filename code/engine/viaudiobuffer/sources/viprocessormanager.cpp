#include "viprocessormanager.h"

QSharedPointer<ViProcessorManager> ViProcessorManager::mInstance;

ViProcessorManager::ViProcessorManager()
{
	mCurrentId = -1;
}

ViProcessorManager* ViProcessorManager::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViProcessorManager>(new ViProcessorManager());
	}
	return mInstance.data();
}

int ViProcessorManager::nextId()
{
	ViProcessorManager *manager = instance();
	manager->incrementId();
	return manager->id();
}

int ViProcessorManager::id()
{
	return mCurrentId;
}

void ViProcessorManager::incrementId()
{
	++mCurrentId;
}
