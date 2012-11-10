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

		ViElement toXml();
		bool fromXml(ViElement &document);

	private:

		ViFileSongInfoList mSongInfos;

};

#endif
