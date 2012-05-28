#ifndef VIFILEINPUT_H
#define VIFILEINPUT_H

#include "viaudioinput.h"
#include "vicoder.h"

class ViFileInput : public ViAudioInput
{
    Q_OBJECT

	public:

		ViFileInput();

		void setFile(QString filePath);

		void start();
		void stop();
		void pause();

	protected:

		QString mFilePath;
		ViCoder mCoder;
};

#endif
