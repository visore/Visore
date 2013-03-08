#ifdef VISINGLETON_H

template <class T>
ViPointer<T> ViSingleton<T>::mInstance;

template <class T>
ViPointer<T> ViSingleton<T>::instance()
{
	if(mInstance.isNull())
	{
		mInstance = ViPointer<T>(new T());
	}
	return mInstance;
}

#endif
