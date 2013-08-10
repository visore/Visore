#ifdef VICHUNK_H

#include <QtCore/qmath.h>
#include <string.h> //memcpy
#include <algorithm> //fill

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
			T *temp = new T[size];
			memcpy(temp, mData, sizeof(T) * qMin(size, mSize));
			delete [] mData;
			mData = temp;
		}
		else
		{
			mData = new T[size];
		}
		mSize = size;
	}
}

template<typename T>
void ViChunk<T>::insert(T value, int repeatValue)
{
	if(mData == 0)
	{
		resize(repeatValue);
		std::fill(mData, mData + repeatValue, value);
	}
	else
	{
		T *temp = new T[mSize];
		memcpy(temp, mData, sizeof(T) * mSize);
		resize(mSize + repeatValue);
		std::fill(mData, mData + repeatValue, value);
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
		std::fill(mData, mData + repeatValue, value);
	}
	else
	{
		int oldSize = mSize;
		T *temp = new T[oldSize];
		memcpy(temp, mData, sizeof(T) * oldSize);
		resize(mSize + repeatValue);
		memcpy(mData, temp, sizeof(T) * oldSize);
		std::fill(mData + oldSize, mData + oldSize + repeatValue, value);
		delete [] temp;
	}
}

template<typename T>
void ViChunk<T>::append(const ViChunk<T> &other)
{
	int oldSize = mSize;
	resize(oldSize + other.size());
	memcpy(mData + oldSize, other.data(), sizeof(T) * other.size());
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
ViChunk<T> ViChunk<T>::subset(const int &startIndex, const int &size) const
{
	ViChunk<T> subsetData;
	subset(subsetData, startIndex, size);
	return subsetData;
}

template<typename T>
bool ViChunk<T>::subset(ViChunk<T> &subsetData, const int &startIndex, const int &size) const
{
	if(startIndex >= mSize)
	{
		subsetData.clear();
		return false;
	}

	int newSize = size;
	bool success = true;
	if(startIndex + size > mSize)
	{
		newSize = mSize - startIndex;
		success = false;
	}

	subsetData.resize(newSize);
	memcpy(subsetData.data(), mData + startIndex, sizeof(T) * newSize);
	return success;
}

template<typename T>
QList<ViChunk<T>> ViChunk<T>::subsets(const int &size) const
{
	QList<ViChunk<T>> result;
	int sets = qCeil(mSize / float(size));
	for(int i = 0; i < sets; ++i)
	{
		result.append(subset(i * size, size));
	}
	return result;
}

template<typename T>
const T& ViChunk<T>::first() const
{
	return mData[0];
}

template<typename T>
const T& ViChunk<T>::first(int offset) const
{
	return mData[offset];
}

template<typename T>
T& ViChunk<T>::first()
{
	return mData[0];
}

template<typename T>
T& ViChunk<T>::first(int offset)
{
	return mData[offset];
}

template<typename T>
const T& ViChunk<T>::last() const
{
	return mData[mSize - 1];
}

template<typename T>
const T& ViChunk<T>::last(int offset) const
{
	return mData[mSize - 1 - offset];
}

template<typename T>
T& ViChunk<T>::last()
{
	return mData[mSize - 1];
}

template<typename T>
T& ViChunk<T>::last(int offset)
{
	return mData[mSize - 1 - offset];
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
    copyData(source, destination);
    destination.mSize = qMin(source.mSize, destination.mSize);
}

template<typename T>
void ViChunk<T>::copyData(const ViChunk<T> &source, ViChunk<T> &destination)
{
    memcpy(destination.mData, source.mData, sizeof(T) * qMin(source.mSize, destination.mSize));
}

#endif
