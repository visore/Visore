#ifndef VIFILEOUTPUT_H
#define VIFILEOUTPUT_H

#include "viaudiooutput.h"

class ViFileOutput : public ViAudioOutput
{
    Q_OBJECT

	public slots:

		virtual void bufferChanged(int size);

	public:

		ViFileOutput();

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
