#ifndef VIAUDIOOBJECT_H
#define VIAUDIOOBJECT_H

#include "viaudiobuffer.h"

class ViAudioObject : public QObject
{

    Q_OBJECT

	signals:

		void finished(); // emitted when all writing to buffers has finished

	public:

		ViAudioObject(bool autoDestruct = true); //autoDestruct determines if the buffers will be deleted automatically.
		ViAudioObject(ViAudioBuffer *original, ViAudioBuffer *corrected, bool autoDestruct = true);
		~ViAudioObject();
		
		void setBuffers(ViAudioBuffer *original, ViAudioBuffer *corrected);
		void setOriginalBuffer(ViAudioBuffer *buffer);
		void setCorrectedBuffer(ViAudioBuffer *buffer);
		void clearBuffers();
		void clearOriginalBuffer();
		void clearCorrectedBuffer();
		ViAudioBuffer* originalBuffer();
		ViAudioBuffer* correctedBuffer();

		void setFinished(bool isFinished = true);
		bool isFinished();

	private:

		bool mAutoDestruct;
		bool mIsFinished;
		ViAudioBuffer *mOriginalBuffer;
		ViAudioBuffer *mCorrectedBuffer;
};

#endif
