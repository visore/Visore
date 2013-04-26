#include <vierrorfunctionmanager.h>
#include <vilogger.h>

ViErrorFunctionManager::ViErrorFunctionManager()
{
	typedef ViErrorFunction* (*FunctionPointer)();
	mManager.load(libraryPath());
	QList<QFunctionPointer> pointers = mManager.resolve("create");
	for(int i = 0; i < pointers.size(); ++i)
	{
		addFunction(((FunctionPointer) pointers[i])());
	}
}

ViErrorFunctionManager::~ViErrorFunctionManager()
{
	viDeleteAll(mFunctions);
}

void ViErrorFunctionManager::addFunction(ViErrorFunction* initializer)
{
	mFunctions.append(initializer);
}

ViErrorFunction* ViErrorFunctionManager::create(QString name)
{
	QString newName = name.toLower();
	ViErrorFunction *initializer = NULL;

	initializer = createFunction(newName);
	if(initializer == NULL && newName.endsWith("error"))
	{
		newName.append("function");
		initializer = createFunction(newName);
	}
	if(initializer == NULL && !newName.endsWith("errorfunction"))
	{
		newName.append("errorfunction");
		initializer = createFunction(newName);
	}
	if(initializer == NULL && newName.startsWith("vi"))
	{
		newName.remove(0, 2);
		initializer = createFunction(newName);
	}
	if(initializer == NULL)
	{
		STATICLOG("The specified error function (" + newName + ") could not be found.", QtCriticalMsg);
	}
	
	return initializer;
}

ViErrorFunction* ViErrorFunctionManager::create(ViElement element)
{
	if(element.name() != "Name")
	{
		if(element.child("Name").isNull())
		{
			if(element.child("ErrorFunction").child("Name").isNull())
			{
				return NULL;
			}
			return create(element.child("ErrorFunction").child("Name").toString()); 
		}
		return create(element.child("Name").toString()); 
	}
	return create(element.toString());
}

ViErrorFunction* ViErrorFunctionManager::createDefault()
{
	return create("RootMeanSquaredErrorFunction");
}

ViErrorFunction* ViErrorFunctionManager::createFunction(QString name)
{
	QString newName = name.toLower();
	ViPointer<ViErrorFunctionManager> manager = instance();
	for(int i = 0; i < manager->mFunctions.size(); ++i)
	{
		if(manager->mFunctions[i]->name().toLower() == newName)
		{
			return manager->mFunctions[i]->clone();
		}
	}
	return NULL;
}

QString ViErrorFunctionManager::libraryPath()
{
	return ERROR_FUNCTION_PATH;
}
