#ifndef VISONGFILE_H
#define VISONGFILE_H

#include "viprojectfile.h"
#include "vifilesonginfo.h"

class ViSongFile : public ViProjectFile
{

	public:

		ViSongFile();

		ViFileSongInfoList songInfos();
		void addSongInfo(ViFileSongInfo info);

	protected:

		QDomDocument toXml();
		void fromXml(QDomDocument document);

	private:

		ViFileSongInfoList mSongInfos;

};

#endif
