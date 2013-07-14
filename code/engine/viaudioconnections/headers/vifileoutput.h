#ifndef VIFILEOUTPUT_H
#define VIFILEOUTPUT_H

#include "viaudiooutput.h"
#include "viaudiocoder.h"

class ViFileOutput : public ViAudioOutput
{
    Q_OBJECT

	public slots:

		void setMetadata(ViMetadata metadata);
		void clearMetadata();

	public:

		ViFileOutput();

		void setFile(QString directory, QString trackNumber, QString extension);
		void setFile(QString filePath);
		ViAudioFormat format();
		void setFormat(ViAudioFormat format);

		ViMetadata metadata();
		QString filePath();

		void start();
		void stop();
		void pause();

	protected:

		QString mFilePath;
		ViAudioCoder mCoder;
		ViAudioFormat mFormat;
		ViMetadata mMetadata;

};

#endif
