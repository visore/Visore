#include <viactivationfunctionmanager.h>
#include <vilogger.h>

ViActivationFunctionManager::ViActivationFunctionManager()
{
	typedef ViActivationFunction* (*FunctionPointer)();
	mManager.load(libraryPath());
	QList<QFunctionPointer> pointers = mManager.resolve("create");
	for(int i = 0; i < pointers.size(); ++i)
	{
		addFunction(((FunctionPointer) pointers[i])());
	}
}

ViActivationFunctionManager::~ViActivationFunctionManager()
{
	viDeleteAll(mFunctions);
}

void ViActivationFunctionManager::addFunction(ViActivationFunction* function)
{
	mFunctions.append(function);
}

ViActivationFunction* ViActivationFunctionManager::create(QString name)
{
	QString newName = name.toLower();
	ViActivationFunction *function = NULL;

	function = createFunction(newName);
	if(function == NULL && !newName.endsWith("activationfunction"))
	{
		newName.append("activationfunction");
		function = createFunction(newName);
	}
	if(function == NULL && newName.startsWith("vi"))
	{
		newName.remove(0, 2);
		function = createFunction(newName);
	}
	if(function == NULL)
	{
		STATICLOG("The specified activation function (" + name + ") could not be found.", QtCriticalMsg);
	}
	
	return function;
}

ViActivationFunction* ViActivationFunctionManager::create(ViElement element)
{
	if(element.name() != "Name")
	{
		if(element.child("Name").isNull())
		{
			if(element.child("activationFunction").child("Name").isNull())
			{
				return NULL;
			}
			return create(element.child("activationFunction").child("Name").toString()); 
		}
		return create(element.child("Name").toString()); 
	}
	return create(element.toString());
}

ViActivationFunction* ViActivationFunctionManager::createDefault()
{
	return create("SigmoidActivationFunction");
}

ViActivationFunction* ViActivationFunctionManager::createFunction(QString name)
{
	QString newName = name.toLower();
	ViPointer<ViActivationFunctionManager> manager = instance();
	for(int i = 0; i < manager->mFunctions.size(); ++i)
	{
		if(manager->mFunctions[i]->name().toLower() == newName)
		{
			return manager->mFunctions[i]->clone();
		}
	}
	return NULL;
}

QString ViActivationFunctionManager::libraryPath()
{
	return ACTIVATION_FUNCTION_PATH;
}
