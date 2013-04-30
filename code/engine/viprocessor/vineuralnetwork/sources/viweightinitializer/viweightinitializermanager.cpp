#include <viweightinitializermanager.h>

QString ViWeightInitializerManager::libraryPath()
{
	return WEIGHT_INITIALIZER_PATH;
}

QString ViWeightInitializerManager::defaultLibrary()
{
	return "RandomWeightInitializer";
}
