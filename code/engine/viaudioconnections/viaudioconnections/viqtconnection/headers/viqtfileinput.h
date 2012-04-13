#ifndef VIQTFILEINPUT_H
#define VIQTFILEINPUT_H

#include "vifileinput.h"
#include <QThread>

class ViQtFileInput : public ViFileInput
{
	Q_OBJECT

	public:
		ViQtFileInput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, QString filePath = "");
		~ViQtFileInput();
		void start();
		void stop();
		void pause();
		void initialize();

	protected:
		void free();

};

#endif

