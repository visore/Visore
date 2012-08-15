#ifndef VIFILEOUTPUT_H
#define VIFILEOUTPUT_H

#include "viaudiooutput.h"
#include "viaudiocoder.h"

class ViFileOutput : public ViAudioOutput
{
    Q_OBJECT

	public slots:

		void setSongInfo(ViSongInfo info);
		void clearSongInfo();

	public:

		ViFileOutput();

		void setFile(QString directory, QString trackNumber, QString extension);
		void setFile(QString filePath);
		ViAudioFormat format();
		void setFormat(ViAudioFormat format);

		ViSongInfo songInfo();
		QString filePath();

		void start();
		void stop();
		void pause();

	protected:

		QString mFilePath;
		ViAudioCoder mCoder;
		ViAudioFormat mFormat;
		ViSongInfo mSongInfo;

};

#endif
