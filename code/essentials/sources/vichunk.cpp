#ifdef VICHUNK_H
#include <vilogger.h>
template<typename T>
ViChunk<T>::ViChunk()
{
	mData = 0;
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
	mData = 0;
	mSize = 0;
	resize(size);
}

template<typename T>
ViChunk<T>::ViChunk(const ViChunk &other)
{
	mData = 0;
	mSize = 0;
	resize(other.mSize);
	copy(other, *this);
}

template<typename T>
ViChunk<T>::~ViChunk()
{
	if(mData != 0)
	{
		delete [] mData;
		mData = 0;
	}
}

template<typename T>
T* ViChunk<T>::data() const
{
	return mData;
}

template<typename T>
const T* ViChunk<T>::constData() const
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
	if(mData != 0)
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
		if(mData != 0)
		{
			delete [] mData;
		}
		mData = new T[size];
		mSize = size;
	}
}

template<typename T>
void ViChunk<T>::insert(T value, int repeatValue)
{
	if(mData == 0)
	{
		resize(repeatValue);
		fill(mData, mData + repeatValue, value);
	}
	else
	{
		T *temp = new T[mSize];
		memcpy(temp, mData, sizeof(T) * mSize);
		resize(mSize + repeatValue);
		fill(mData, mData + repeatValue, value);
		memcpy(mData + repeatValue, temp, sizeof(T) * repeatValue);
		delete [] temp;
	}
}

template<typename T>
void ViChunk<T>::append(T value, int repeatValue)
{
	if(mData == 0)
	{
		resize(repeatValue);
		fill(mData, mData + repeatValue, value);
	}
	else
	{
		int oldSize = mSize;
		T *temp = new T[oldSize];
		memcpy(temp, mData, sizeof(T) * oldSize);
		resize(mSize + repeatValue);
		memcpy(mData, temp, sizeof(T) * oldSize);
		fill(mData + oldSize, mData + oldSize + repeatValue, value);
		delete [] temp;
	}
}

template<typename T>
void ViChunk<T>::clear()
{
	mSize = 0;
	if(mData != 0)
	{
		delete [] mData;
		mData = 0;
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
void ViChunk<T>::operator= (const ViChunk &other)
{
	resize(other.mSize);
	memcpy(mData, other.mData, sizeof(T) * mSize);
}

template<typename T>
void ViChunk<T>::copy(const ViChunk<T> &source, ViChunk<T> &destination)
{
	memcpy(destination.mData, source.mData, sizeof(T) * source.mSize);
	destination.mSize = source.mSize;
}

#endif
