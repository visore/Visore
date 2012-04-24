#ifndef VIFILEINPUT_H
#define VIFILEINPUT_H

#include "viaudioinput.h"

class ViFileInput : public ViAudioInput
{
    Q_OBJECT

	public:

		ViFileInput();
		
		virtual void initialize();
		virtual void free();

		virtual void setFile(QString filePath);

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;

	protected:

		QString mFilePath;
};

#endif
