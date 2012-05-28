#ifndef VILIBRARY_H
#define VILIBRARY_H

#include "viobject.h"
#include <dlfcn.h>
#include <stdlib.h>

template <class T, class P1 = ViObject>
class ViLibrary
{
	public:
		ViLibrary();
		~ViLibrary();
		bool open(QString path);
		void close();
		T* createObject(QString functionName = "createObject", P1 *object = NULL);
		void deleteObject(QString functionName = "deleteObject", T *object = NULL);

	private:
		QString mPath;
		void *mHandle;
};

//Template decleration and implementation should actually be in the same file
#include "../sources/vilibrary.cpp"

#endif

