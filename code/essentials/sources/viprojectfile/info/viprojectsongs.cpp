#include "viprojectsongs.h"

ViProjectSongs::ViProjectSongs()
	: ViProjectInfo()
{
	setName("Songs");
}

ViFileSongInfoList ViProjectSongs::songInfos()
{
	return mSongInfos;
}

void ViProjectSongs::addSongInfo(ViFileSongInfo info)
{
	mSongInfos.append(info);
}

ViInfoElement ViProjectSongs::toXml()
{
	ViInfoElement root(name());

	for(int i = 0; i < mSongInfos.size(); ++i)
	{
		//Song
		ViInfoElement song("Song");
		song.addAttribute("id", QString::number(i + 1));
		song.addChild("Artist", mSongInfos[i].artistName(true));
		song.addChild("Artist", mSongInfos[i].songTitle(true));
		song.addChild("AlbumArt", mSongInfos[i].albumArtPath());

		//File
		ViInfoElement file("File");
		song.addChild("Original", mSongInfos[i].correctedFilePath());
		song.addChild("Corrected", mSongInfos[i].originalFilePath());
		song.addChild(file);

		root.addChild(song);
	}

	return root;
}

bool ViProjectSongs::fromXml(ViInfoElement &document)
{
	
}
