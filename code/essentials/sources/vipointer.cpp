#ifdef VIPOINTER_H
/*
template<class T>
ViPointer<T>::ViPointer()
	: QSharedPointer<T>()
{
	mCounter = new ViAtomicInt(1);
}

template<class T>
ViPointer<T>::ViPointer(T *pointer)
	: QSharedPointer<T>(pointer)
{
	mCounter = new ViAtomicInt(1);
}

template<class T>
ViPointer<T>::ViPointer(T *pointer, Deleter deleter)
	: QSharedPointer<T>(pointer, deleter)
{
	mCounter = new ViAtomicInt(1);
}

template<class T>
ViPointer<T>::ViPointer(const ViPointer<T> &other)
	: QSharedPointer<T>(other)
{
	mCounter = other.mCounter;
	mCounter->increase();
}

template<class T>
ViPointer<T>::ViPointer(const QSharedPointer<T> &other)
	: QSharedPointer<T>(other)
{
	mCounter = other.mCounter;
	mCounter->increase();
}

template<class T>
ViPointer<T>::ViPointer(const QWeakPointer<T> &other)
	: QSharedPointer<T>(other)
{
	mCounter = other.mCounter;
	mCounter->increase();
}

template<class T>
ViPointer<T>::~ViPointer()
{
	mCounter->decrease();
	if(!isUsed())
	{
		delete mCounter;
	}
}

template<class T>
int ViPointer<T>::referenceCount()
{
	return mCounter->value();
}

template<class T>
bool ViPointer<T>::isUsed()
{
	return referenceCount() != 0;
}

template<class T>
ViPointer<T>& ViPointer<T>::operator = (const ViPointer<T> &other)
{
	ViPointer<T> temp(other);
	return *this;
}*/
#include"vilogger.h"
template<class T>
ViPointer<T>::ViPointer()
{
	mData = NULL;
	mCounter = new ViAtomicInt(1);
	mLimiter = new ViAtomicInt(1);
}

template<class T>
ViPointer<T>::ViPointer(T *pointer)
{
	mData = pointer;
	mCounter = new ViAtomicInt(1);
	mLimiter = new ViAtomicInt(1);
}

template<class T>
ViPointer<T>::ViPointer(const ViPointer<T> &other)
{
	mData = other.mData;
	mDeleter = other.mDeleter;
	mCounter = other.mCounter;
	mLimiter = other.mLimiter;
	mCounter->increase();
}

template<class T>
ViPointer<T>::~ViPointer()
{LOG("dfgdfg");
	mCounter->decrease();
	if(referenceCount() == mLimiter->value())
	{LOG("dfgdfg222");
		mDeleter(mData);
	}
	else if(!isUsed())
	{
		delete mCounter;
		if(mData != NULL)
		{
			delete mData;
			mData = NULL;
		}
	}
}

template<class T>
int ViPointer<T>::referenceCount()
{
	return mCounter->value();
}

template<class T>
bool ViPointer<T>::isUsed()
{
	return referenceCount() != 0;
}

template<class T>
bool ViPointer<T>::isNull()
{
	return mData == NULL;
}

template<class T>
T* ViPointer<T>::data()
{
	return mData;
}

template<class T>
T& ViPointer<T>::operator * ()
{
	return *mData;
}

template<class T>
T* ViPointer<T>::operator -> ()
{
	return mData;
}

#endif
