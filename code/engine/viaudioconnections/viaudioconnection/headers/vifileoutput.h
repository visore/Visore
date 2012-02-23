#ifndef VIFILEOUTPUT_H
#define VIFILEOUTPUT_H

#include "viaudiooutput.h"
#include "viaudiometadata.h"
#include "viencodermanager.h"

class ViFileOutput : public ViAudioOutput
{
    Q_OBJECT

	public slots:
		virtual void bufferChanged(int size);

	public:
		ViFileOutput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, QString filePath = "");
		virtual void setFilePath(QString filePath);

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;
		virtual void initialize() = 0;

		bool setPosition(ViAudioPosition position);
		ViAudioPosition position();

	protected:
		virtual void free() = 0;

	protected:
		QString mFilePath;
};

#endif
