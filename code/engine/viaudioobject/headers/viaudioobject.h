#ifndef VIAUDIOOBJECT_H
#define VIAUDIOOBJECT_H

#include <viaudio.h>
#include <vifunctor.h>
#include <vibuffer.h>
#include <vielement.h>
#include <viaudiocoder.h>
#include <vilogger.h>
#include <vialigner.h>
#include <visongidentifier.h>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>

class ViWaveForm;
class ViWaveFormer;
class ViMetadataer;
class ViAudioObject;
typedef ViPointer<ViAudioObject> ViAudioObjectPointer;
typedef QList<QList<ViAudioObjectPointer> > ViAudioObjectMatrix;
typedef QList<ViAudioObjectPointer> ViAudioObjectList;
typedef QQueue<ViAudioObjectPointer> ViAudioObjectQueue;

class ViAudioObject : public QObject, public ViFunctorParameter, public ViId
{

    Q_OBJECT

	public:

		/*******************************************************************************************************************

			ENUMERATIONS

		*******************************************************************************************************************/

		//Values are important
		enum Type
		{
			Undefined = 0x1,
			Target = 0x2,
			Corrupted = 0x4,
			Corrected = 0x8,
			Temporary = 0x10,
			Correlated = Corrected,
			Temp = Temporary,
			All = Target | Corrupted | Corrected | Temporary
		};

		//Values are important
		enum Resource
		{
			None = 0x1,
			File = 0x2,
			Buffer = 0x4,
			Both = File | Buffer
		};

	signals:

		void finished(); // emitted when all writing to buffers has finished

		void progressed(qreal percentage);
		void statused(QString status);

		void decoded();
		void encoded();

		void aligned();

		void waved();

		void infoed(bool success);

	public slots:

		/*******************************************************************************************************************

			FORMATS

		*******************************************************************************************************************/

		void setOutputFormat(ViAudioFormat format);

	private slots:

		/*******************************************************************************************************************

			PROGRESS

		*******************************************************************************************************************/

		void progress(qreal progress);

		/*******************************************************************************************************************

			SLOTS

		*******************************************************************************************************************/

		void checkFinished();

		/*******************************************************************************************************************

			ENCODE & DECODE SLOTS

		*******************************************************************************************************************/

		void encodeNext();
		void decodeNext();

		/*******************************************************************************************************************

			ALIGN

		*******************************************************************************************************************/

		void initializeAlign();
		void alignNext();

		/*******************************************************************************************************************

			WAVEFORM

		*******************************************************************************************************************/

		void initializeWaveForm();
		void generateNextWaveForm();

		/*******************************************************************************************************************

			SONG INFO

		*******************************************************************************************************************/

		void finishDetection(bool success);

	public:

		/*******************************************************************************************************************

			CONSTRUCTORS, DESTRUCTORS & GENERAL

		*******************************************************************************************************************/

		static ViAudioObjectPointer create(ViAudioObject *object);
		static ViAudioObjectPointer create(bool autoDestruct = true);
		static ViAudioObjectPointer createNull();
		~ViAudioObject();

		static QQueue<ViAudioObject::Type> decomposeTypes(ViAudioObject::Type type, ViAudioObject::Type exclude = ViAudioObject::Undefined);
		static ViAudioObject::Type composeTypes(QQueue<ViAudioObject::Type> &types, ViAudioObject::Type exclude = ViAudioObject::Undefined);

		bool hasResource(ViAudioObject::Type type);
		ViAudioObject::Resource resourceAvailable(ViAudioObject::Type type);
		ViAudioObject::Type availableResources(ViAudioObject::Resource resource = ViAudioObject::Both);

		qreal length(ViAudioPosition::Unit unit = ViAudioPosition::Bytes);
		qreal length(ViAudioObject::Type type, ViAudioPosition::Unit unit = ViAudioPosition::Bytes);
		
		/*******************************************************************************************************************

			AUTO DESTRUCT

		*******************************************************************************************************************/

		void setAutoDestruct(bool destruct);
		void addDestructRule(ViAudioObject::Type type, bool destruct);

		/*******************************************************************************************************************

			ENCODE & DECODE

		*******************************************************************************************************************/

		bool encode(ViAudioFormat format, bool clearWhenFinished = false);
		bool encode(ViAudioObject::Type type, ViAudioFormat format, bool clearWhenFinished = false);
		bool encode(ViAudioObject::Type type = ViAudioObject::Undefined, bool clearWhenFinished = false);
		bool decode(ViAudioObject::Type type);

		/*******************************************************************************************************************

			ALIGN

		*******************************************************************************************************************/

		Q_INVOKABLE bool align();

		/*******************************************************************************************************************

			INPUT & OUTPUT

		*******************************************************************************************************************/

		void setType(ViAudioObject::Type input, ViAudioObject::Type output);
		void setInputType(ViAudioObject::Type type);
		void setOutputType(ViAudioObject::Type type);

		ViAudioObject::Type inputType();
		ViAudioObject::Type outputType();
		
		/*******************************************************************************************************************

			BUFFERS

		*******************************************************************************************************************/

		// Transfer a certain buffer from object to this object.
		// If type is UnknownType, the output buffer will be used.
		// The ownership of the buffer will be transfered to this object, hence object will not delete it.
		// The file path is also transfered.
		void transferBuffer(ViAudioObjectPointer object, ViAudioObject::Type type = ViAudioObject::Undefined);

		ViBuffer* buffer(ViAudioObject::Type type, bool dontCreate = false); // dontCreate will not create buffers if they are NULL
		ViBuffer* targetBuffer(bool dontCreate = false);
		ViBuffer* corruptedBuffer(bool dontCreate = false);
		ViBuffer* correctedBuffer(bool dontCreate = false);
		ViBuffer* temporaryBuffer(bool dontCreate = false);

		ViBuffer* inputBuffer(bool dontCreate = false); // returns the buffer that will be used as input for the processing chain
		ViBuffer* outputBuffer(bool dontCreate = false); // returns the buffer that will be used as output for the processing chain

		void setBuffer(ViAudioObject::Type type, ViBuffer *buffer);
		void setTargetBuffer(ViBuffer *buffer);
		void setCorruptedBuffer(ViBuffer *buffer);
		void setCorrectedBuffer(ViBuffer *buffer);

		void clearBuffers(ViAudioObject::Type type = ViAudioObject::All);
		void clearBuffer(ViAudioObject::Type type);
		void clearTargetBuffer();
		void clearCorruptedBuffer();
		void clearCorrectedBuffer();
		void clearTemporaryBuffer();

		bool hasBuffer(ViAudioObject::Type type);
		bool hasInputBuffer();
		bool hasOutputBuffer();

		/*******************************************************************************************************************

			FILES

		*******************************************************************************************************************/

		QString filePath(ViAudioObject::Type type);
		QString targetFilePath();
		QString corruptedFilePath();
		QString correctedFilePath();

		void setFilePath(ViAudioObject::Type type, QString path);
		void setTargetFilePath(QString path);
		void setCorruptedFilePath(QString path);
		void setCorrectedFilePath(QString path);

		bool hasFile(ViAudioObject::Type type);

		/*******************************************************************************************************************

			FORMATS

		*******************************************************************************************************************/

		ViAudioFormat format(ViAudioObject::Type type);
		ViAudioFormat targetFormat();
		ViAudioFormat corruptedFormat();
		ViAudioFormat correctedFormat();
		ViAudioFormat inputFormat();
		ViAudioFormat outputFormat();

		/*******************************************************************************************************************

			WAVEFORM

		*******************************************************************************************************************/

		bool generateWaveForm(ViAudioObject::Type types);
		void setWaveForm(ViAudioObject::Type type, ViWaveForm *form);
		ViWaveForm* waveForm(ViAudioObject::Type type);

		/*******************************************************************************************************************

			SONG INFO

		*******************************************************************************************************************/

		ViSongInfo& songInfo();
		void setSongInfo(ViSongInfo info);
		void detectSongInfo();
		bool isDetectingSongInfo();



		void setSong(bool song = true); //If the buffers represent a song, or if they are just intermediate buffers

		bool isSong();

		bool isFinished();
		bool isUsed(QIODevice::OpenMode mode = QIODevice::ReadWrite);

		void addCorrelation(const ViElement &correlation);
		ViElementList& correlations();

	private:

		/*******************************************************************************************************************

			CONSTRUCTORS, DESTRUCTORS & GENERAL

		*******************************************************************************************************************/

		ViAudioObject(bool autoDestruct); // autoDestruct determines if the buffers will be deleted automatically.

		/*******************************************************************************************************************

			PROGRESS

		*******************************************************************************************************************/

		void setProgress(qreal parts, qreal ratio);
		void setProgress(qreal parts);

	private:

		ViAudioObjectPointer thisPointer; // Keep a ViPointer to own object. Needed for passing it to ViProcessor

		ViAudioObject::Type mInputType;
		ViAudioObject::Type mOutputType;
		ViAudioObject::Type mDestructType;

		ViBuffer *mTargetBuffer;
		ViBuffer *mCorruptedBuffer;
		ViBuffer *mCorrectedBuffer;
		ViBuffer *mTemporaryBuffer;

		QString mTargetFile;
		QString mCorruptedFile;
		QString mCorrectedFile;

		QMutex mMutex;
		bool mIsFinished;
		bool mIsSong;

		ViElementList mCorrelations;

		/*******************************************************************************************************************

			PROGRESS

		*******************************************************************************************************************/

		qreal mProgressRatio;
		qreal mProgressParts;
		qreal mProgress;

		/*******************************************************************************************************************

			FORMATS

		*******************************************************************************************************************/

		ViAudioFormat mOutputFormat;

		/*******************************************************************************************************************

			ENCODE & DECODE

		*******************************************************************************************************************/

		ViAudioCoder *mCoder;
		QQueue<ViAudioObject::Type> mCodingInstructions;
		bool mClearEncodedBuffer;
		ViAudioObject::Type mPreviousEncodedType;

		/*******************************************************************************************************************

			ALIGN

		*******************************************************************************************************************/

		ViAligner *mAligner;
		QQueue<ViAudioObject::Type> mAlignerInstructions;
		ViAudioObject::Type mAlignerTypes;
		ViAudioObject::Type mMainAligner;

		/*******************************************************************************************************************

			WAVEFORM

		*******************************************************************************************************************/

		ViWaveFormer *mWaveFormer;
		QQueue<ViAudioObject::Type> mWaveInstructions;
		QMap<ViAudioObject::Type, ViWaveForm*> mWaveForms;

		/*******************************************************************************************************************

			SONG INFO

		*******************************************************************************************************************/

		ViMetadataer *mMetadataer;
		ViMetadata mMetadata;
		ViSongInfo mSongInfo;
		bool mIsDetectingInfo;

};

#endif
