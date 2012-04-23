#ifndef VIFILEINPUT_H
#define VIFILEINPUT_H

#include "viaudioinput.h"

class ViFileInput : public ViAudioInput
{
    Q_OBJECT

	public:
		ViFileInput(ViAudioFormat format, ViAudioBuffer *buffer = NULL, QString filePath = "");
		virtual void setFilePath(QString filePath);

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;
		virtual void initialize() = 0;

	protected:
		virtual void free() = 0;

	protected:
		QString mFilePath;
};

#endif
