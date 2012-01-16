#ifndef VIFILEINPUT_H
#define VIFILEINPUT_H

#include "viaudioinput.h"
#include <QFile>

class ViFileInput : public ViAudioInput
{
	Q_OBJECT

	public:
		ViFileInput(QString filePath, QAudioFormat format = QAudioFormat());
		~ViFileInput();

	private:
		QFile *mAnalysisFile;

};

#endif
