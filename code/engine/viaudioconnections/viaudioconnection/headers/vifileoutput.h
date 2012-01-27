#ifndef VIFILEOUTPUT_H
#define VIFILEOUTPUT_H

#include "viaudiooutput.h"
#include "viaudiometadata.h"
#include "viencodermanager.h"

class ViFileOutput : public ViAudioOutput
{
    Q_OBJECT

	public:
		ViFileOutput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, QString filePath = "");
		virtual void setFilePath(QString filePath);

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;

		qint64 setPosition(ViAudioTransmission::ViTransmissionType type, qint64 position);
		qint64 position(ViAudioTransmission::ViTransmissionType type);

	protected:
		QString mFilePath;
};

#endif
