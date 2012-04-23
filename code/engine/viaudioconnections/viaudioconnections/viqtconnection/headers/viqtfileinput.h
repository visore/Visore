#ifndef VIQTFILEINPUT_H
#define VIQTFILEINPUT_H

#include "vifileinput.h"
#include "vicoder.h"
#include <QThread>

class ViQtFileInput : public ViFileInput
{
	Q_OBJECT

	public:
		ViQtFileInput(ViAudioFormat format, ViAudioBuffer *buffer = NULL, QString filePath = "");
		~ViQtFileInput();
		void start();
		void stop();
		void pause();
		void initialize();

	protected:
		void free();

	private:
		ViCoder mCoder;

};

#endif

