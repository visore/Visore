#ifndef VIAUDIOOBJECT_H
#define VIAUDIOOBJECT_H

#include "vibuffer.h"
#include <QMutex>
#include <QMutexLocker>

class ViAudioObject : public QObject
{

    Q_OBJECT

	signals:

		void finished(); // emitted when all writing to buffers has finished

	public:

		ViAudioObject(bool autoDestruct = true); //autoDestruct determines if the buffers will be deleted automatically.
		ViAudioObject(ViBuffer *original, ViBuffer *corrected, bool autoDestruct = true);
		~ViAudioObject();
		
		void setSong(bool song = true); //If the buffers represent a song, or if they are just intermediate buffers
		void setBuffers(ViBuffer *original, ViBuffer *corrected);
		void setOriginalBuffer(ViBuffer *buffer);
		void setCorrectedBuffer(ViBuffer *buffer);

		bool isSong();
		void clearBuffers();
		void clearOriginalBuffer();
		void clearCorrectedBuffer();
		ViBuffer* originalBuffer();
		ViBuffer* correctedBuffer();

		void setFinished(bool isFinished = true);
		bool isFinished();

	private:

		QMutex mMutex;
		bool mAutoDestruct;
		bool mIsFinished;
		bool mIsSong;
		ViBuffer *mOriginalBuffer;
		ViBuffer *mCorrectedBuffer;
};

#endif
