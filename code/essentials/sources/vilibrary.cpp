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
T* ViLibrary<T>::create(QString functionName)
{
	if(mHandle == NULL)
	{
		return NULL;
	}
	typedef T* (*createObject)();
	createObject creator = (createObject) dlsym(mHandle, functionName.toUtf8().data());
	return creator();
}

#endif
