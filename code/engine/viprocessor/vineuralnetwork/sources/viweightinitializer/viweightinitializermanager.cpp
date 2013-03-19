#include <viweightinitializermanager.h>
#include <vilogger.h>

ViWeightInitializerManager::ViWeightInitializerManager()
{
	typedef ViWeightInitializer* (*InitializerPointer)();
	mManager.load(libraryPath());
	QList<QFunctionPointer> pointers = mManager.resolve("create");
	for(int i = 0; i < pointers.size(); ++i)
	{
		addInitializer(((InitializerPointer) pointers[i])());
	}
}

ViWeightInitializerManager::~ViWeightInitializerManager()
{
	viDeleteAll(mInitializers);
}

void ViWeightInitializerManager::addInitializer(ViWeightInitializer* initializer)
{
	mInitializers.append(initializer);
}

ViWeightInitializer* ViWeightInitializerManager::create(QString name)
{
	QString newName = name.toLower();
	ViWeightInitializer *initializer = NULL;

	initializer = createInitializer(newName);
	if(initializer == NULL && !newName.endsWith("weightinitializer"))
	{
		newName.append("weightinitializer");
		initializer = createInitializer(newName);
	}
	if(initializer == NULL && newName.startsWith("vi"))
	{
		newName.remove(0, 2);
		initializer = createInitializer(newName);
	}
	if(initializer == NULL)
	{
		STATICLOG("The specified weight initializer (" + name + ") could not be found.", QtCriticalMsg);
	}
	
	return initializer;
}

ViWeightInitializer* ViWeightInitializerManager::create(ViElement element)
{
	if(element.name() != "Name")
	{
		if(element.child("Name").isNull())
		{
			if(element.child("WeightInitializer").child("Name").isNull())
			{
				return NULL;
			}
			return create(element.child("WeightInitializer").child("Name").toString()); 
		}
		return create(element.child("Name").toString()); 
	}
	return create(element.toString());
}

ViWeightInitializer* ViWeightInitializerManager::createDefault()
{
	return create("RandomWeightInitializer");
}

ViWeightInitializer* ViWeightInitializerManager::createInitializer(QString name)
{
	QString newName = name.toLower();
	ViPointer<ViWeightInitializerManager> manager = instance();
	for(int i = 0; i < manager->mInitializers.size(); ++i)
	{
		if(manager->mInitializers[i]->name().toLower() == newName)
		{
			return manager->mInitializers[i]->clone();
		}
	}
	return NULL;
}

QString ViWeightInitializerManager::libraryPath()
{
	return WEIGHT_INITIALIZER_PATH;
}
