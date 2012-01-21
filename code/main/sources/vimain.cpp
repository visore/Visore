#include "viessentials.h"


#include <QApplication>

#include <QUrl>


#include <QAudioCaptureSource>
#include <QMediaRecorder>
#include <QAudioEncoderSettings>

#include <QMainWindow>


//#include "vibassconnection.h"
#include "viaudioengine.h"

void run();

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);


/*
ViAudioConnection *mAudioConnection;
ViLibrary<ViAudioConnection> *mAudioConnectionLoader;
ViAudioBuffer *mBuffer = new ViAudioBuffer();
ViAudioInput *mAudioInput;

mAudioConnection = new ViBassConnection();
ViAudioMetaData *metaData = new ViAudioMetaData();
//ViAudioDevice inputDevice = mAudioConnection->defaultDevice(ViAudioDevice::Input);
ViBassStreamInput *input = new ViBassStreamInput(mBuffer, metaData, NULL);
input->start();*/














/*
// Audio only recording
  QAudioCaptureSource *audioSource = new QAudioCaptureSource();

QMediaRecorder *recorder = new QMediaRecorder(audioSource);
 bool bb= recorder->setOutputLocation(QUrl("/home/visore/Desktop/meme.raw"));



ViLogger::debug("*+++++++++++: "+recorder->containerMimeType ());
 recorder->record();
ViLogger::debug("*+++++++++++: "+recorder->errorString ());
ViLogger::debug("*+++++++++++: "+QString::number(recorder->state()));



 


QList<QString> a = recorder->supportedAudioCodecs();



for(int i = 0; i < a.length(); ++i)
{
ViLogger::debug("*: "+a[i]);
}*/


   /* QMediaServiceProvider *provider = QMediaServiceProvider::defaultServiceProvider();
    QMediaService * service = provider->requestService(Q_MEDIASERVICE_AUDIOENDPOINTSELECTOR);
    QAudioEndpointSelector* control = qobject_cast<QAudioEndpointSelector*> (service->requestControl(QMediaPlayerControl_iid));*/

//ViLogger::debug(QMediaServiceProvider::defaultServiceProvider()->deviceDescription());




ViAudioEngine *e = new ViAudioEngine();
	//ViLibrary<ViAudioEngine> interface("/home/visore/Visore/install/system/libviaudioengine.so");
	//ViLibrary<QMainWindow> interface("/home/visore/Visore/install/interface/libvimainwindow.so");
	//if(!interface.open()) ViLogger::debug("Library cannot be loaded!");
	//ViAudioEngine *engine = interface.create();
	//QMainWindow *window = interface.create();
	//window->show();
	//delete window;
//ViAudioEngine *e = new ViAudioEngine();

	return application.exec();
}

