#ifdef VILIBRARY_H

template <class T, class P1>
ViLibrary<T, P1>::ViLibrary(QString path)
{
	mPath = path;
	mHandle = NULL;
}

template <class T, class P1>
ViLibrary<T, P1>::~ViLibrary()
{
	if(mHandle != NULL)
	{
		dlclose(mHandle);
		mHandle = NULL;
	}
}

template <class T, class P1>
bool ViLibrary<T, P1>::open()
{
	mHandle = dlopen(mPath.toUtf8().data(), RTLD_NOW);
	if(mHandle == NULL)
	{
		setErrorParameters("ViLibrary - Library Error", "Can't open the specified library(" + mPath + ")", ViErrorInfo::Fatal);
		return false;
	}
	return true;
}

template <class T, class P1>
T* ViLibrary<T, P1>::createObject(QString functionName, P1 *object)
{
	if(mHandle == NULL)
	{
		setErrorParameters("ViLibrary - Library Error", "Please open the library first", ViErrorInfo::Fatal);
		return NULL;
	}
	if(object != NULL)
	{
		typedef T* (*createObject)(P1*);
		createObject creator = (createObject) dlsym(mHandle, functionName.toUtf8().data());
		return creator(object);
	}
	typedef T* (*createObject)();
	createObject creator = (createObject) dlsym(mHandle, functionName.toUtf8().data());
	return creator();
}

template <class T, class P1>
void ViLibrary<T, P1>::deleteObject(QString functionName, P1 *object)
{
	if(mHandle == NULL)
	{
		setErrorParameters("ViLibrary - Library Error", "Please open the library first", ViErrorInfo::Fatal);
	}
	typedef void (*deleteObject)(void*);
	deleteObject deleter = (deleteObject) dlsym(mHandle, functionName.toUtf8().data());
	deleter(object);
}

#endif
