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

ViElement ViProjectSongs::toXml()
{
	ViElement root(name());

	for(int i = 0; i < mSongInfos.size(); ++i)
	{
		//Song
		ViElement song("Song");
		song.addAttribute("id", QString::number(i + 1));
		song.addChild("Artist", mSongInfos[i].artistName(true));
		song.addChild("Title", mSongInfos[i].songTitle(true));
		song.addChild("AlbumArt", mSongInfos[i].albumArtPath());

		//File
		ViElement file("File");
		file.addChild("Original", mSongInfos[i].originalFilePath());
		file.addChild("Corrected", mSongInfos[i].correctedFilePath());
		song.addChild(file);

		root.addChild(song);
	}

	return root;
}

bool ViProjectSongs::fromXml(ViElement &document)
{
	if(document.name() == name())
	{
		mSongInfos.clear();
		ViElementList children = document.children();
		for(int i = 0; i < children.size(); ++i)
		{
			ViFileSongInfo info;
			info.setArtistName(children[i].child("Artist").toString());
			info.setSongTitle(children[i].child("Title").toString());
			info.setAlbumArtPath(children[i].child("AlbumArt").toString());
			ViElement file = children[i].child("File");
			info.setOriginalFilePath(file.child("Original").toString());
			info.setCorrectedFilePath(file.child("Corrected").toString());
			mSongInfos.append(info);
		}
		return true;
	}
	return false;
}
