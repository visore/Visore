#include <vitargetprovidermanager.h>

QString ViTargetProviderManager::libraryPath()
{
	return TARGET_PROVIDER_PATH;
}

QString ViTargetProviderManager::defaultLibrary()
{
	return "InterpolationTargetProvider";
}
