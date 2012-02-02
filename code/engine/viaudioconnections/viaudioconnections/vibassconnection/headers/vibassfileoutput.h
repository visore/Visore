#ifndef VIBASSFILEOUTPUT_H
#define VIBASSFILEOUTPUT_H

#define FILE_CHUNK_SIZE 10000

#include "vifileoutput.h"
#include <QProcess>

class ViBassFileOutput : public ViFileOutput
{
	Q_OBJECT

	public:
		ViBassFileOutput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, QString filePath = "");
		~ViBassFileOutput();
		void start();
		void stop();
		void pause();
		void initialize();

	protected:
		void free();

	private:
		QProcess mProcess;
};

#endif
