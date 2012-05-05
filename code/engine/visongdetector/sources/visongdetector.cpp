#include "visongdetector.h"
#include "Codegen.h"
#include <QJsonDocument>
#include <QJsonObject>

ViSongCodeGeneratorThread::ViSongCodeGeneratorThread(ViAudioBuffer *buffer, ViAudioFormat format, QObject *parent)
	: QThread(parent)
{
	mBuffer = buffer;
	mFormat  = format;
	mOffset = 0;
}

void ViSongCodeGeneratorThread::setOffset(int offset)
{
	mOffset = offset;
}

void ViSongCodeGeneratorThread::run()
{
	/*ViAudioFormat format;
	format.setSampleRate(22050);
	format.setSampleSize(16);
	format.setByteOrder(ViAudioFormat::LittleEndian);
	format.setChannelCount(1);
	format.setCodec(ViCodecManager::selected("WAV"));

	mCoder.encode(mBuffer, &mOutput, &format);



	//ViAudioBufferStream *stream = newBuffer.createReadStream();
	ViAudioBufferStream *stream = mBuffer->createReadStream();
	int size = stream->bufferSize();
	if(size > 0)
	{
		ViAudioBufferChunk chunk;
		size = stream->read(&chunk, size * mFormat.sampleSize() / 8);
		double *result = new double[size];
		if(mFormat.sampleSize() == 8)
		{
			size = ViPcmConverter::pcmToReal8(chunk.data(), result, size);
		}
		else if(mFormat.sampleSize() == 16)
		{
			size = ViPcmConverter::pcmToReal16(chunk.data(), result, size);
		}
		else if(mFormat.sampleSize() == 32)
		{
			size = ViPcmConverter::pcmToReal32(chunk.data(), result, size);
		}
		float *newData = reinterpret_cast<float*>(result);
		Codegen *codegen = new Codegen(newData, size, mOffset);
		delete [] result;
		QString code = QString::fromStdString(codegen->getCodeString());
		QString version = QString::number(codegen->getVersion());
		emit finished(code, version, codegen->getNumCodes());
		delete codegen;
	}*/
}

ViSongDetector::ViSongDetector(ViAudioOutput *output)
	: QObject()
{
	mAudioOutput = output;
	mThread = new ViSongCodeGeneratorThread(mAudioOutput->buffer(), mAudioOutput->format(), this);
	mNetworkManager = new QNetworkAccessManager();
	mState = ViSongDetector::Idle;
	mResult = ViSongDetector::None;
	mRequestsSent = 0;
	mBufferSize = 0;
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

void ViSongDetector::bufferChanged(int size)
{
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
		{
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

			ViObject::disconnect(mAudioOutput->buffer(), SIGNAL(changed(int)), this, SLOT(bufferChanged(int)));
			//mThread->start();
cout<<"encoding start..."<<endl;
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
		QDataStream stream(&mOutput, QIODevice::ReadOnly);
		int size = mOutput.size();
		if(size > 0)
		{
			ViAudioFormat format = mAudioOutput->format();
			int offset = 0;
			char *chunk = new char[size];
			size = stream.readRawData(chunk, size);
			float *result = new float[size / 2];
			size = ViPcmConverter::pcmToReal16(chunk, result, size);
			delete [] chunk;
			Codegen *codegen = new Codegen(result, size / 2, offset);
			delete [] result;
			QString code = QString::fromStdString(codegen->getCodeString());
			QString version = QString::number(codegen->getVersion());
			codeFinished(code, version, codegen->getNumCodes());
			delete codegen;
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


/*
code = "eJwtk1miZCEIQ7d0FZyWgwP7X8I7qe76SEHUgMH7fb9fOUAVmKAF0F2gdAiWuBBswa0CRU9RdkSKlMoETGlT2hRJqgz2lSlYSqVXQtHWlkON8pS-Rysfq7WwudYfKDVFzpYq5dooVLuirtWhaIqbEljo1VC0BUdw2V2fFpLUigtITfJmArVrTZxEbWjfVKR2Tde3UwWK5IGpZzkFcNaLQG25_wABb9oiUZcRPgVLJ9SkByq-FaFc267b1rI5Vj9xrNdXHuT47txz2DxLJF09gzSqPEhrccu0Wdq8fXtnqXVIBrEOJJJ3av9ar4-OXYN_6vhZN3_1LnqjrPFK-kGyt5yjJPKQFBoidd0Licm5IDE2bD-3zG1BT8nvHXr-FKVljAlJ3Xzr4fyhbsYXZ4usWeqGLKfu43FqxPgiC-sR69Tqg3V8v3MV-TEvt4ds_szvtCirQRYamb25NbyEtDKvQ1L64AMvt9MNz2_fujZRv5EenRba7pBhr7d-6SMmflCfPsKscwTSzrhqGguzZJ9fNuZwhuHM7Xp7eQ9kZa6WgyefetvcP9t-iy_iZt6e1PccPz8im93fPFT__zwo8W8eTJehM--Rk3tzf-bCKDjSIfHf038jDVuQ-HAgS9_4MPSaWyQ-MMu3-i5e80Jidft2Ol_PTsjKcDtk9aOJzz_k7Qfg";
*/



//cout<<code.toAscii().data()<<endl;

	codeMap.insert("code", code);	
	/*
if(code == "eJyNklFywyAMBa-EkMD2cQyY-x-hb6np9KeTJpM3QLwrgUnp-3PqZ0QmnAiiVEVldBAncRE30ULRGQ3iIabC8Bk-w2f4DJ8VAp_hM3yGz_BZI_AZPsNn81ZTiTAiE_gyvlyYVgJfPhnhyzejRuDL-DK-jM_xOT7H504Egc_x-UHgc3yOz_F5JwbxEPgCX-ALfEF_gS_wBb6gv8AXF4Ev5PPRutdiHmPWmYbmw4_R8lM0D-bt0lybnvV5iuuhc45mJYnMrv9LaHHxo6RPvNYX78KvWLzqy_vP-ptPV9l8LpvvV_mL99J3_-PQoqV7tFm6hpHb7j_dmzd_Nh-zvfzwZ_Ne6-bHtflZ60dedwFe--mpLV770Wz1v_azeNXZvOq8vL67vohdX_xb_xf_qX6e567fWFz8M8-f-rqpL6_n3vNXnz_vr_R9_uPI-_2Vbvf8Pn-z2sTn3k_dwvkFmj0Njg==")
cout<<"***************************************************************************************"<<endl;
else
cout<<"NOOOOOOOOOOOOOOOOO: "<<code.toAscii().data()<<endl;*/
cout<<"encoding sent.."<<endl;
	QJsonObject jsonObject;
	jsonObject.insert("code", code);
	mNetworkManager->post(request, QJsonDocument(jsonObject).toJson());
}

void ViSongDetector::replyFinished(QNetworkReply *reply)
{
	ViObject::disconnect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	//mResponse.analyze(reply->readAll());


cout<<QString(reply->readAll()).toAscii().data()<<endl;
bufferChanged(mAudioOutput->buffer()->size());



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
