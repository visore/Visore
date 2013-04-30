#ifndef VIWEIGHTINITIALIZERMANAGER_H
#define VIWEIGHTINITIALIZERMANAGER_H

#include <viweightinitializer.h>
#include <vilibrarymanager.h>

class ViWeightInitializerManager : public ViLibraryManager<ViWeightInitializerManager, ViWeightInitializer>
{

	public:

		QString libraryPath();
		QString defaultLibrary();

};

#endif
