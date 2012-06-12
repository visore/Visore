#ifdef VISINGELTON_H

template<typename T>
QSharedPointer<T> ViSingleton<T>::mInstance;

template<typename T>
T* ViSingleton<T>::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<T>(new T());
	}
	return mInstance.data();
}

#endif
