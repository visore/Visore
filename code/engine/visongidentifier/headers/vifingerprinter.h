#ifndef VIFINGERPRINTER_H
#define VIFINGERPRINTER_H

#include <vibuffer.h>
#include <viaudioposition.h>
#include <QThread>

class ViBufferOffsets
{

	public:

		ViBufferOffsets();
		ViBufferOffsets(ViBuffer *buffer, int from = 0, int to = 0);
		ViBufferOffsets(const ViBufferOffsets &other);

		int from();
		int to();
		ViBuffer* buffer();
		void clear();
		bool isValid();

	private:

		int mFrom;
		int mTo;
		ViBuffer *mBuffer;

};

class ViFingerprinterThread : public QThread
{

	public:

		ViFingerprinterThread();
		virtual ~ViFingerprinterThread();
		void setBufferOffset(ViBufferOffsets bufferOffset);
		void generate();
		QString fingerprint();
		ViAudioPosition duration();
		virtual void run() = 0;

	protected:

		void setFingerprint(QString fingerprint);
		void setDuration(ViAudioPosition duration);

	private:

		void start();

	protected:

		ViBufferOffsets mBufferOffset;
		ViBufferStreamPointer mStream;

	private:

		QString mFingerprint;
		ViAudioPosition mDuration;

};

class ViFingerprinter : public QObject
{

	Q_OBJECT

	signals:

		void generated();

	private slots:

		void finishFingerprint();

	public:

		ViFingerprinter();
		virtual ~ViFingerprinter();

		QString fingerprint();
		ViAudioPosition duration();

		virtual void generate(ViBufferOffsets bufferOffset) = 0;

	protected:

		void setThread(ViFingerprinterThread *thread);
		ViFingerprinterThread* thread();

	private:

		QString mFingerprint;
		ViAudioPosition mDuration;
		ViFingerprinterThread *mThread;

};

#endif
