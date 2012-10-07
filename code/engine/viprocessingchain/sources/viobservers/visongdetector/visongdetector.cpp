#include "visongdetector.h"
#include "vimanager.h"
#include "Codegen.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>

//Interval (milliseconds) use to request info
#define REQUEST_SAMPLES_1 10000
#define REQUEST_SAMPLES_2 15000
#define REQUEST_SAMPLES_3 20000
#define REQUEST_SAMPLES_4 35000
#define REQUEST_SAMPLES_5 90000

ViSongCodeGeneratorThread::ViSongCodeGeneratorThread(QObject *parent)
	: QThread(parent)
{
	mOffset = 0;
}

void ViSongCodeGeneratorThread::setData(QByteArray *data)
{
	mData = data;
}

void ViSongCodeGeneratorThread::setOffset(int offset)
{
	mOffset = offset;
}

void ViSongCodeGeneratorThread::run()
{
	QDataStream stream(mData, QIODevice::ReadOnly);
	int size = mData->size();
	if(size > 0)
	{
		char *chunk = new char[size];
		size = stream.readRawData(chunk, size);
		float *result = new float[size / 2];
		size = ViPcmConverter<float>::pcmToReal16(chunk, result, size);
		delete [] chunk;
		Codegen codegen(result, size / 2, mOffset);
		delete [] result;
		QString code = QString::fromStdString(codegen.getCodeString());
		QString version = QString::number(codegen.getVersion());
		emit finished(code, version, codegen.getNumCodes());
	}
	mData->clear();
}

ViSongDetector::ViSongDetector()
	: ViObserver()
{
	mNetworkManager = new QNetworkAccessManager();

	mState = ViSongDetector::Idle;
	mError = ViSongDetector::NoError;
	mNetworkError = QNetworkReply::NoError;
	mFound = false;

	mIsEnabled = false;

	mRequestsSent = 0;
	mKey = "";
	QObject::connect(&mThread, SIGNAL(finished(QString, QString, int)), this, SLOT(codeFinished(QString, QString, int)));
	QObject::connect(&mCoder, SIGNAL(finished()), this, SLOT(encodingFinished()));
}

ViSongDetector::~ViSongDetector()
{
	QObject::disconnect(&mThread, SIGNAL(finished(QString, QString, int)), this, SLOT(codeFinished(QString, QString, int)));
	delete mNetworkManager;
}

void ViSongDetector::initialize()
{
	mNetworkError = QNetworkReply::NoError;
	mError = ViSongDetector::NoError;
	mFound = false;
	mRequestSent = false;
	mRequestsSent = 0;
	mResponse.reset();
}

void ViSongDetector::finalize()
{
	mCoder.stop();
}

void ViSongDetector::run()
{
	mIsEnabledMutex.lock();
	bool isEnabled = mIsEnabled;
	mIsEnabledMutex.unlock();
	if(isEnabled && mError != ViSongDetector::NetworkError && !mFound)
	{
		if(mKey == "")
		{
			mError = ViSongDetector::KeyError;
			return;
		}
		qint64 bufferLength = ViAudioPosition::convertPosition(mBuffer->size(), ViAudioPosition::Bytes, ViAudioPosition::Milliseconds, mBuffer->format());
		if(!mRequestSent && (mRequestsSent == 0 && bufferLength >= REQUEST_SAMPLES_1)
			|| (mRequestsSent == 1 && bufferLength >= REQUEST_SAMPLES_2)
			|| (mRequestsSent == 2 && bufferLength >= REQUEST_SAMPLES_3)
			|| (mRequestsSent == 3 && bufferLength >= REQUEST_SAMPLES_4)
			|| (mRequestsSent == 4 && bufferLength >= REQUEST_SAMPLES_5))
		{
			mRequestSent = true;
			if(bufferLength >= REQUEST_SAMPLES_5)
			{
				mRequestsSent = 5;
			}
			else if(bufferLength >= REQUEST_SAMPLES_4)
			{
				mRequestsSent = 4;
			}
			else if(bufferLength >= REQUEST_SAMPLES_3)
			{
				mRequestsSent = 3;
			}
			else if(bufferLength >= REQUEST_SAMPLES_2)
			{
				mRequestsSent = 2;
			}
			else if(bufferLength >= REQUEST_SAMPLES_1)
			{
				mRequestsSent = 1;
			}
			setState(ViSongDetector::Encoding);
			ViAudioFormat format;
			format.setSampleRate(22050);
			format.setSampleSize(16);
			format.setSampleType(ViAudioFormat::SignedInt);
			format.setByteOrder(ViAudioFormat::LittleEndian);
			format.setChannelCount(1);
			format.setCodec(ViAudioManager::codec("WAVE"));
			mOutput.clear();
			mCoder.encode(mBuffer, mOutput, format);
		}
	}
}

void ViSongDetector::encodingFinished()
{
	if(mCoder.error() == ViCoder::NoError)
	{
		setState(ViSongDetector::CodeGeneration);
		mThread.setData(&mOutput);
		mThread.start();
	}
	else
	{
		mOutput.clear();
	}
}

void ViSongDetector::codeFinished(QString code, QString version, int codeLength)
{
	setState(ViSongDetector::SongDetection);
	QObject::connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	QUrl url("http://developer.echonest.com/api/v4/song/identify?api_key=" + mKey + "&bucket=id:7digital-US&bucket=tracks&bucket=audio_summary&bucket=artist_familiarity&bucket=artist_hotttnesss&bucket=song_hotttnesss");
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/octet-stream"));
	QJsonObject jsonObject;
	jsonObject.insert("code", code);
	mNetworkManager->post(request, QJsonDocument(jsonObject).toJson());
}

void ViSongDetector::replyFinished(QNetworkReply *reply)
{
	mRequestSent = false;
	mNetworkError = reply->error();
	if(mNetworkError != QNetworkReply::NoError)
	{
		mError = ViSongDetector::NetworkError;
		setState(ViSongDetector::Idle);
		return;
	}

	mResponse.analyze(reply->readAll());
	if(mResponse.message().contains("api") && mResponse.message().contains("key"))
	{
		mError = ViSongDetector::KeyError;
		setState(ViSongDetector::Idle);
		return;
	}
	else if(mResponse.numberOfSongs() == 0)
	{
		run();
	}
	else
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
				setState(ViSongDetector::Idle);
			}
			else
			{
				setState(ViSongDetector::ImageRetrieval);
				QObject::connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));
				QUrl url(mResponse.songInfo().imagePath());
				QNetworkRequest request(url);
				mNetworkManager->get(request);
			}
			LOG("Song info detected (" + mResponse.songInfo().artistName() + " - " + mResponse.songInfo().songTitle() + ").");
			emit songDetected(mResponse.songInfo());
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

void ViSongDetector::downloadFinished(QNetworkReply *reply)
{
	mNetworkManager->disconnect();
	mNetworkError = reply->error();
	if(mNetworkError != QNetworkReply::NoError)
	{
		mError = ViSongDetector::NetworkError;
		setState(ViSongDetector::Idle);
		return;
	}
	
	QByteArray data = reply->readAll();
	if(data.startsWith("{\"response\""))
	{
		mResponse.songInfo().changeImagePath(reply->url().toString(), "");
		setState(ViSongDetector::Idle);
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
	setState(ViSongDetector::Idle);
	emit songDetected(ViSongInfo(mResponse.songInfo()));
}

void ViSongDetector::setProxy(QNetworkProxy::ProxyType type, QString host, quint16 port, QString username, QString password)
{
	QNetworkProxy proxy(type, host, port, username, password);
	mNetworkManager->setProxy(proxy);
}

void ViSongDetector::setKey(QString key)
{
	mKey = key;
}

ViSongDetector::State ViSongDetector::state()
{
	return mState;
}

ViSongDetector::Error ViSongDetector::error()
{
	return mError;
}

QNetworkReply::NetworkError ViSongDetector::networkError()
{
	return mNetworkError;
}

QList<ViSongInfo> ViSongDetector::songInfo()
{
	return mResponse.songInfos();
}

void ViSongDetector::setState(ViSongDetector::State state)
{
	mState = state;
	emit stateChanged(mState);
}
