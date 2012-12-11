#ifndef VIAUDIOOBJECT_H
#define VIAUDIOOBJECT_H

#include "vifunctor.h"
#include "vibuffer.h"
#include "vielement.h"
#include "visonginfo.h"
#include <QMutex>
#include <QMutexLocker>
#include "vilogger.h"

class ViAudioObject;
typedef ViPointer<ViAudioObject> ViAudioObjectPointer;

class ViAudioObject : public QObject, public ViFunctorParameter, public ViId
{

    Q_OBJECT

	signals:

		void finished(); // emitted when all writing to buffers has finished

	private slots:

		/*******************************************************************************************************************

			SLOTS

		*******************************************************************************************************************/

		void checkFinished();

	public:

		/*******************************************************************************************************************

			CONSTRUCTORS & DESTRUCTORS

		*******************************************************************************************************************/

		static ViAudioObjectPointer create(ViAudioObject *object);
		static ViAudioObjectPointer create(bool autoDestruct = true);
		static ViAudioObjectPointer createNull();
		~ViAudioObject();

		/*******************************************************************************************************************

			INPUT & OUTPUT

		*******************************************************************************************************************/

		enum Type
		{
			Unknown,
			Target,
			Corrupted,
			Corrected,
			Temporary,
			Temp = Temporary
		};
		void setType(ViAudioObject::Type input, ViAudioObject::Type output);
		void setInputType(ViAudioObject::Type type);
		void setOutputType(ViAudioObject::Type type);
		
		/*******************************************************************************************************************

			BUFFERS

		*******************************************************************************************************************/

		ViBuffer* targetBuffer();
		ViBuffer* corruptedBuffer();
		ViBuffer* correctedBuffer();
		ViBuffer* tempBuffer();

		ViBuffer* inputBuffer(); // returns the buffer that will be used as input for the processing chain
		ViBuffer* outputBuffer(); // returns the buffer that will be used as output for the processing chain

		void setTargetBuffer(ViBuffer *buffer);
		void setCorruptedBuffer(ViBuffer *buffer);
		void setCorrectedBuffer(ViBuffer *buffer);

		void clearBuffers();
		void clearTargetBuffer();
		void clearCorruptedBuffer();
		void clearCorrectedBuffer();
		void clearTempBuffer();

		/*******************************************************************************************************************

			FILES

		*******************************************************************************************************************/

		QString targetFile();
		QString corruptedFile();
		QString correctedFile();

		void setTargetFile(QString path);
		void setCorruptedFile(QString path);
		void setCorrectedFile(QString path);

		/*******************************************************************************************************************

			SONG INFO

		*******************************************************************************************************************/

		ViSongInfo& songInfo();
		void setSongInfo(ViSongInfo info);




		void setSong(bool song = true); //If the buffers represent a song, or if they are just intermediate buffers

		bool isSong();

		bool isFinished();
		bool isUsed(QIODevice::OpenMode mode = QIODevice::ReadWrite);

		void addCorrelation(const ViElement &correlation);
		ViElementList& correlations();

	private:

		ViAudioObject(bool autoDestruct); // autoDestruct determines if the buffers will be deleted automatically.

	private:

		ViAudioObject::Type mInputType;
		ViAudioObject::Type mOutputType;

		ViBuffer *mTargetBuffer;
		ViBuffer *mCorruptedBuffer;
		ViBuffer *mCorrectedBuffer;
		ViBuffer *mTempBuffer;

		QString mTargetFile;
		QString mCorruptedFile;
		QString mCorrectedFile;

		ViSongInfo mSongInfo;



		QMutex mMutex;
		bool mAutoDestruct;
		bool mIsFinished;
		bool mIsSong;

		ViElementList mCorrelations;
};

#endif
