#include "viaudiobuffermutex.h"

QSharedPointer<ViAudioBufferMutex> ViAudioBufferMutex::mInstance;

ViAudioBufferMutex::ViAudioBufferMutex()
	: QMutex()
{
}

ViAudioBufferMutex* ViAudioBufferMutex::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViAudioBufferMutex>(new ViAudioBufferMutex());
	}
	return mInstance.data();
}
