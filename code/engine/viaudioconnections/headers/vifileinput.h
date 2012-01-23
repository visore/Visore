#ifndef VIFILEINPUT_H
#define VIFILEINPUT_H

#include "viaudioinput.h"
#include "viaudiometadata.h"

class ViFileInput : public ViAudioInput
{
    Q_OBJECT

	public:
		ViFileInput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, QString filePath = "")
			: ViAudioInput(buffer, metaData)
		{
			mFilePath = filePath;
		}

		virtual void setFilePath(QString filePath)
		{
			mFilePath = filePath;
		}

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;

	protected:
		QString mFilePath;
};

#endif
