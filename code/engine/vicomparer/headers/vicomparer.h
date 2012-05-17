#ifndef VICOMPARER_H
#define VICOMPARER_H

#include "viaudiobuffer.h"
#include "vicompareresult.h"

class ViComparer
{

	signals:
		
		void resultChanged(ViCompareResult result);
		void finished(ViCompareResult result);

	public:
		
		ViComparer(ViAudioBuffer *originalBuffer, ViAudioBuffer *corectedBuffer);
		void compare();
		ViCompareResult result();

	private:

		ViAudioBuffer *mOriginalBuffer;
		ViAudioBuffer *mCorrectedBuffer;
		ViCompareResult mResult;
		
};

#endif
