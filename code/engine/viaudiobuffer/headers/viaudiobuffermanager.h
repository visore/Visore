#ifndef VIAUDIOBUFFERMANAGER_H
#define VIAUDIOBUFFERMANAGER_H

#include <QList>
#include "viaudiobuffer.h"

class ViAudioBufferManager
{
	
	public:

		~ViAudioBufferManager();
		static ViAudioBuffer* allocate();
		static bool deallocate(int id);
		static bool deallocate(ViAudioBuffer *buffer);
		static void deallocateAll();

	protected:

		ViAudioBufferManager();
		static ViAudioBufferManager* instance();

	protected:

		static QSharedPointer<ViAudioBufferManager> mInstance;
		QList<ViAudioBuffer*> mBuffers;
		QList<int> mUsedIds;

};

#endif
