#include <vitrainermanager.h>
#include <vilogger.h>

ViTrainerManager::ViTrainerManager()
{
	typedef ViTrainer* (*FunctionPointer)();
	mManager.load(libraryPath());
	QList<QFunctionPointer> pointers = mManager.resolve("create");
	for(int i = 0; i < pointers.size(); ++i)
	{
		addTrainer(((FunctionPointer) pointers[i])());
	}
}

ViTrainerManager::~ViTrainerManager()
{
	viDeleteAll(mTrainers);
}

void ViTrainerManager::addTrainer(ViTrainer* trainer)
{
	mTrainers.append(trainer);
}

ViTrainer* ViTrainerManager::create(QString name)
{
	QString newName = name.toLower();
	ViTrainer *trainer = NULL;
	trainer = createTrainer(newName);
	if(trainer == NULL && !newName.endsWith("trainer"))
	{
		newName.append("trainer");
		trainer = createTrainer(newName);
	}
	if(trainer == NULL && newName.startsWith("vi"))
	{
		newName.remove(0, 2);
		trainer = createTrainer(newName);
	}
	if(trainer == NULL)
	{
		STATICLOG("The specified trainer (" + name + ") could not be found.", QtCriticalMsg);
	}
	
	return trainer;
}

ViTrainer* ViTrainerManager::createDefault()
{
	return create("BackpropagationTrainer");
}

ViTrainer* ViTrainerManager::createTrainer(QString name)
{
	QString newName = name.toLower();
	ViPointer<ViTrainerManager> manager = instance();
	for(int i = 0; i < manager->mTrainers.size(); ++i)
	{
		if(manager->mTrainers[i]->name().toLower() == newName)
		{
			return manager->mTrainers[i]->clone();
		}
	}
	return NULL;
}

QString ViTrainerManager::libraryPath()
{
	return TRAINER_PATH;
}
