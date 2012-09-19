#ifndef VIAUDIOOBJECT_H
#define VIAUDIOOBJECT_H

#include "viaudiobuffer.h"

class ViAudioObject : public QObject
{

    Q_OBJECT


	public:

		ViAudioObject(bool autoDestruct = true); //autoDestruct determines if the buffers will be deleted automatically.
		ViAudioObject(ViAudioBuffer *original, ViAudioBuffer *corrected, bool autoDestruct = true);
		~ViAudioObject();
		
		void setBuffers(ViAudioBuffer *original, ViAudioBuffer *corrected);
		void setOriginalBuffer(ViAudioBuffer *buffer);
		void setCorrectedBuffer(ViAudioBuffer *buffer);
		void clearBuffers();

	private:

		bool mAutoDestruct;
		ViAudioBuffer *mOriginalBuffer;
		ViAudioBuffer *mCorrectedBuffer;
};

#endif
