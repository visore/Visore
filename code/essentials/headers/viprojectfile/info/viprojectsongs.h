#ifndef VIPROJECTSONGS_H
#define VIPROJECTSONGS_H

#include "viprojectinfo.h"
#include "vifilesonginfo.h"

class ViProjectSongs : public ViProjectInfo
{

	public:

		ViProjectSongs();

		ViFileSongInfoList songInfos();
		void addSongInfo(ViFileSongInfo info);

	protected:

		ViInfoElement toXml();
		bool fromXml(ViInfoElement &document);

	private:

		ViFileSongInfoList mSongInfos;

};

#endif
