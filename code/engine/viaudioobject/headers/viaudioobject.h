#ifndef VIAUDIOOBJECT_H
#define VIAUDIOOBJECT_H

#include "vifunctor.h"
#include "vibuffer.h"
#include "viaudioobjectpointer.h"
#include "vielement.h"
#include "visonginfo.h"
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

		/*******************************************************************************************************************

			CONSTRUCTORS & DESTRUCTORS

		*******************************************************************************************************************/

		static ViAudioObjectPointer create(ViAudioObject *object);
		static ViAudioObjectPointer create(bool autoDestruct = true);
		static ViAudioObjectPointer create(ViBuffer *original, ViBuffer *corrected, bool autoDestruct = true);
		static ViAudioObjectPointer createNull();
		~ViAudioObject();
		
		/*******************************************************************************************************************

			BUFFERS

		*******************************************************************************************************************/

		ViBuffer* targetBuffer();
		ViBuffer* originalBuffer();
		ViBuffer* correctedBuffer();

		void setTargetBuffer(ViBuffer *buffer);
		void setOriginalBuffer(ViBuffer *buffer);
		void setCorrectedBuffer(ViBuffer *buffer);

		void clearBuffers();
		void clearTargetBuffer();
		void clearOriginalBuffer();
		void clearCorrectedBuffer();

		/*******************************************************************************************************************

			FILES

		*******************************************************************************************************************/

		QString targetFile();
		QString originalFile();
		QString correctedFile();

		void setTargetFile(QString path);
		void setOriginalFile(QString path);
		void setCorrectedFile(QString path);

		/*******************************************************************************************************************

			SONG INFO

		*******************************************************************************************************************/

		ViSongInfo songInfo();
		void setSongInfo(ViSongInfo info);




		void setSong(bool song = true); //If the buffers represent a song, or if they are just intermediate buffers
		void setBuffers(ViBuffer *original, ViBuffer *corrected);

		bool isSong();



		void setFinished(bool isFinished = true);
		bool isFinished();

		void addCorrelation(const ViElement &correlation);
		ViElementList& correlations();

	private:

		ViAudioObject(bool autoDestruct); //autoDestruct determines if the buffers will be deleted automatically.
		ViAudioObject(ViBuffer *original, ViBuffer *corrected, bool autoDestruct);

	private:

		ViBuffer *mTargetBuffer;
		ViBuffer *mOriginalBuffer;
		ViBuffer *mCorrectedBuffer;

		QString mTargetFile;
		QString mOriginalFile;
		QString mCorrectedFile;

		ViSongInfo mSongInfo;



		QMutex mMutex;
		bool mAutoDestruct;
		bool mIsFinished;
		bool mIsSong;

		ViElementList mCorrelations;
};

#endif
