#include <vitargetprovidermanager.h>
#include <vilogger.h>

ViTargetProviderManager::ViTargetProviderManager()
{
	typedef ViTargetProvider* (*InitializerPointer)();
	mManager.load(libraryPath());
	QList<QFunctionPointer> pointers = mManager.resolve("create");
	for(int i = 0; i < pointers.size(); ++i)
	{
		addProvider(((InitializerPointer) pointers[i])());
	}
}

ViTargetProviderManager::~ViTargetProviderManager()
{
	viDeleteAll(mProviders);
}

void ViTargetProviderManager::addProvider(ViTargetProvider* provider)
{
	mProviders.append(provider);
}

ViTargetProvider* ViTargetProviderManager::create(QString name)
{
	QString newName = name.toLower();
	ViTargetProvider *provider = NULL;

	provider = createProvider(newName);
	if(provider == NULL && !newName.endsWith("targetprovider"))
	{
		newName.append("targetprovider");
		provider = createProvider(newName);
	}
	if(provider == NULL && newName.startsWith("vi"))
	{
		newName.remove(0, 2);
		provider = createProvider(newName);
	}
	if(provider == NULL)
	{
		STATICLOG("The specified target provider (" + name + ") could not be found.", QtCriticalMsg);
	}
	
	return provider;
}

ViTargetProvider* ViTargetProviderManager::create(ViElement element)
{
	if(element.name() != "Name")
	{
		if(element.child("Name").isNull())
		{
			if(element.child("TargetProvider").child("Name").isNull())
			{
				return NULL;
			}
			return create(element.child("TargetProvider").child("Name").toString()); 
		}
		return create(element.child("Name").toString()); 
	}
	return create(element.toString());
}

ViTargetProvider* ViTargetProviderManager::createDefault()
{
	//return create("RandomWeightInitializer");
}

ViTargetProvider* ViTargetProviderManager::createProvider(QString name)
{
	QString newName = name.toLower();
	ViPointer<ViTargetProviderManager> manager = instance();
	for(int i = 0; i < manager->mProviders.size(); ++i)
	{
		if(manager->mProviders[i]->name().toLower() == newName)
		{
			return manager->mProviders[i]->clone();
		}
	}
	return NULL;
}

QString ViTargetProviderManager::libraryPath()
{
	return TARGET_PROVIDER_PATH;
}
