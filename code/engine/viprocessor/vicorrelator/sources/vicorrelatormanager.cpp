#include <vicorrelatormanager.h>

QString ViCorrelatorManager::libraryPath()
{
    return CORRELATOR_PATH;
}

QString ViCorrelatorManager::defaultLibrary()
{
    return "SampleCorrelator";
}
