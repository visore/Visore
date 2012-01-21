#ifdef VILIBRARY_H

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
		setErrorParameters("ViLibrary - Library Error", "Can't open the specified library(" + mPath + ")", ViErrorInfo::Fatal);
		return false;
	}
	return true;
}

template <class T>
T* ViLibrary<T>::createObject(QString functionName)
{
	if(mHandle == NULL)
	{
		setErrorParameters("ViLibrary - Library Error", "Please open the library first", ViErrorInfo::Fatal);
		return NULL;
	}
	typedef T* (*createObject)();
	createObject creator = (createObject) dlsym(mHandle, functionName.toUtf8().data());
	return creator();
}

template <class T>
void ViLibrary<T>::deleteObject(QString functionName, void *object)
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
