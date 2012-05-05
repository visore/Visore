#include "visongdetector.h"
#include "Codegen.h"
#include <QJsonDocument>
#include <QJsonObject>

//Interval (milliseconds) use to request info
#define REQUEST_SAMPLES_1 200000
#define REQUEST_SAMPLES_2 500000
#define REQUEST_SAMPLES_3 1000000

ViSongCodeGeneratorThread::ViSongCodeGeneratorThread(QByteArray *data, QObject *parent)
	: QThread(parent)
{
	mData = data;
	mOffset = 0;
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
		size = ViPcmConverter::pcmToReal16(chunk, result, size);
		delete [] chunk;
		Codegen *codegen = new Codegen(result, size / 2, mOffset);
		delete [] result;
		QString code = QString::fromStdString(codegen->getCodeString());
		QString version = QString::number(codegen->getVersion());
		finished(code, version, codegen->getNumCodes());
		delete codegen;
	}
}

ViSongDetector::ViSongDetector(ViAudioOutput *output)
	: QObject()
{
	mAudioOutput = output;
	mThread = new ViSongCodeGeneratorThread(&mOutput, this);
	mNetworkManager = new QNetworkAccessManager();

	mState = ViSongDetector::Idle;
	mError = ViSongDetector::NoError;
	mNetworkError = QNetworkReply::NoError;
	mFound = false;

	mRequestsSent = 0;
	mKey = "";
	ViObject::connect(mThread, SIGNAL(finished(QString, QString, int)), this, SLOT(codeFinished(QString, QString, int)));
	ViObject::connect(mAudioOutput->buffer(), SIGNAL(changed(int)), this, SLOT(bufferChanged(int)));
	ViObject::connect(&mCoder, SIGNAL(stateChanged(ViCoder::State)), this, SLOT(encodingStateChanged(ViCoder::State)));
}

ViSongDetector::~ViSongDetector()
{
	ViObject::disconnect(mThread, SIGNAL(finished(QString, QString, int)), this, SLOT(codeFinished(QString, QString, int)));
	ViObject::disconnect(mAudioOutput->buffer(), SIGNAL(changed(int)), this, SLOT(bufferChanged(int)));
	delete mThread;
	delete mNetworkManager;
}

void ViSongDetector::reset()
{
	mNetworkError = QNetworkReply::NoError;
	mError = ViSongDetector::NoError;
	mFound = false;
	mRequestsSent = 0;
	mResponse.reset();
}

void ViSongDetector::bufferChanged(int size)
{
	if(mKey == "")
	{
		mError = ViSongDetector::KeyError;
		return;
	}

	qint64 bufferSize = mAudioOutput->buffer()->size();
	if(mError != ViSongDetector::NetworkError && !mFound && !mThread->isRunning())
	{
		if((mRequestsSent == 0 && bufferSize >= REQUEST_SAMPLES_1)
			|| (mRequestsSent == 1 && bufferSize >= REQUEST_SAMPLES_2)
			|| (mRequestsSent == 2 && bufferSize >= REQUEST_SAMPLES_3))
		{
			if(bufferSize >= REQUEST_SAMPLES_3)
			{
				mRequestsSent = 3;
			}
			else if(bufferSize >= REQUEST_SAMPLES_2)
			{
				mRequestsSent = 2;
			}
			else if(bufferSize >= REQUEST_SAMPLES_1)
			{
				mRequestsSent = 1;
			}
			setState(ViSongDetector::Encoding);
			ViObject::disconnect(mAudioOutput->buffer(), SIGNAL(changed(int)), this, SLOT(bufferChanged(int)));
			ViAudioFormat format;
			format.setSampleRate(22050);
			format.setSampleSize(16);
			format.setByteOrder(ViAudioFormat::LittleEndian);
			format.setChannelCount(1);
			format.setCodec(ViCodecManager::selected("WAV"));
			mCoder.encode(mAudioOutput->buffer(), &mOutput, mAudioOutput->buffer()->format(), format);
		}
	}
}

void ViSongDetector::encodingStateChanged(ViCoder::State state)
{
	if(state == ViCoder::SuccessState)
	{
		setState(ViSongDetector::CodeGeneration);
		mThread->start();
	}
}

void ViSongDetector::codeFinished(QString code, QString version, int codeLength)
{
	setState(ViSongDetector::SongDetection);
	mOutput.clear();
	ViObject::connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	QUrl url("http://developer.echonest.com/api/v4/song/identify?api_key=" + mKey + "&bucket=id:7digital-US&bucket=tracks&bucket=audio_summary&bucket=artist_familiarity&bucket=artist_hotttnesss&bucket=song_hotttnesss");
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/octet-stream"));
	QJsonObject jsonObject;
	jsonObject.insert("code", code);
	mNetworkManager->post(request, QJsonDocument(jsonObject).toJson());
}

void ViSongDetector::replyFinished(QNetworkReply *reply)
{
	ViObject::disconnect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
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
		bufferChanged();
	}
	else
	{
		setState(ViSongDetector::ImageRetrieval);
		mFound = true;
		QUrl url;
		if(mResponse.songInfo().imagePath() != "")
		{
			
			ViObject::connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));
			url = QUrl(mResponse.songInfo().imagePath());
		}
		else
		{
			url = QUrl("http://developer.echonest.com/api/v4/artist/images?api_key=" + mKey + "&id=" + mResponse.songInfo().artistId() + "&format=json&results=1&start=0&license=unknown");
			ViObject::connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));	
		}
		QNetworkRequest request(url);
		mNetworkManager->get(request);
	}
}

void ViSongDetector::downloadFinished(QNetworkReply *reply)
{
	ViObject::disconnect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));
	mNetworkError = reply->error();
	if(mNetworkError != QNetworkReply::NoError)
	{
		mError = ViSongDetector::NetworkError;
	}

	QString newPath = "/tmp/" + mResponse.songInfo().songId();
	QFile file(newPath);
	if(!file.open(QIODevice::WriteOnly))
	{
		return;
	}
	file.write(reply->readAll());
	file.close();
	mResponse.songInfo().changeImagePath(reply->url().toString(), newPath);
	setState(ViSongDetector::Idle);
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
	emit stateChanged(mState, mFound);
}
