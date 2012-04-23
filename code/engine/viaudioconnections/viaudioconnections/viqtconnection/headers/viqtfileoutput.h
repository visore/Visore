#ifndef VIQTFILEOUTPUT_H
#define VIQTFILEOUTPUT_H

#define FILE_CHUNK_SIZE 10000

#include "vifileoutput.h"
#include <QProcess>

class ViQtFileOutput : public ViFileOutput
{
	Q_OBJECT

	public:
		ViQtFileOutput(ViAudioFormat format, ViAudioBuffer *buffer = NULL, QString filePath = "");
		~ViQtFileOutput();
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
