#ifdef VIPOINTER_H

template<class T>
ViPointerData<T>::ViPointerData(T *data, ViFunctor *deleter, int counter, int limiter)
{
    mData = data;
    mDeleter = deleter;
    mCounter.setValue(counter);
    mLimiter.setValue(limiter);
}

template<class T>	
void ViPointerData<T>::setData(T *data)
{
	mData = data;
}

template<class T>
void ViPointerData<T>::setDeleter(ViFunctor *deleter)
{
	mDeleter = deleter;
}

template<class T>
void ViPointerData<T>::setCounter(int counter)
{
	mCounter.setValue(counter);
}

template<class T>
void ViPointerData<T>::setLimiter(int limiter)
{
	mLimiter.setValue(limiter);
}

template<class T>
T* ViPointerData<T>::data()
{
	return mData;
}

template<class T>
ViFunctor* ViPointerData<T>::deleter()
{
	return mDeleter;
}

template<class T>
ViAtomicInt& ViPointerData<T>::counter()
{
	return mCounter;
}

template<class T>
ViAtomicInt& ViPointerData<T>::limiter()
{
	return mLimiter;
}


template<class T>
ViPointer<T>::ViPointer()
{
    mData = new ViPointerData<T>();
    mDestructMutex = new QMutex();
}

template<class T>
ViPointer<T>::ViPointer(T *pointer)
{
	mData = new ViPointerData<T>();
    mData->setData(pointer);
    mDestructMutex = new QMutex();
}

template<class T>
ViPointer<T>::ViPointer(const ViPointer<T> &other)
{
	copy(other);
}

template<class T>
ViPointer<T>::~ViPointer()
{
    destruct();
}

template<class T>
void ViPointer<T>::copy(const ViPointer<T> &other)
{
    mDestructMutex = other.mDestructMutex;
    mData = other.mData;
    mData->counter().increase();
}

template<class T>
void ViPointer<T>::destruct()
{
    if(mDestructMutex != NULL)
    {
        if(!mDestructMutex->tryLock())
        {
            return;
        }

        if(mData != NULL)
        {
            mData->counter().decrease();
            if(!isUsed())
            {
                if(mData->deleter() != NULL)
                {
                    mData->deleter()->execute(mData->data());
                }
                else
                {
                    delete mData->data();
                    mData->setData(NULL);
                }
                delete mData;
                mData = NULL;
                mDestructMutex->unlock();
                delete mDestructMutex;
                mDestructMutex = NULL;
            }
            else
            {
                mDestructMutex->unlock();
            }
        }
        else
        {
            mDestructMutex->unlock();
        }
    }
}

template<class T>
void ViPointer<T>::setDeleter(ViFunctor *deleter)
{
	mData->setDeleter(deleter);
}

template<class T>
void ViPointer<T>::setUnusedLimit(int limit)
{
	mData->limiter().setValue(limit);
}

template<class T>
int ViPointer<T>::referenceCount()
{
	return mData->counter().value();
}

template<class T>
bool ViPointer<T>::isUsed()
{
    return referenceCount() > mData->limiter().value();
}

template<class T>
bool ViPointer<T>::isNull()
{
	return (mData->data() == NULL);
}

template<class T>
void ViPointer<T>::setNull()
{
	destruct();
	mData = new ViPointerData<T>();
    mDestructMutex = new QMutex();
}

template<class T>
T* ViPointer<T>::data()
{
	return mData->data();
}

template<class T>
T& ViPointer<T>::operator * ()
{
	return *mData->data();
}

template<class T>
T* ViPointer<T>::operator -> ()
{
	return mData->data();
}

template<class T>
ViPointer<T>& ViPointer<T>::operator = (const ViPointer<T> &other)
{
    destruct();
    copy(other);
	return *this;
}

template<class T>
bool ViPointer<T>::operator == (const ViPointer<T> &other)
{
	return mData->data() == other.mData->data();
}

template<class T>
bool ViPointer<T>::operator == (const T &other)
{
	return mData->data() == &other;
}

template<class T>
bool ViPointer<T>::operator == (const T *other)
{
	return mData->data() == other;
}

template<class T>
bool ViPointer<T>::operator != (const ViPointer<T> &other)
{
	return mData->data() != other.mData->data();
}

template<class T>
bool ViPointer<T>::operator != (const T &other)
{
	return mData->data() != &other;
}

template<class T>
bool ViPointer<T>::operator != (const T *other)
{
	return mData->data() != other;
}

template<class T, class F>
ViPointer<T> viPointerCast(ViPointer<F> pointer)
{
	return ViPointer<T>(static_cast<T*>(pointer.data()));
}

#endif
