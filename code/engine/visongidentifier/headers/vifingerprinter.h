#ifndef VIFINGERPRINTER_H
#define VIFINGERPRINTER_H

#include <vibuffer.h>
#include <QThread>

class ViFingerprinterThread : public QThread
{

	public:

		ViFingerprinterThread();
		virtual ~ViFingerprinterThread();
		void setBuffer(ViBuffer *buffer);
		void generate();
		QString fingerprint();
		virtual void run() = 0;

	protected:

		void setFingerprint(QString fingerprint);

	private:

		void start();

	protected:

		ViBufferStreamPointer mStream;

	private:

		QString mFingerprint;

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

		virtual void generate(ViBuffer *buffer) = 0;

	protected:

		void setThread(ViFingerprinterThread *thread);
		ViFingerprinterThread* thread();

	private:

		QString mFingerprint;
		ViFingerprinterThread *mThread;

};

#endif
