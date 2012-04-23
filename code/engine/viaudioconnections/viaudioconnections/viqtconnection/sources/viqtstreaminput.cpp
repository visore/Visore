#include "viqtstreaminput.h"

ViQtStreamInput::ViQtStreamInput(ViAudioFormat format, ViAudioBuffer *buffer, ViAudioDevice *device)
	: ViStreamInput(format, buffer, device)
{
	 /*recorder = new QAudioRecorder;

 QAudioEncoderSettings audioSettings;
 audioSettings.setCodec("audio/amr");
 audioSettings.setQuality(QtMultimedia::HighQuality);

 recorder->setEncodingSettings(audioSettings);

 recorder->setOutputLocation(QUrl::fromLocalFile("test.amr"));
 recorder->record();*/

QAudioDeviceInfo t(QAudioDeviceInfo::defaultInputDevice());
QList<int> 	 s = t.supportedSampleRates();
cout<<"********************************************: "<<	QAudioDeviceInfo::availableDevices(QAudio::AudioInput).size()<<"  *"<<t.deviceName().toAscii().data()<<"*"<<endl;
for(int i = 0;i < s.size(); i++)
{
cout<<"codec: "<<s[i]<<endl;
}
}

ViQtStreamInput::~ViQtStreamInput()
{

}

void ViQtStreamInput::initialize()
{

}

void ViQtStreamInput::free()
{

}

void ViQtStreamInput::start()
{

}

void ViQtStreamInput::stop()
{

}

void ViQtStreamInput::pause()
{

}
