#ifndef VIFILEINPUT_H
#define VIFILEINPUT_H

#include "viaudioinput.h"

class ViFileInput : public ViAudioInput
{
    Q_OBJECT

	public:
		ViFileInput()
			: ViAudioInput()
		{
		}
		virtual void setFilePath(QString filePath)
		{
			mFilePath = filePath;
		}
		virtual void start() = 0;

	protected:
		QString mFilePath;
};

#endif
