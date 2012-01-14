#ifndef VISTREAMINPUT_H
#define VISTREAMINPUT_H

#include "viaudioinput.h"
#include <QAudioInput>
#include <QAudioDeviceInfo>

class ViStreamInput : public ViAudioInput
{
	public:
		ViStreamInput(QAudioDeviceInfo deviceInfo = QAudioDeviceInfo::defaultInputDevice());

	private:
		QAudioInput *mAudioInput;
};

#endif
