#ifdef VIPOINTER_H
#include "vilogger.h"
template<class T>
ViPointer<T>::ViPointer()
{
	mData = new ViPointerData<T>();
	mData->mCounter = new ViAtomicInt(1);
	mData->mLimiter = new ViAtomicInt(0);
	/*mData = NULL;
	mDeleter = NULL;
	mCounter = new ViAtomicInt(1);
	mLimiter = new ViAtomicInt(0);*/
}

template<class T>
ViPointer<T>::ViPointer(T *pointer)
{
	mData = new ViPointerData<T>();
	mData->mData = pointer;
	mData->mCounter = new ViAtomicInt(1);
	mData->mLimiter = new ViAtomicInt(0);
	/*mData = pointer;
	mDeleter = NULL;
	mCounter = new ViAtomicInt(1);
	mLimiter = new ViAtomicInt(0);*/
}

template<class T>
ViPointer<T>::ViPointer(const ViPointer<T> &other)
{
	/*mData = other.mData;
	mDeleter = other.mDeleter;
	mCounter = other.mCounter;
	mLimiter = other.mLimiter;
	mCounter->increase();*/
	//mData = other.mData;
	mData = other.mData;
	mData->mCounter->increase();
}

template<class T>
ViPointer<T>::~ViPointer()
{
	destruct();
}

template<class T>
void ViPointer<T>::destruct()
{
	mData->mCounter->decrease();
	if(referenceCount() == mData->mLimiter->value() )
	{
		if(mData->mDeleter != NULL) mData->mDeleter->execute(mData->mData);
	}
	else if(!isUsed())
	{
		delete mData->mCounter;
		if(mData->mData != NULL)
		{
			delete mData->mData;
			mData->mData = NULL;
		}
	}
}

template<class T>
void ViPointer<T>::setDeleter(ViFunctor *deleter)
{
	mData->mDeleter = deleter;
}

template<class T>
void ViPointer<T>::setUnusedLimit(int limit)
{
	mData->mLimiter->setValue(limit);
}

template<class T>
int ViPointer<T>::referenceCount()
{
	return mData->mCounter->value();
}

template<class T>
bool ViPointer<T>::isUsed()
{
	return referenceCount() != 0;
}

template<class T>
bool ViPointer<T>::isNull()
{
	return (mData->mData == NULL);
}

template<class T>
T* ViPointer<T>::data()
{
	return mData->mData;
}

template<class T>
T& ViPointer<T>::operator * ()
{
	return *mData->mData;
}

template<class T>
T* ViPointer<T>::operator -> ()
{
	return mData->mData;
}

template<class T>
ViPointer<T>& ViPointer<T>::operator = (const ViPointer<T> &other)
{
	destruct();
	/*ViPointer temp(other);
	return temp; // Must be in 2 seperate lines*/
	mData = other.mData;
	mData->mCounter->increase();
	return *this;
}

template<class T>
bool ViPointer<T>::operator == (const ViPointer<T> &other)
{
	return mData->mData == other.mData->mData;
}

template<class T>
bool ViPointer<T>::operator == (const T &other)
{
	return mData->mData == &other;
}

template<class T>
bool ViPointer<T>::operator == (const T *other)
{
	return mData->mData == other;
}

template<class T>
bool ViPointer<T>::operator != (const ViPointer<T> &other)
{
	return mData->mData != other.mData->mData;
}

template<class T>
bool ViPointer<T>::operator != (const T &other)
{
	return mData->mData != &other;
}

template<class T>
bool ViPointer<T>::operator != (const T *other)
{
	return mData->mData != other;
}

#endif
