#ifndef VIACTIVATIONFUNCTIONMANAGER_H
#define VIACTIVATIONFUNCTIONMANAGER_H

#include <viactivationfunction.h>
#include <vilibrarymanager.h>

class ViActivationFunctionManager : public ViLibraryManager<ViActivationFunctionManager, ViActivationFunction>
{

	public:

		QString libraryPath();
		QString defaultLibrary();

};

#endif
