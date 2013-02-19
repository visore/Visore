#ifndef VIACOUSTIDIDENTIFIER_H
#define VIACOUSTIDIDENTIFIER_H

#include <fingerprintcalculator.h>
#include <visongidentifier.h>
#include <QThread>

class ViAcoustidIdentifierThread : public QThread
{

	Q_OBJECT

	signals:

		void finished(QString code, int duration);

	public:

		ViAcoustidIdentifierThread();
		void start(ViBuffer *buffer);
		void run();

	private:

		ViBuffer *mBuffer;
		FingerprintCalculator mCalculator;

};

class ViAcoustidIdentifier : public ViSongIdentifier
{

	Q_OBJECT

	private slots:

		void retieveInfo(QString code, int duration);
		void processInfo(bool success);

	public:

		ViAcoustidIdentifier();

		void identify(ViBuffer *buffer);

	private:

		ViAcoustidIdentifierThread mThread;

};

#endif
