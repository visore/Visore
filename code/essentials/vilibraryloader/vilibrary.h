#ifndef VILIBRARY_H
#define VILIBRARY_H

#include <dlfcn.h>
#include <stdlib.h>
#include <QString>

template <class T>
class ViLibrary
{
	public:
		ViLibrary(QString path);
		~ViLibrary();
		bool open();
		T* create();

	private:
		QString mPath;
		void *mHandle;
};

#endif

//Implementation needed in header due to templates

template <class T>
ViLibrary<T>::ViLibrary(QString path)
{
	mPath = path;
	mHandle = NULL;
}

template <class T>
ViLibrary<T>::~ViLibrary()
{
	if(mHandle != NULL)
	{
		dlclose(mHandle);
		mHandle = NULL;
	}
}

template <class T>
bool ViLibrary<T>::open()
{
	mHandle = dlopen(mPath.toUtf8().data(), RTLD_NOW);
	if(mHandle == NULL)
	{
		return false;
	}
	return true;
}

template <class T>
T* ViLibrary<T>::create()
{
	if(mHandle == NULL)
	{
		return NULL;
	}
	typedef T* (*create)();
	create creator = (create) dlsym(mHandle, "create");;
	return creator();
}
