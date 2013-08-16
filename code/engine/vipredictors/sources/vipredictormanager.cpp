#include <vipredictormanager.h>

QString ViPredictorManager::libraryPath()
{
	return PREDICTOR_PATH;
}

QString ViPredictorManager::defaultLibrary()
{
	return "LinearPredictor";
}
