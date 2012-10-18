#include "viaudioobjectpointer.h"
#include "viaudioobject.h"
/*
ViAudioObjectPointer::ViAudioObjectPointer()
	: QObject(), ViPointer<ViAudioObject>()
{
	mLimit = new ViAtomicInt();
}

ViAudioObjectPointer::ViAudioObjectPointer(ViAudioObject *pointer)
	: QObject(), ViPointer<ViAudioObject>(pointer)
{
	mLimit = new ViAtomicInt();
}

ViAudioObjectPointer::ViAudioObjectPointer(const ViAudioObjectPointer &other)
	: QObject(), ViPointer<ViAudioObject>(other)
{
	mLimit = other.mLimit;
	checkUnused();
}

ViAudioObjectPointer::~ViAudioObjectPointer()
{
	if(referenceCount() == 1)
	{
		delete mLimit;
	}
}

void ViAudioObjectPointer::setUnusedLimit(int unusedLimit)
{
	mLimit->setValue(unusedLimit);
}

int ViAudioObjectPointer::unusedLimit()
{
	return mLimit->value();
}

void ViAudioObjectPointer::checkUnused()
{
	if(referenceCount() <= unusedLimit())
	{
		emit unused();
	}
}

ViAudioObjectPointer& ViAudioObjectPointer::operator = (const ViAudioObjectPointer &other)
{
	ViAudioObjectPointer temp(other);
	return *this;
}*/
