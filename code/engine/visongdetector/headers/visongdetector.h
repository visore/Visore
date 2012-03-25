#ifndef VISONGDETECTOR_H
#define VISONGDETECTOR_H

#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QNetworkAccessManager>
#include "viaudiobuffer.h"
#include "viaudioprocessingchain.h"
#include "viaudioposition.h"
#include "visonginfo.h"
#include "viobject.h"

//Interval (milliseconds) use to request info
#define MAXIMUM_REQUESTS 3
#define REQUEST_INTERVAL_1 500
#define REQUEST_INTERVAL_2 7000
#define REQUEST_INTERVAL_3 10000

class ViSongCodeGeneratorThread : public QThread
{
	Q_OBJECT

	signals:
		void finished(QString code, QString version);

	public:
		ViSongCodeGeneratorThread(ViAudioBuffer *buffer, QObject *parent = 0);
		void setOffset(int offset);
		void run();

	private:
		ViAudioBuffer *mBuffer;
		int mOffset;
};

class ViSongDetector : public QObject
{
	Q_OBJECT

	public:
		enum State
		{
			Idle = 0,
			Busy = 1,
			Finished = 3
		};
		enum Result
		{
			None = 0,
			Timeout = 1,
			NetworkProblem = 2,
			NotFound = 3,
			Found = 4,
			KeyProblem = 5
		};

	signals:
		void stateChanged(ViSongDetector::State state);

	private slots:
		void positionChanged(ViAudioPosition position);
		void codeFinished(QString code, QString version);
		void replyFinished(QNetworkReply *reply);

	public:
		ViSongDetector(ViAudioOutput *output);
		~ViSongDetector();
		void setProxy(QNetworkProxy::ProxyType type, QString host, quint16 port, QString username, QString password);
		void setKey(QString key);
		ViSongDetector::State state();
		ViSongDetector::Result result();
		ViSongInfo songInfo();

	private:
		

	private:
		ViSongCodeGeneratorThread *mThread;
		ViAudioOutput *mOutput;
		QNetworkAccessManager *mNetworkManager;
		bool mKeyWasSet;
		ViSongDetector::State mState;
		ViSongDetector::Result mResult;
		ViSongInfo mSongInfo;
		qint8 mRequestsSent;
};

#endif
