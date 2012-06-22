#ifndef VIRECORDINGINFO_H
#define VIRECORDINGINFO_H

#include "viprojectinfo.h"

class ViRecordingInfo : public ViProjectInfo
{

	public:

		

	private:

		QString mAlbumName;
		int mAlbumYear;
		QDateTime mRecordingStarted;
		QDateTime mRecordingEnded;
		QList<ViSongInfo> mSongs;
		

};

#endif
