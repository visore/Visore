#ifndef VIALIGNER_H
#define VIALIGNER_H

#include <QThread>
#include <viaudioposition.h>
#include <vibuffer.h>

class ViAligner;

class ViAlignerThread : public QThread
{

	public:

		ViAlignerThread();
		void setAligner(ViAligner *aligner);
		void run();

	private:

		ViAligner *mAligner;

};

class ViAligner : public QObject
{

	friend class ViAlignerThread;

	Q_OBJECT

	signals:

		void started();
		void progressed(qreal progress);
		void finished();

	private slots:

		void alignBuffer();

	public:
		
		ViAligner();
		
		qint64 setMaximumDelay(qint64 samples);
		ViAudioPosition setMaximumDelay(ViAudioPosition delay);
		ViAudioPosition delay();

		void align(ViBuffer *buffer1, ViBuffer *buffer2, int maximumDelay = 0);

	protected:

		virtual void execute() = 0;
		qint64 setDelay(qint64 delay);

	protected:

		qint64 mMaximumDelay;
		ViSampleChunk mChunk1;
		ViSampleChunk mChunk2;
		ViBuffer *mBuffer1;
		ViBuffer *mBuffer2;
		ViAudioFormat mFormat1;
		ViAudioFormat mFormat2;

		ViAlignerThread mThread;

	private:

		qint64 mDelay;

};

#endif
