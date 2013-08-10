#include <vinoisedetectormanager.h>

QString ViNoiseDetectorManager::libraryPath()
{
    return NOISE_DETECTOR_PATH;
}

QString ViNoiseDetectorManager::defaultLibrary()
{
	return "MahalanobisNoiseDetector";
	//return "ZscoreNoiseDetector";
}
