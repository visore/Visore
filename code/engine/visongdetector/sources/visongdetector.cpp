#include "visongdetector.h"
#include "serializer.h"
#include "Codegen.h"

ViSongCodeGeneratorThread::ViSongCodeGeneratorThread(ViAudioBuffer *buffer, QObject *parent)
	: QThread(parent)
{
	mBuffer = buffer;
	mOffset = 0;
}

void ViSongCodeGeneratorThread::setOffset(int offset)
{
	mOffset = offset;
}

void ViSongCodeGeneratorThread::run()
{
	float *data = reinterpret_cast<float*>(mBuffer->data()->data());
	int size = mBuffer->size() / (sizeof(float) / sizeof(char));
	if(data != NULL && size > 0)
	{
		for(int i = 0;i<size;++i){
cout<< mBuffer->data()->data()[i]<<" "<<i/30000<<endl;

data[i] = i/30000.0;}
		Codegen codegen(data, size, mOffset);
		
		//QString code(codegen.getCodeString().c_str());
		cout<<"*"<<codegen.getCodeString()<<"* "<<codegen.getNumCodes()<<endl;
		/*QString version = QString::number(codegen.getVersion());
		emit finished(code, version);*/
	}
}

ViSongDetector::ViSongDetector(ViAudioOutput *output)
	: QObject()
{
	mOutput = output;
	mThread = new ViSongCodeGeneratorThread(mOutput->buffer(), this);
	mNetworkManager = new QNetworkAccessManager();
	mState = ViSongDetector::Idle;
	mResult = ViSongDetector::None;
	mRequestsSent = 0;
	mKeyWasSet = false;
	ViObject::connect(mThread, SIGNAL(finished(QString, QString)), this, SLOT(codeFinished(QString, QString)));
	ViObject::connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	ViObject::connect(mOutput, SIGNAL(positionChanged(ViAudioPosition)), this, SLOT(positionChanged(ViAudioPosition)));
}

ViSongDetector::~ViSongDetector()
{
	ViObject::disconnect(mThread, SIGNAL(finished(QString, QString)), this, SLOT(codeFinished(QString, QString)));
	ViObject::disconnect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	ViObject::disconnect(mOutput, SIGNAL(positionChanged(ViAudioPosition)), this, SLOT(positionChanged(ViAudioPosition)));
	delete mNetworkManager;
	delete mThread;
}

void ViSongDetector::positionChanged(ViAudioPosition position)
{
	
	if(!mKeyWasSet)
	{
		mResult = ViSongDetector::KeyProblem;
	}
	else if(mResult != ViSongDetector::NetworkProblem && mResult != ViSongDetector::Found)
	{
		if((mRequestsSent == 0 && position.milliseconds() >= REQUEST_INTERVAL_1)
			/*|| (mRequestsSent == 1 && position.milliseconds() >= REQUEST_INTERVAL_2)
			|| (mRequestsSent == 2 && position.milliseconds() >= REQUEST_INTERVAL_3)*/)
		{
			++mRequestsSent;
			mThread->start();


/*QString url = "http://developer.echonest.com/api/v4/song/identify?api_key=G1TZBE4IHJAYUSNCN";
	QUrl u(url);
	QNetworkRequest request(u);
	request.setHeader( QNetworkRequest::ContentTypeHeader, QLatin1String( "application/octet-stream" ) );


QVariantMap bob;
bob.insert("code", "eJyNklFywyAMBa-EkMD2cQyY-x-hb6np9KeTJpM3QLwrgUnp-3PqZ0QmnAiiVEVldBAncRE30ULRGQ3iIabC8Bk-w2f4DJ8VAp_hM3yGz_BZI_AZPsNn81ZTiTAiE_gyvlyYVgJfPhnhyzejRuDL-DK-jM_xOT7H504Egc_x-UHgc3yOz_F5JwbxEPgCX-ALfEF_gS_wBb6gv8AXF4Ev5PPRutdiHmPWmYbmw4_R8lM0D-bt0lybnvV5iuuhc45mJYnMrv9LaHHxo6RPvNYX78KvWLzqy_vP-ptPV9l8LpvvV_mL99J3_-PQoqV7tFm6hpHb7j_dmzd_Nh-zvfzwZ_Ne6-bHtflZ60dedwFe--mpLV770Wz1v_azeNXZvOq8vL67vohdX_xb_xf_qX6e567fWFz8M8-f-rqpL6_n3vNXnz_vr_R9_uPI-_2Vbvf8Pn-z2sTn3k_dwvkFmj0Njg==");

QJson::Serializer serializer;
QByteArray json = serializer.serialize(bob);

cout<<"Started now..."<<endl;
QNetworkReply *reply = mNetworkManager->post(request, json);*/
		}
	}
}

void ViSongDetector::codeFinished(QString code, QString version)
{
	cout<<"code: "<<code.toAscii().data()<<"\nversion: "<<version.toAscii().data()<<endl;
}

void ViSongDetector::replyFinished(QNetworkReply *reply)
{
cout<<"*: "<<QString(reply->readAll()).toAscii().data()<<endl;
}

void ViSongDetector::setProxy(QNetworkProxy::ProxyType type, QString host, quint16 port, QString username, QString password)
{
	QNetworkProxy proxy(type, host, port, username, password);
	mNetworkManager->setProxy(proxy);
}

void ViSongDetector::setKey(QString key)
{
	mKeyWasSet = true;
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
