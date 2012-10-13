#ifndef VIAUDIOOBJECT_H
#define VIAUDIOOBJECT_H

#include "viaudiobuffer.h"
#include <QMutex>
#include <QMutexLocker>

class ViAudioObject : public QObject
{

    Q_OBJECT

	signals:

		void finished(); // emitted when all writing to buffers has finished

	public:

		ViAudioObject(bool autoDestruct = true); //autoDestruct determines if the buffers will be deleted automatically.
		ViAudioObject(ViAudioBuffer *original, ViAudioBuffer *corrected, bool autoDestruct = true);
		~ViAudioObject();
		
		void setSong(bool song = true); //If the buffers represent a song, or if they are just intermediate buffers
		void setBuffers(ViAudioBuffer *original, ViAudioBuffer *corrected);
		void setOriginalBuffer(ViAudioBuffer *buffer);
		void setCorrectedBuffer(ViAudioBuffer *buffer);

		bool isSong();
		void clearBuffers();
		void clearOriginalBuffer();
		void clearCorrectedBuffer();
		ViAudioBuffer* originalBuffer();
		ViAudioBuffer* correctedBuffer();

		void setFinished(bool isFinished = true);
		bool isFinished();

	private:

		QMutex mMutex;
		bool mAutoDestruct;
		bool mIsFinished;
		bool mIsSong;
		ViAudioBuffer *mOriginalBuffer;
		ViAudioBuffer *mCorrectedBuffer;
};

#endif
