#ifndef VITARGETPROVIDERMANAGER_H
#define VITARGETPROVIDERMANAGER_H

#include <vitargetprovider.h>
#include <vilibrarymanager.h>

class ViTargetProviderManager : public ViLibraryManager<ViTargetProviderManager, ViTargetProvider>
{

	public:

		QString libraryPath();
		QString defaultLibrary();

};

#endif
