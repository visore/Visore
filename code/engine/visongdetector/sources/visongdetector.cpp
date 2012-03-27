#include "visongdetector.h"
#include "serializer.h"
#include "Codegen.h"

ViSongCodeGeneratorThread::ViSongCodeGeneratorThread(ViAudioBuffer *buffer, ViAudioMetaData *metaData, QObject *parent)
	: QThread(parent)
{
	mBuffer = buffer;
	mMetaData = metaData;
	mOffset = 0;
}

void ViSongCodeGeneratorThread::setOffset(int offset)
{
	mOffset = offset;
}

void ViSongCodeGeneratorThread::run()
{
	ViAudioBufferStream *stream = mBuffer->createReadStream();
	int size = stream->bufferSize();
	if(size > 0)
	{
		ViAudioBufferChunk chunk;
		size = stream->read(&chunk, size * mMetaData->bitDepth() / 8);
		double *result = new double[size];
		if(mMetaData->bitDepth() == 8)
		{
			size = ViPcmConverter::pcmToReal8(chunk.data(), result, size);
		}
		else if(mMetaData->bitDepth() == 16)
		{
			size = ViPcmConverter::pcmToReal16(chunk.data(), result, size);
		}
		else if(mMetaData->bitDepth() == 32)
		{
			size = ViPcmConverter::pcmToReal32(chunk.data(), result, size);
		}
		float *newData = reinterpret_cast<float*>(result);
	cout<<"*5: "<<size<<endl;
		Codegen *codegen = new Codegen(newData, size, mOffset);cout<<"*6"<<endl;
		delete [] result;
		QString code = QString::fromStdString(codegen->getCodeString());
	cout<<"*6.2: "<<size<<endl;
		QString version = QString::number(codegen->getVersion());
		emit finished(code, version, codegen->getNumCodes());
		delete codegen;
	}
}

ViSongDetector::ViSongDetector(ViAudioOutput *output)
	: QObject()
{
	mOutput = output;
	mThread = new ViSongCodeGeneratorThread(mOutput->buffer(), output->metaData(), this);
	mNetworkManager = new QNetworkAccessManager();
	mState = ViSongDetector::Idle;
	mResult = ViSongDetector::None;
	mRequestsSent = 0;
	mBufferSize = 0;
	mKey = "";
	ViObject::connect(mThread, SIGNAL(finished(QString, QString, int)), this, SLOT(codeFinished(QString, QString, int)));
	ViObject::connect(mOutput->buffer(), SIGNAL(changed(int)), this, SLOT(bufferChanged(int)));
}

ViSongDetector::~ViSongDetector()
{
	ViObject::disconnect(mThread, SIGNAL(finished(QString, QString, int)), this, SLOT(codeFinished(QString, QString, int)));
	ViObject::disconnect(mOutput->buffer(), SIGNAL(changed(int)), this, SLOT(bufferChanged(int)));
	delete mThread;
	delete mNetworkManager;
}

void ViSongDetector::bufferChanged(int size)
{
	ViObject::disconnect(mOutput->buffer(), SIGNAL(changed(int)), this, SLOT(bufferChanged(int)));
	mBufferSize += size;
	if(mKey == "")
	{
		mResult = ViSongDetector::KeyProblem;
	}
	else if(mResult != ViSongDetector::NetworkProblem && mResult != ViSongDetector::Found && !mThread->isRunning())
	{
		if((mRequestsSent == 0 && mBufferSize >= REQUEST_SAMPLES_1)
			|| (mRequestsSent == 1 && mBufferSize >= REQUEST_SAMPLES_2)
			|| (mRequestsSent == 2 && mBufferSize >= REQUEST_SAMPLES_3))
		{cout<<"buff: "<<mBufferSize<<endl;
			if(mBufferSize >= REQUEST_SAMPLES_3)
			{
				mRequestsSent = 3;
			}
			else if(mBufferSize >= REQUEST_SAMPLES_2)
			{
				mRequestsSent = 2;
			}
			else if(mBufferSize >= REQUEST_SAMPLES_1)
			{
				mRequestsSent = 1;
			}
			mThread->start();
		}
	}
}

void ViSongDetector::codeFinished(QString code, QString version, int codeLength)
{
	ViObject::connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

	QUrl url("http://developer.echonest.com/api/v4/song/identify?api_key=" + mKey + "&bucket=id:7digital-US&bucket=tracks&bucket=audio_summary&bucket=artist_familiarity&bucket=artist_hotttnesss&bucket=song_hotttnesss");

	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/octet-stream"));
cout<<"version: "<<version.toAscii().data()<<endl;
	QVariantMap codeMap;
	codeMap.insert("code", code);	
	
if(code == "eJw9lImx5CAMRFMyEggIhzP_EP5rpnanamS5betotfi-36_xT3JMJssWeSFT9bTJDN3OZ3S75G2ZI3OfaYrEfVKQ5LrN8nKW0QMFTVVel5mESUtmy9ylKkJGXpJxEluWKTLVZOQ1eZ0cpqpMVdnWt6rFLp4rlCunGxW4P8O3roK86EGQw5tuO5_50O0ksq9n9PKWd_SyIvsFy48qcZLTI43k2eU5AbL4y2EydJlFYm56oKbzzDLyFD5vGZWb1X5R5-UNweQpXskyileCl0slQGl8IUYwhC9Tt0ueKi2H8EVBi4KGCg5xECoyFDTEQRR5IU9EhCKHiI1O6BjyxK4efrEJr4Yweio2QuFrAqzGy9XlidiqyFXfVbVfFa9OGdFZ1XQ98hSq3iwd8nJLvNyMAM2fOH3Xnb_lSOJCBs4CLOPiONcdcy2LyxU25tfnLcMim0AftXfAdPbphzaDGuqon53UALmi5kB4UrUdr19dgFZiL0C8XmZFz322QVsj8ohj5tV6t07-ttcIW3PH8Oq9TyM_1wOh8xvrBCU4nE5Ab2p8rNT8pBvt2yS_rXpbHoAo8Tig5_iOQPoDbqk0-suRfJcAdAZ6S2mE3A3w5avxmc1fvrzauL98Dph6JV9ehIxCPhbgOGBClFt5TkYYjozP7YC2zvB5st87fVDT5XfWYc7yLjtQGyB5L8SyberzqrUp0Hs2iFhp2SmANuB89gw-Rl9muZ4CSKs9XfC2HXlWK9SV6X-kXgAThbQo2QsLA-ip7QxI6gUPnw3VkyB3D9WTYo_LilACCzG3DfaWWVLHaPBBKfUbw988AH395kEd980DibEcFR34zpL75Vy6xbr2Hh3EHSj1Mo8yT__ePAA5O_7N4-T_8-gcVPXNgxR9ax7H4aUy73rbenmZC6PgkwCE_3zJa1TpHRAeFmCKCQ9VhxfKhwdmeXrMlO1uQKgu37zZSpsX0BhuAFKHJt6M1yM8IYXu_upp43CqMJoJqNQS69PJr66fThhFAD5evjeXx0sK5pIfL0VzgY-RAa1djin6Zk2ghpEM1qT9dMLupHyfTmzn4vDBCGYBpF86Zs_Rg0Tz0wP9jvT0YJN8tW2rweIPjtzZ95hLx7qdGuRfjGjyEfth-Q8ZwLdq")
cout<<"***************************************************************************************"<<endl;
else
cout<<"NOOOOOOOOOOOOOOOOO"<<endl;


	QJson::Serializer serializer;
	QByteArray json = serializer.serialize(codeMap);
	mNetworkManager->post(request, json);
}

void ViSongDetector::replyFinished(QNetworkReply *reply)
{
	ViObject::disconnect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	mResponse.analyze(reply->readAll());
	cout<<mResponse.toString().toAscii().data()<<endl;
	if(mResponse.numberOfSongs() == 0)
	{
		bufferChanged(0);
	}
	else if(mResponse.songInfo().imagePath() != "")
	{
		ViObject::connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));
		QNetworkRequest request(QUrl(mResponse.songInfo().imagePath()));
		mNetworkManager->get(request);
	}
}

void ViSongDetector::downloadFinished(QNetworkReply *reply)
{
	ViObject::disconnect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));
	QString newPath = "/tmp/" + mResponse.songInfo().songId();
	QFile file(newPath);
	if(!file.open(QIODevice::WriteOnly))
	{
		return;
	}
	file.write(reply->readAll());
	file.close();
	mResponse.songInfo().setImagePath(newPath);
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

ViSongDetector::Result ViSongDetector::result()
{
	return mResult;
}

ViSongInfo ViSongDetector::songInfo()
{
	return mSongInfo;
}
