#ifndef VIAUDIOOBJECT_H
#define VIAUDIOOBJECT_H

#include "viaudio.h"
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

			TYPE

		*******************************************************************************************************************/

		enum Type
		{
			None,
			Unknown,
			Target,
			Corrupted,
			Corrected,
			Temporary,
			Temp = Temporary,
			All = Target | Corrupted | Corrected | Temporary
		};

		/*******************************************************************************************************************

			CONSTRUCTORS & DESTRUCTORS

		*******************************************************************************************************************/

		static ViAudioObjectPointer create(ViAudioObject *object);
		static ViAudioObjectPointer create(bool autoDestruct = true);
		static ViAudioObjectPointer createNull();
		~ViAudioObject();

		/*******************************************************************************************************************

			AUTO DESTRUCT

		*******************************************************************************************************************/

		void setAutoDestruct(bool destruct);
		void addDestructRule(ViAudio::Type type, bool destruct);


		/*******************************************************************************************************************

			INPUT & OUTPUT

		*******************************************************************************************************************/

		void setType(ViAudio::Type input, ViAudio::Type output);
		void setInputType(ViAudio::Type type);
		void setOutputType(ViAudio::Type type);
		
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

		void clearBuffers(ViAudio::Type type = ViAudio::AllTypes);
		void clearTargetBuffer();
		void clearCorruptedBuffer();
		void clearCorrectedBuffer();
		void clearTempBuffer();

		/*******************************************************************************************************************

			FILES

		*******************************************************************************************************************/

		QString filePath(ViAudio::Type type);
		QString targetFilePath();
		QString corruptedFilePath();
		QString correctedFilePath();

		void setFilePath(ViAudio::Type type, QString path);
		void setTargetFilePath(QString path);
		void setCorruptedFilePath(QString path);
		void setCorrectedFilePath(QString path);

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

		ViAudio::Type mInputType;
		ViAudio::Type mOutputType;
		ViAudio::Type mDestructType;

		ViBuffer *mTargetBuffer;
		ViBuffer *mCorruptedBuffer;
		ViBuffer *mCorrectedBuffer;
		ViBuffer *mTempBuffer;

		QString mTargetFile;
		QString mCorruptedFile;
		QString mCorrectedFile;

		ViSongInfo mSongInfo;


		QMutex mMutex;
		bool mIsFinished;
		bool mIsSong;

		ViElementList mCorrelations;
};

#endif
