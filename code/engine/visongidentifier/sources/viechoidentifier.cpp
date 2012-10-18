#include "viechoidentifier.h"
#include "vimanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>

ViEchoCodeThread::ViEchoCodeThread(QObject *parent)
	: QThread(parent)
{
	mOffset = 0;
}

void ViEchoCodeThread::setData(QByteArray *data)
{
	mData = data;
}

void ViEchoCodeThread::setOffset(int offset)
{
	mOffset = offset;
}

void ViEchoCodeThread::run()
{
	QString code;
	QString version;
	int length;
	generateCode(mData, mOffset, code, version, length);
	emit finished(code, version, length);
	mData->clear();
}

ViEchoIdentifier::ViEchoIdentifier()
	: ViSongIdentifier()
{
	mNetworkManager = new QNetworkAccessManager();

	mState = ViEchoIdentifier::Idle;
	mError = ViEchoIdentifier::NoError;
	mNetworkError = QNetworkReply::NoError;
	mKey = "";
	mThread = NULL;
	
	QObject::connect(&mCoder, SIGNAL(finished()), this, SLOT(encodingFinished()));
}

ViEchoIdentifier::~ViEchoIdentifier()
{
	clearThread();
	delete mNetworkManager;
}

void ViEchoIdentifier::reset()
{
	mNetworkError = QNetworkReply::NoError;
	mError = ViEchoIdentifier::NoError;
	mFound = false;
	mResponse.reset();
	clearThread();
}

void ViEchoIdentifier::clearThread()
{
	if(mThread != NULL)
	{
		mThread->quit();
		while(mThread->isRunning());
		QObject::disconnect(mThread, SIGNAL(finished(QString, QString, int)), this, SLOT(codeFinished(QString, QString, int)));
		delete mThread;
		mThread = NULL;
	}
}

bool ViEchoIdentifier::identify(ViBuffer *buffer)
{
	reset();
	if(mError != ViEchoIdentifier::NetworkError && !mFound)
	{
		if(mKey == "")
		{
			mError = ViEchoIdentifier::KeyError;
		}
		else
		{
			setState(ViEchoIdentifier::Encoding);
			mOutput.clear();
			mCoder.encode(buffer, mOutput, encodingFormat());
			return true;
		}
	}
	return false;
}

void ViEchoIdentifier::encodingFinished()
{
	if(mCoder.error() == ViCoder::NoError)
	{
		clearThread();
		mThread = codeThread();
		QObject::connect(mThread, SIGNAL(finished(QString, QString, int)), this, SLOT(codeFinished(QString, QString, int)));
		setState(ViEchoIdentifier::CodeGeneration);
		mThread->setData(&mOutput);
		mThread->start();
	}
	else
	{
		mOutput.clear();
	}
}

void ViEchoIdentifier::codeFinished(QString code, QString version, int codeLength)
{
	clearThread();
	if(code == "")
	{
		mError = ViEchoIdentifier::CodeError;
		setState(ViEchoIdentifier::Idle);
		return;
	}
	setState(ViEchoIdentifier::SongDetection);
	QObject::connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)), Qt::UniqueConnection);
	QUrl url("http://developer.echonest.com/api/v4/song/identify?api_key=" + mKey + "&version=" + version + "&bucket=id:7digital-US&bucket=tracks&bucket=audio_summary&bucket=artist_familiarity&bucket=artist_hotttnesss&bucket=song_hotttnesss");
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/octet-stream"));
	QJsonObject jsonObject;
	jsonObject.insert("code", code);
	mNetworkManager->post(request, QJsonDocument(jsonObject).toJson());
}

void ViEchoIdentifier::replyFinished(QNetworkReply *reply)
{
	mNetworkError = reply->error();
	if(mNetworkError != QNetworkReply::NoError)
	{
		mError = ViEchoIdentifier::NetworkError;
		setState(ViEchoIdentifier::Idle);
		return;
	}
	mResponse.analyze(reply->readAll());
	if(mResponse.message().contains("api") && mResponse.message().contains("key"))
	{
		mError = ViEchoIdentifier::KeyError;
		setState(ViEchoIdentifier::Idle);
		return;
	}
	else if(mResponse.numberOfSongs() > 0)
	{
		mNetworkManager->disconnect();
		if(mResponse.songInfo().imagePath() != "")
		{
			mFound = true;
			QFileInfo info(mResponse.songInfo().imagePath());
			QString imagePath = ViManager::tempPath() + QDir::separator() + "albumart" + QDir::separator() + mResponse.songInfo().songId() + "." + info.suffix();
			QFile image(imagePath);
			if(image.exists())
			{
				mResponse.songInfo().changeImagePath(mResponse.songInfo().imagePath(), imagePath);
				setState(ViEchoIdentifier::Idle);
			}
			else
			{
				setState(ViEchoIdentifier::ImageRetrieval);
				QObject::connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));
				QUrl url(mResponse.songInfo().imagePath());
				QNetworkRequest request(url);
				mNetworkManager->get(request);
			}
			detected(mResponse.songInfo());
		}
		else
		{
			QUrl url("http://developer.echonest.com/api/v4/artist/images?api_key=" + mKey + "&id=" + mResponse.songInfo().artistId() + "&format=json&results=1&start=0&license=unknown");
			QObject::connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
			QNetworkRequest request(url);
			mNetworkManager->get(request);
		}
	}
}

void ViEchoIdentifier::downloadFinished(QNetworkReply *reply)
{
	mNetworkManager->disconnect();
	mNetworkError = reply->error();
	if(mNetworkError != QNetworkReply::NoError)
	{
		mError = ViEchoIdentifier::NetworkError;
		setState(ViEchoIdentifier::Idle);
		return;
	}
	
	QByteArray data = reply->readAll();
	if(data.startsWith("{\"response\""))
	{
		mResponse.songInfo().changeImagePath(reply->url().toString(), "");
		setState(ViEchoIdentifier::Idle);
		return;
	}

	QString newPath = ViManager::tempPath() + QDir::separator() + "albumart";
	QDir dir(newPath);
	if(!dir.exists())
	{
		dir.mkpath(newPath);
	}
	QFileInfo info(mResponse.songInfo().imagePath());
	newPath += QDir::separator() + mResponse.songInfo().songId() + "." + info.suffix();
	QFile file(newPath);
	if(!file.open(QIODevice::WriteOnly))
	{
		return;
	}
	file.write(data);
	file.close();
	mResponse.songInfo().changeImagePath(reply->url().toString(), newPath);
	setState(ViEchoIdentifier::Idle);
	emit songDetected(ViSongInfo(mResponse.songInfo()));
}

void ViEchoIdentifier::setProxy(QNetworkProxy::ProxyType type, QString host, quint16 port, QString username, QString password)
{
	QNetworkProxy proxy(type, host, port, username, password);
	mNetworkManager->setProxy(proxy);
}

void ViEchoIdentifier::setKey(QString key)
{
	mKey = key;
}

ViEchoIdentifier::State ViEchoIdentifier::state()
{
	return mState;
}

ViEchoIdentifier::Error ViEchoIdentifier::error()
{
	return mError;
}

QNetworkReply::NetworkError ViEchoIdentifier::networkError()
{
	return mNetworkError;
}

QList<ViSongInfo> ViEchoIdentifier::songInfo()
{
	return mResponse.songInfos();
}

void ViEchoIdentifier::setState(ViEchoIdentifier::State state)
{
	mState = state;
	emit stateChanged(mState);
}
