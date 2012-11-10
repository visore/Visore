#ifndef VIAUDIOOBJECT_H
#define VIAUDIOOBJECT_H

#include "vifunctor.h"
#include "vibuffer.h"
#include "viaudioobjectpointer.h"
#include "vielement.h"
#include <QMutex>
#include <QMutexLocker>

class ViAudioObject;
typedef ViPointer<ViAudioObject> ViAudioObjectPointer;

class ViAudioObject : public QObject, public ViFunctorParameter
{

    Q_OBJECT

	signals:

		void finished(); // emitted when all writing to buffers has finished

	public:

		static ViAudioObjectPointer create(ViAudioObject *object);
		static ViAudioObjectPointer create(bool autoDestruct = true);
		static ViAudioObjectPointer create(ViBuffer *original, ViBuffer *corrected, bool autoDestruct = true);
		static ViAudioObjectPointer createNull();
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

		void addCorrelation(const ViElement &correlation);
		ViElementList& correlations();

	private:

		ViAudioObject(bool autoDestruct); //autoDestruct determines if the buffers will be deleted automatically.
		ViAudioObject(ViBuffer *original, ViBuffer *corrected, bool autoDestruct);

	private:

		QMutex mMutex;
		bool mAutoDestruct;
		bool mIsFinished;
		bool mIsSong;
		ViBuffer *mOriginalBuffer;
		ViBuffer *mCorrectedBuffer;
		ViElementList mCorrelations;
};

#endif
