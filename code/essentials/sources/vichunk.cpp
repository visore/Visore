#ifdef VICHUNK_H

template<typename T>
ViChunk<T>::ViChunk()
{
	mData = NULL;
	mSize = 0;
}

template<typename T>
ViChunk<T>::ViChunk(T *data, int size)
{
	mData = data;
	mSize = size;
}

template<typename T>
ViChunk<T>::ViChunk(int size)
{
	resize(size);
}

template<typename T>
ViChunk<T>::~ViChunk()
{
	if(mData != NULL)
	{
		delete [] mData;
		mData = NULL;
	}
}

template<typename T>
T* ViChunk<T>::data()
{
	return mData;
}

template<typename T>
int ViChunk<T>::size()
{
	return mSize;
}

template<typename T>
void ViChunk<T>::setData(T *data, int size)
{
	if(mData != NULL)
	{
		delete [] mData;
	}
	mData = data;
	mSize = size;
}

template<typename T>
void ViChunk<T>::resize(int size)
{
	if(size != mSize)
	{
		if(mData != NULL)
		{
			delete [] mData;
		}
		mData = new T[size];
		mSize = size;
	}
}

#endif
