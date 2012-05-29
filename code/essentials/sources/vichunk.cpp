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
T* ViChunk<T>::data() const
{
	return mData;
}

template<typename T>
int ViChunk<T>::size() const
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
void ViChunk<T>::setSize(int size)
{
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

template<typename T>
const T& ViChunk<T>::at(int index) const
{
	return mData[index];
}

template<typename T>
T& ViChunk<T>::at(int index)
{
	return mData[index];
}

template<typename T>
const T& ViChunk<T>::operator[](const int index) const
{
	return mData[index];
}

template<typename T>
T& ViChunk<T>::operator[] (const int index)
{
	return mData[index];
}

template<typename T>
void ViChunk<T>::copy(const ViChunk<T> *source, ViChunk<T> *destination)
{
	memcpy(destination->mData, source->mData, sizeof(T) * source->mSize);
	destination->mSize = source->mSize;
}

#endif
