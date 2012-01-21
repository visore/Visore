#ifndef VILIBRARY_H
#define VILIBRARY_H

#include "vierror.h"
#include <dlfcn.h>
#include <stdlib.h>

template <class T>
class ViLibrary : public ViError
{
	public:
		ViLibrary(QString path);
		~ViLibrary();
		bool open();
		T* createObject(QString functionName = "createObject");
		void deleteObject(QString functionName = "deleteObject", void *object = NULL);

	private:
		QString mPath;
		void *mHandle;
};

//Template decleration and implementation should actually be in the same file
#include "../sources/vilibrary.cpp"

#endif

