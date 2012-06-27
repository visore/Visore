#ifndef VIFILEOUTPUT_H
#define VIFILEOUTPUT_H

#include "viaudiooutput.h"
#include "vicoder.h"

class ViFileOutput : public ViAudioOutput
{
    Q_OBJECT

	public:

		ViFileOutput();

		void setFile(QString filePath);
		ViAudioFormat format();
		void setFormat(ViAudioFormat format);

		void start();
		void stop();
		void pause();

	protected:

		QString mFilePath;
		ViCoder mCoder;
		ViAudioFormat mFormat;

};

#endif
