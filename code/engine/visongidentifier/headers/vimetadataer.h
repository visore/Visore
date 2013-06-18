#ifndef VIMETADATAER_H
#define VIMETADATAER_H

#include <visongidentifier.h>
#include <vicoverretriever.h>
#include <viaudioobject.h>

class ViMetadataer : public QObject
{

	Q_OBJECT

	signals:

		void finished(bool success);

	private slots:

		void processIdentification(bool success);
		void processRetrieval(bool success);

	public:

		ViMetadataer();
		~ViMetadataer();
		bool detect(ViAudioObjectPointer object);
		bool detect(ViAudioObjectPointer object, ViAudio::Type type);
		bool detect(ViBuffer *buffer);
		bool detect(QList<ViBuffer*> buffers);
		bool detected();
		ViMetadata metadata();

	private:

		bool startNextIdentifier();
		bool startNextRetriever();
		void processNextBuffer();
		void reset();
		void finish(bool success);

	private:

		QList<ViSongIdentifier*> mIdentifiers;
		QList<ViCoverRetriever*> mRetrievers;

		ViAudioObjectPointer mObject;
		QQueue<ViBuffer*> mBuffers;
		ViBuffer *mBuffer;
		ViMetadata mMetadata;

		bool mDetected;
		int mCurrentIdentifier;
		int mCurrentRetriever;

};

#endif
