#ifndef VIECHOIDENTIFIER_H
#define VIECHOIDENTIFIER_H

#include <QUrl>
#include <QFile>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QNetworkAccessManager>
#include "vibuffer.h"
#include "viechoresponse.h"
#include "viaudiocoder.h"
#include "visongidentifier.h"

class ViEchoCodeThread : public QThread
{

	Q_OBJECT

	signals:

		void finished(QString code, QString version, int codeLength);

	public:

		ViEchoCodeThread(QObject *parent = 0);
		void setData(QByteArray *data);
		void setOffset(int offset);

	protected:

		void run();
		virtual void generateCode(const QByteArray *data, int offset, QString &code, QString &version, int &codeLength) = 0;

	protected:

		QByteArray *mData;
		int mOffset;

};

class ViEchoIdentifier : public ViSongIdentifier
{

	Q_OBJECT

	public:

		enum State
		{
			Idle = 0,
			Encoding = 1,
			CodeGeneration = 2,
			SongDetection = 3,
			ImageRetrieval = 4
		};

		enum Error
		{
			NoError = 0,
			NetworkError = 1,
			KeyError = 2,
			CodeError = 3
		};

	signals:

		void stateChanged(ViEchoIdentifier::State state);

	private slots:

		void codeFinished(QString code, QString version, int codeLength);
		void replyFinished(QNetworkReply *reply);
		void downloadFinished(QNetworkReply *reply);
		void encodingFinished();

	public:

		ViEchoIdentifier();
		~ViEchoIdentifier();

		bool identify(ViBuffer *buffer);

		void setProxy(QNetworkProxy::ProxyType type, QString host, quint16 port, QString username, QString password);
		void setKey(QString key);

		ViEchoIdentifier::State state();
		ViEchoIdentifier::Error error();
		QNetworkReply::NetworkError networkError();
		QList<ViSongInfo> songInfo();

	protected:

		void reset();
		void clearThread();
		virtual ViEchoCodeThread* codeThread() = 0;
		virtual ViAudioFormat encodingFormat() = 0;

	private:

		void setState(ViEchoIdentifier::State state);

	private:

		QByteArray mOutput;
		ViAudioCoder mCoder;

		ViEchoResponse mResponse;
		ViEchoCodeThread *mThread;

		QNetworkAccessManager *mNetworkManager;

		QString mKey;

		ViEchoIdentifier::State mState;
		ViEchoIdentifier::Error mError;
		QNetworkReply::NetworkError mNetworkError;

};

#endif
