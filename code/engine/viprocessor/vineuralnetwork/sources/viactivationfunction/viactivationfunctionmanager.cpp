#include <viactivationfunctionmanager.h>

QString ViActivationFunctionManager::libraryPath()
{
	return ACTIVATION_FUNCTION_PATH;
}

QString ViActivationFunctionManager::defaultLibrary()
{
	return "DirectActivationFunction";
}
