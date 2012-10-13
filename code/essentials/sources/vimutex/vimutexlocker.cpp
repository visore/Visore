#include "vimutexlocker.h"

ViMutexLocker::ViMutexLocker(ViMutex *mutex)
{
	mMutex = mutex;
	mMutex->lock();
}

ViMutexLocker::ViMutexLocker(ViMutex &mutex)
{
	mMutex = &mutex;
	mMutex->lock();
}

ViMutexLocker::~ViMutexLocker()
{
	mMutex->unlock();
}

void ViMutexLocker::lock()
{
	mMutex->lock();
}

void ViMutexLocker::unlock()
{
	mMutex->unlock();
}
