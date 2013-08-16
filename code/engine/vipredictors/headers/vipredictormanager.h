#ifndef VIPREDICTORMANAGER_H
#define VIPREDICTORMANAGER_H

#include <vipredictor.h>
#include <vilibrarymanager.h>

class ViPredictorManager : public ViLibraryManager<ViPredictorManager, ViPredictor>
{

    public:

        QString libraryPath();
        QString defaultLibrary();

};

#endif
