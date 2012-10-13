#ifndef VIMUTEXLOCKER_H
#define VIMUTEXLOCKER_H

#include "vimutex.h"

class ViMutexLocker
{

	public:
		
		ViMutexLocker(ViMutex *mutex);
		ViMutexLocker(ViMutex &mutex);
		~ViMutexLocker();
		void lock();
		void unlock();

	private:

		ViMutex *mMutex;

};

#endif
