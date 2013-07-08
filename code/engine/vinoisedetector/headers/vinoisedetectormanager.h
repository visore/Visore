#ifndef VINOISEDETECTORMANAGER_H
#define VINOISEDETECTORMANAGER_H

#include <vinoisedetector.h>
#include <vilibrarymanager.h>

class ViNoiseDetectorManager : public ViLibraryManager<ViNoiseDetectorManager, ViNoiseDetector>
{

    public:

        QString libraryPath();
        QString defaultLibrary();

};

#endif
