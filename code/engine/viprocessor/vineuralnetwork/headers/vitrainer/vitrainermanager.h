#ifndef VITRAINERMANAGER_H
#define VITRAINERMANAGER_H

#include <QList>
#include <vitrainer.h>
#include <vilibrarymanager.h>

class ViTrainerManager : public ViLibraryManager<ViTrainerManager, ViTrainer>
{

	public:

		QString libraryPath();
		QString defaultLibrary();

};

#endif
