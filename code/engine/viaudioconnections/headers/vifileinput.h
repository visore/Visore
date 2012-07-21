#ifndef VIFILEINPUT_H
#define VIFILEINPUT_H

#include "viaudioinput.h"
#include "viaudiocoder.h"

class ViFileInput : public ViAudioInput
{
    Q_OBJECT

	public:

		ViFileInput();

		void setFile(QString filePath);
		ViAudioFormat format();

		void start();
		void stop();
		void pause();

	protected:

		QString mFilePath;
		ViAudioCoder mCoder;
};

#endif
