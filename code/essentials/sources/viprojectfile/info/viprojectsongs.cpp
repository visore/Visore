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
		song.addChild("Title", mSongInfos[i].songTitle(true));
		song.addChild("AlbumArt", mSongInfos[i].albumArtPath());

		//File
		ViInfoElement file("File");
		file.addChild("Original", mSongInfos[i].originalFilePath());
		file.addChild("Corrected", mSongInfos[i].correctedFilePath());
		song.addChild(file);

		root.addChild(song);
	}

	return root;
}

bool ViProjectSongs::fromXml(ViInfoElement &document)
{
	if(document.name() == name())
	{
		mSongInfos.clear();
		ViInfoElementList children = document.children();
		for(int i = 0; i < children.size(); ++i)
		{
			ViFileSongInfo info;
			info.setArtistName(children[i].child("Artist").valueString());
			info.setSongTitle(children[i].child("Title").valueString());
			info.setAlbumArtPath(children[i].child("AlbumArt").valueString());
			ViInfoElement file = children[i].child("File");
			info.setOriginalFilePath(file.child("Original").valueString());
			info.setCorrectedFilePath(file.child("Corrected").valueString());
			mSongInfos.append(info);
		}
		return true;
	}
	return false;
}
