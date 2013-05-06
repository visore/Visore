#ifndef ViInterpolatorMANAGER_H
#define ViInterpolatorMANAGER_H

#include <vilibrarymanager.h>
#include <viinterpolator.h>

class ViInterpolatorManager : public ViLibraryManager<ViInterpolatorManager, ViInterpolator>
{

	public:

		QString libraryPath();
		QString defaultLibrary();

};

#endif
