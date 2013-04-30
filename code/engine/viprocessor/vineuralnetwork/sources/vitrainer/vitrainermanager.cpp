#include <vitrainermanager.h>
#include <vilogger.h>

QString ViTrainerManager::libraryPath()
{
	return TRAINER_PATH;
}

QString ViTrainerManager::defaultLibrary()
{
	return "BackpropagationTrainer";
}
