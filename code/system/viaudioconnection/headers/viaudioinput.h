#ifndef VIAUDIOINPUT_H
#define VIAUDIOINPUT_H

#include "viaudiobuffer.h"

class ViAudioInput : public QObject, public ViError
{
    Q_OBJECT

	signals:
		void changed(int startIndex, int size);

	public:
		ViAudioInput()
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
