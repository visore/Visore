#ifndef VIAUDIOOBJECT_H
#define VIAUDIOOBJECT_H

#include <vicommon.h>
#include <vifunctor.h>
#include <vibuffer.h>
#include <vielement.h>
#include <viaudiocoder.h>
#include <vilogger.h>
#include <vialigner.h>
#include <visongidentifier.h>
#include <vicorrelationgroup.h>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>

class ViWaveForm;
class ViWaveFormer;
class ViMetadataer;
class ViAudioObject;
class ViModifyProcessor;
class ViDualProcessor;
class ViCorrelator;

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
		enum Resource
		{
			None = 0x1,
			File = 0x2,
			Buffer = 0x4,
			Both = File | Buffer
		};

	signals:

		void started();
		void finished();
        void changed(); //Project is saved once this signal is emitted;

		void progressed(qreal percentage);
		void statused(QString status);

		void decoded();
		void encoded();

		void aligned();

		void waved();

        void correctorChanged();
		void corrected();

        void correlated();

		void infoed(bool success);

	public slots:

		/*******************************************************************************************************************

			FORMATS

		*******************************************************************************************************************/

        void setFormat(ViAudio::Type type, ViAudioFormat format);
        void setTargetFormat(ViAudioFormat format);
        void setCorruptedFormat(ViAudioFormat format);
        void setCorrectedFormat(ViAudioFormat format);

	private slots:

		/*******************************************************************************************************************

			LOGGING

		*******************************************************************************************************************/

		void log(QString message, QtMsgType type = QtDebugMsg);
		void logStatus(QString message, QtMsgType type = QtDebugMsg); //Logs and emits status

		/*******************************************************************************************************************

			PROGRESS

		*******************************************************************************************************************/

		void setFinished();
		void setStarted();
		void progress(qreal progress);

		/*******************************************************************************************************************

			ENCODE & DECODE SLOTS

		*******************************************************************************************************************/

		void encodeNext();
		void decodeNext();

		/*******************************************************************************************************************

			ALIGN

		*******************************************************************************************************************/

		void alignNext();

		/*******************************************************************************************************************

			WAVEFORM

		*******************************************************************************************************************/

		void initializeWaveForm();
		void generateNextWaveForm();

		/*******************************************************************************************************************

			CORRECTION

		*******************************************************************************************************************/

		void endCorrect();

        /*******************************************************************************************************************

            CORRELATE

        *******************************************************************************************************************/

        void correlateNext();

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

		static QQueue<ViAudio::Type> decomposeTypes(ViAudio::Type type, ViAudio::Type exclude = ViAudio::Undefined);
		static ViAudio::Type composeTypes(QQueue<ViAudio::Type> &types, ViAudio::Type exclude = ViAudio::Undefined);

		bool hasResource(ViAudio::Type type);
		ViAudioObject::Resource resourceAvailable(ViAudio::Type type);
		ViAudio::Type availableResources(ViAudioObject::Resource resource = ViAudioObject::Both);

		qreal length(ViAudioPosition::Unit unit = ViAudioPosition::Bytes);
		qreal length(ViAudio::Type type, ViAudioPosition::Unit unit = ViAudioPosition::Bytes);
		
		/*******************************************************************************************************************

			AUTO DESTRUCT

		*******************************************************************************************************************/

		void setAutoDestruct(bool destruct);
		void addDestructRule(ViAudio::Type type, bool destruct);

		/*******************************************************************************************************************

			PROGRESS

		*******************************************************************************************************************/

		bool isFinished();
		bool isBusy();

		/*******************************************************************************************************************

			ENCODE & DECODE

		*******************************************************************************************************************/

		void setEncoder(ViAudioCoder *coder); //Takes ownership
		bool hasEncoder();
        Q_INVOKABLE bool encode(int type);
		Q_INVOKABLE bool encode(ViAudio::Type type = ViAudio::All, bool clearWhenFinished = false);
		
		void setDecoder(ViAudioCoder *coder); //Takes ownership
		bool hasDecoder();
        Q_INVOKABLE bool decode(int type);
        Q_INVOKABLE bool decode(ViAudio::Type type = ViAudio::All);

		/*******************************************************************************************************************

			ALIGN

		*******************************************************************************************************************/

		void setAligner(ViAligner *aligner); //Takes ownership
		bool hasAligner();
		Q_INVOKABLE bool align(ViAligner *aligner = NULL); //Takes ownership
		
		/*******************************************************************************************************************

			BUFFERS

		*******************************************************************************************************************/

		// Transfer a certain buffer from object to this object.
		// The ownership of the buffer will be transfered to this object, hence object will not delete it.
		// The file path is also transfered.
		void transferBuffer(ViAudioObjectPointer object, ViAudio::Type type = ViAudio::Undefined);

		ViBuffer* buffer(ViAudio::Type type, bool dontCreate = false); // dontCreate will not create buffers if they are NULL
		ViBuffer* targetBuffer(bool dontCreate = false);
		ViBuffer* corruptedBuffer(bool dontCreate = false);
		ViBuffer* correctedBuffer(bool dontCreate = false);
		ViBuffer* temporaryBuffer(bool dontCreate = false);

		void setBuffer(ViAudio::Type type, ViBuffer *buffer);
		void setTargetBuffer(ViBuffer *buffer);
		void setCorruptedBuffer(ViBuffer *buffer);
		void setCorrectedBuffer(ViBuffer *buffer);

		Q_INVOKABLE void clearBuffers(ViAudio::Type type = ViAudio::All);
		Q_INVOKABLE void clearBuffer(ViAudio::Type type);
		Q_INVOKABLE void clearTargetBuffer();
		Q_INVOKABLE void clearCorruptedBuffer();
		Q_INVOKABLE void clearCorrectedBuffer();
		Q_INVOKABLE void clearTemporaryBuffer();

		bool hasBuffer(ViAudio::Type type);

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

		bool hasFile(ViAudio::Type type);

		QString fileName(bool track = true, bool side = false);
		QString temporaryFilePath(ViAudio::Type type);

		/*******************************************************************************************************************

			FORMATS

		*******************************************************************************************************************/

		ViAudioFormat format(ViAudio::Type type);
		ViAudioFormat targetFormat();
		ViAudioFormat corruptedFormat();
		ViAudioFormat correctedFormat();
		ViAudioFormat inputFormat();

		/*******************************************************************************************************************

			WAVEFORM

		*******************************************************************************************************************/

		bool generateWaveForm(ViAudio::Type types);
		void setWaveForm(ViAudio::Type type, ViWaveForm *form);
		ViWaveForm* waveForm(ViAudio::Type type);

        /*******************************************************************************************************************

            CORRECTION

        *******************************************************************************************************************/

        void setCorrector(ViModifyProcessor *corrector); //Takes ownership
        bool hasCorrector();
        ViModifyProcessor* corrector();
        Q_INVOKABLE bool correct(ViModifyProcessor *corrector = NULL); //Takes ownership

        /*******************************************************************************************************************

            CORRELATE

        *******************************************************************************************************************/

        ViCorrelation correlation(QString correlator, ViAudio::Type type1 = ViAudio::Target, ViAudio::Type type2 = ViAudio::Corrected);
        ViCorrelationGroup correlation(ViAudio::Type type1 = ViAudio::Target, ViAudio::Type type2 = ViAudio::Corrected);
        ViCorrelationGroups correlations();

        void clearCorrelators();
        void addCorrelator(ViCorrelator *correlator); //Takes ownership
        bool hasCorrelator();
        int correlatorCount();

        Q_INVOKABLE bool correlate(ViCorrelator *correlator); //Takes ownership
        Q_INVOKABLE bool correlate(QList<ViCorrelator*> correlators); //Takes ownership
        Q_INVOKABLE bool correlate();

		/*******************************************************************************************************************

			SONG INFO

		*******************************************************************************************************************/

		ViSongInfo& songInfo();
		void setSongInfo(ViSongInfo info);
		Q_INVOKABLE void detectSongInfo();
		bool isDetectingSongInfo();
        bool hasSongInfo();

		void setSideNumber(int side);
		int sideNumber();

		void setTrackNumber(int track);
		int trackNumber();
















		void setSong(bool song = true); //If the buffers represent a song, or if they are just intermediate buffers

		bool isSong();

		bool isUsed(QIODevice::OpenMode mode = QIODevice::ReadWrite);

	private:

		/*******************************************************************************************************************

			CONSTRUCTORS, DESTRUCTORS & GENERAL

		*******************************************************************************************************************/

		ViAudioObject(bool autoDestruct); // autoDestruct determines if the buffers will be deleted automatically.

		/*******************************************************************************************************************

			PROGRESS

		*******************************************************************************************************************/

		void setProgress(qreal parts);

	private:

		ViAudioObjectPointer thisPointer; // Keep a ViPointer to own object. Needed for passing it to ViProcessor

		ViAudio::Type mDestructType;

		ViBuffer *mTargetBuffer;
		ViBuffer *mCorruptedBuffer;
		ViBuffer *mCorrectedBuffer;
		ViBuffer *mTemporaryBuffer;

		QString mTargetFile;
		QString mCorruptedFile;
		QString mCorrectedFile;

		QMutex mMutex;
		bool mIsSong;

		/*******************************************************************************************************************

			PROGRESS

		*******************************************************************************************************************/

		qreal mProgressParts;
		qreal mProgress;
		bool mIsBusy;

		/*******************************************************************************************************************

			FORMATS

		*******************************************************************************************************************/

        ViAudioFormat mTargetFormat;
        ViAudioFormat mCorruptedFormat;
        ViAudioFormat mCorrectedFormat;

		/*******************************************************************************************************************

			ENCODE & DECODE

		*******************************************************************************************************************/

		ViAudioCoder *mEncoder;
		ViAudioCoder *mDecoder;

		QQueue<ViAudio::Type> mCodingInstructions;
		bool mClearEncodedBuffer;
		ViAudio::Type mPreviousEncodedType;

		/*******************************************************************************************************************

			ALIGN

		*******************************************************************************************************************/

		ViAligner *mAligner;
		QQueue<ViAudio::Type> mAlignerInstructions;
		ViAudio::Type mAlignerTypes;
		ViAudio::Type mMainAligner;

		/*******************************************************************************************************************

			WAVEFORM

		*******************************************************************************************************************/

		ViWaveFormer *mWaveFormer;
		QQueue<ViAudio::Type> mWaveInstructions;
		QMap<ViAudio::Type, ViWaveForm*> mWaveForms;

		/*******************************************************************************************************************

			CORRECTION

		*******************************************************************************************************************/

		ViModifyProcessor *mCorrector;

        /*******************************************************************************************************************

            CORRELATE

        *******************************************************************************************************************/

        QList<ViCorrelator*> mCorrelators;
        QQueue<QPair<ViAudio::Type, ViAudio::Type> > mCorrelationTypes;
        int mCurrentCorrelator;
        int mCurrentCorrelation;
        ViCorrelationGroups mCorrelations;

		/*******************************************************************************************************************

			SONG INFO

		*******************************************************************************************************************/

		ViMetadataer *mMetadataer;
		ViMetadata mMetadata;
		ViSongInfo mSongInfo;
		bool mIsDetectingInfo;
		int mSideNumber;
		int mTrackNumber;

};

#endif
