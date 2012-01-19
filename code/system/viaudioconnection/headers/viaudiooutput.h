#ifndef VIAUDIOOUTPUT_H
#define VIAUDIOOUTPUT_H

#include "viaudiobuffer.h"

class ViAudioOutput : public QObject, public ViError
{
    Q_OBJECT

	private slots:
		virtual void updateOutputBuffer(int startIndex, int size) = 0;

	public:
		ViAudioOutput()
			: QObject()
		{
		}
		virtual void setBuffer(ViAudioBuffer *buffer)
		{
			mBuffer = buffer;
		}
		virtual void start() = 0;
		
	protected:
		ViAudioBuffer *mBuffer;
};

#endif
