#include <viinterpolatormanager.h>

QString ViInterpolatorManager::libraryPath()
{
	return INTERPOLATORS_PATH;
}

QString ViInterpolatorManager::defaultLibrary()
{
	return "PolynomialInterpolator";
}
