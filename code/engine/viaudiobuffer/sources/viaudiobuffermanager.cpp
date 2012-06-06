#include "viaudiobuffermanager.h"

QSharedPointer<ViAudioBufferManager> ViAudioBufferManager::mInstance;

ViAudioBufferManager::ViAudioBufferManager()
{
}

ViAudioBufferManager::~ViAudioBufferManager()
{
	qDeleteAll(mBuffers);
	mBuffers.clear();
}

ViAudioBufferManager* ViAudioBufferManager::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViAudioBufferManager>(new ViAudioBufferManager());
	}
	return mInstance.data();
}

ViAudioBuffer* ViAudioBufferManager::allocate()
{
	ViAudioBufferManager *instance = ViAudioBufferManager::instance();
	ViAudioBuffer *buffer = new ViAudioBuffer();
	int id = 0;
	while(instance->mUsedIds.contains(id))
	{
		++id;
	}
	instance->mUsedIds.append(id);
	buffer->setId(id);
	return buffer;
}

bool ViAudioBufferManager::deallocate(int id)
{
	ViAudioBufferManager *instance = ViAudioBufferManager::instance();
	if(instance->mUsedIds.removeOne(id))
	{
		for(int i = 0; i < instance->mBuffers.size(); ++i)
		{
			if(instance->mBuffers[i]->id() == id)
			{
				delete instance->mBuffers[i];
				instance->mBuffers[i] = NULL;
				instance->mBuffers.removeAt(id);
				return true;
			}
		}
	}
	return false;
}

bool ViAudioBufferManager::deallocate(ViAudioBuffer *buffer)
{
	deallocate(buffer->id());
}

void ViAudioBufferManager::deallocateAll()
{
	ViAudioBufferManager *instance = ViAudioBufferManager::instance();
	for(int i = 0; i < instance->mBuffers.size(); ++i)
	{
		deallocate(instance->mBuffers[i]);
	}
}
