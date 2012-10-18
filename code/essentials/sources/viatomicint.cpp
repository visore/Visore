#include "viatomicint.h"

ViAtomicInt::ViAtomicInt(int value)
{
	mValue = value;
}

void ViAtomicInt::increase()
{
	QMutexLocker locker(&mMutex);
	++mValue;
}

void ViAtomicInt::decrease()
{
	QMutexLocker locker(&mMutex);
	--mValue;
}

int ViAtomicInt::value()
{
	QMutexLocker locker(&mMutex);
	return mValue;
}

void ViAtomicInt::setValue(int value)
{
	QMutexLocker locker(&mMutex);
	mValue = value;
}
