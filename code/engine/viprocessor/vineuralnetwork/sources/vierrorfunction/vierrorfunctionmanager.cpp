#include <vierrorfunctionmanager.h>

QString ViErrorFunctionManager::libraryPath()
{
	return ERROR_FUNCTION_PATH;
}

QString ViErrorFunctionManager::defaultLibrary()
{
	return "RootMeanSquaredErrorFunction";
}
