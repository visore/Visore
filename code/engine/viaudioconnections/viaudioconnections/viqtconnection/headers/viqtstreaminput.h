#ifndef VIQTSTREAMINPUT_H
#define VIQTSTREAMINPUT_H

#include "vistreaminput.h"
#include <QAudioInput>
#include <QBuffer>

class ViQtStreamBuffer : public QBuffer
{
	public:
		ViQtStreamBuffer(ViAudioBuffer *buffer);
		qint64 write(const char *data, qint64 maxSize){cout<<"pol: 1"<<endl;}
		qint64 write(const char *data){cout<<"pol: 2"<<endl;}
		qint64 write(const QByteArray &byteArray){cout<<"pol: 3"<<endl;}
		qint64 writeData ( const char * data, qint64 len ){cout<<"pol: 8"<<endl;}
};

class ViQtStreamInput : public ViStreamInput
{
	Q_OBJECT

private slots:
void a();

	public:

		ViQtStreamInput();
		~ViQtStreamInput();
		void initialize();
		void free();

		void start();
		void stop();
		void pause();

		qreal volume();
		void setVolume(qreal volumeValue);
		void mute(bool value);
		
	private:

		QAudioInput *mAudioInput;
		QBuffer *mBufferDevice;
		ViAudioPosition mOldPosition;

		qreal mMuteVolume;
		bool mIsMute;

};

#endif
