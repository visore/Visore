#ifndef VICORRELATORMANAGER_H
#define VICORRELATORMANAGER_H

#include <vicorrelator.h>
#include <vilibrarymanager.h>

class ViCorrelatorManager : public ViLibraryManager<ViCorrelatorManager, ViCorrelator>
{

    public:

        QString libraryPath();
        QString defaultLibrary();

};

#endif
