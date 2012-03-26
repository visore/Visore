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
	mState = ViSongDetector::Idle;
	mResult = ViSongDetector::None;
	mRequestsSent = 0;
	mBufferSize = 0;
	mKey = "";
	createNetworkManagers();
	ViObject::connect(mThread, SIGNAL(finished(QString, QString, int)), this, SLOT(codeFinished(QString, QString, int)));
	ViObject::connect(mOutput->buffer(), SIGNAL(changed(int)), this, SLOT(bufferChanged(int)));
}

ViSongDetector::~ViSongDetector()
{
	ViObject::disconnect(mThread, SIGNAL(finished(QString, QString, int)), this, SLOT(codeFinished(QString, QString, int)));
	ViObject::disconnect(mOutput->buffer(), SIGNAL(changed(int)), this, SLOT(bufferChanged(int)));
	delete mThread;
	mNetworkManagers.clear();
}

void ViSongDetector::createNetworkManagers()
{
	mNetworkManagers.insert("SongIdentify", new QNetworkAccessManager());
	mNetworkManagers.insert("SongSearch", new QNetworkAccessManager());

	ViObject::connect(mNetworkManagers["SongIdentify"], SIGNAL(finished(QNetworkReply*)), this, SLOT(songIdentifyFinished(QNetworkReply*)));
	ViObject::connect(mNetworkManagers["SongSearch"], SIGNAL(finished(QNetworkReply*)), this, SLOT(songSearchFinished(QNetworkReply*)));
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
	QUrl url("http://developer.echonest.com/api/v4/song/identify?api_key=" + mKey);
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/octet-stream"));
	QVariantMap codeMap;
	codeMap.insert("code", "eJyNklFywyAMBa-EkMD2cQyY-x-hb6np9KeTJpM3QLwrgUnp-3PqZ0QmnAiiVEVldBAncRE30ULRGQ3iIabC8Bk-w2f4DJ8VAp_hM3yGz_BZI_AZPsNn81ZTiTAiE_gyvlyYVgJfPhnhyzejRuDL-DK-jM_xOT7H504Egc_x-UHgc3yOz_F5JwbxEPgCX-ALfEF_gS_wBb6gv8AXF4Ev5PPRutdiHmPWmYbmw4_R8lM0D-bt0lybnvV5iuuhc45mJYnMrv9LaHHxo6RPvNYX78KvWLzqy_vP-ptPV9l8LpvvV_mL99J3_-PQoqV7tFm6hpHb7j_dmzd_Nh-zvfzwZ_Ne6-bHtflZ60dedwFe--mpLV770Wz1v_azeNXZvOq8vL67vohdX_xb_xf_qX6e567fWFz8M8-f-rqpL6_n3vNXnz_vr_R9_uPI-_2Vbvf8Pn-z2sTn3k_dwvkFmj0Njg==");	
	QJson::Serializer serializer;
	QByteArray json = serializer.serialize(codeMap);
	mNetworkManagers["SongIdentify"]->post(request, json);
}

void ViSongDetector::songIdentifyFinished(QNetworkReply *reply)
{
	mResponse.analyzeSongIdentify(reply->readAll());
	if(mResponse.numberOfSongs() == 0)
	{
		bufferChanged(0);
	}
	else
	{
		QNetworkRequest request1(QUrl("http://developer.echonest.com/api/v4/song/search?api_key=" + mKey + "&format=json&results=1&artist_id=" + mResponse.songInfo().artistId() + "&title=" + ViEchoNestResponse::toUrl(mResponse.songInfo().songTitle()) + "&bucket=id:musicbrainz&bucket=audio_summary&bucket=artist_familiarity&bucket=artist_hotttnesss&bucket=song_hotttnesss&bucket=tracks"));
		mNetworkManagers["SongSearch"]->get(request1);
	}
}

void ViSongDetector::songSearchFinished(QNetworkReply *reply)
{
	cout<<"--a*: "<<QString(reply->readAll()).toAscii().data()<<endl;
	mResponse.analyzeSongSearch(reply->readAll());
	cout<<"a*: "<<mResponse.toString().toAscii().data()<<endl;
}

void ViSongDetector::setProxy(QNetworkProxy::ProxyType type, QString host, quint16 port, QString username, QString password)
{
	QNetworkProxy proxy(type, host, port, username, password);
	QMapIterator<QString, QNetworkAccessManager*> iterator(mNetworkManagers);
	while(iterator.hasNext())
	{
		iterator.next();
		iterator.value()->setProxy(proxy);
	}
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
