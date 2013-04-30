#ifndef VIERRORFUNCTIONMANAGER_H
#define VIERRORFUNCTIONMANAGER_H

#include <vierrorfunction.h>
#include <vilibrarymanager.h>

class ViErrorFunctionManager : public ViLibraryManager<ViErrorFunctionManager, ViErrorFunction>
{

	public:

		QString libraryPath();
		QString defaultLibrary();

};

#endif
