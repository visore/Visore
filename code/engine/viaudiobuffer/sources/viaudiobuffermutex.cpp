#include "viaudiobuffermutex.h"

QSharedPointer<ViAudioBufferMutex> ViAudioBufferMutex::mReadInstance;
QSharedPointer<ViAudioBufferMutex> ViAudioBufferMutex::mWriteInstance;

ViAudioBufferMutex::ViAudioBufferMutex()
	: QMutex()
{
}

ViAudioBufferMutex* ViAudioBufferMutex::readInstance()
{
	if(mReadInstance.isNull())
	{
		mReadInstance = QSharedPointer<ViAudioBufferMutex>(new ViAudioBufferMutex());
	}
	return mReadInstance.data();
}

ViAudioBufferMutex* ViAudioBufferMutex::writeInstance()
{
	if(mWriteInstance.isNull())
	{
		mWriteInstance = QSharedPointer<ViAudioBufferMutex>(new ViAudioBufferMutex());
	}
	return mWriteInstance.data();
}
