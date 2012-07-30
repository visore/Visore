#include "visongfile.h"

ViSongFile::ViSongFile()
	: ViProjectFile()
{
	setName("Songs");
}

ViFileSongInfoList ViSongFile::songInfos()
{
	return mSongInfos;
}

void ViSongFile::addSongInfo(ViFileSongInfo info)
{
	mSongInfos.append(info);
}

QDomDocument ViSongFile::toXml()
{
	QDomDocument document;

	QDomElement root = document.createElement(name());
	document.appendChild(root);

	for(int i = 0; i < mSongInfos.size(); ++i)
	{
		QDomElement song = document.createElement("Song");
		song.setAttribute("id", QString::number(i + 1));
		root.appendChild(song);

		QDomElement artist = document.createElement("Artist");
		song.appendChild(artist);
		QString artistName = mSongInfos[i].artistName();
		if(artistName == "")
		{
			artistName = "Unknown Artist";
		}
		QDomText artistText = document.createTextNode(artistName);
		artist.appendChild(artistText);

		QDomElement title = document.createElement("Title");
		song.appendChild(title);
		QString songTitle = mSongInfos[i].songTitle();
		if(songTitle == "")
		{
			songTitle = "Unknown Title";
		}
		QDomText titleText = document.createTextNode(songTitle);
		title.appendChild(titleText);

		QDomElement file = document.createElement("File");
		song.appendChild(file);

		QDomElement original = document.createElement("Original");
		file.appendChild(original);
		QDomText originalText = document.createTextNode(mSongInfos[i].correctedFilePath());
		original.appendChild(originalText);

		QDomElement corrected = document.createElement("Corrected");
		file.appendChild(corrected);
		QDomText correctedText = document.createTextNode(mSongInfos[i].originalFilePath());
		corrected.appendChild(correctedText);

		QDomElement albumArt = document.createElement("AlbumArt");
		song.appendChild(albumArt);
		QDomText albumArtText = document.createTextNode(mSongInfos[i].albumArtPath());
		albumArt.appendChild(albumArtText);
	}

	return document;
}

void ViSongFile::fromXml(QDomDocument document)
{
	/*QDomElement created = document.firstChildElement("Created");
	QDomElement createdVersion = created.firstChildElement("Version");
	mCreatedVersion = ViVersion(createdVersion.firstChildElement("Major").text().toInt(), createdVersion.firstChildElement("Minor").text().toInt(), createdVersion.firstChildElement("Patch").text().toInt());
	QDomElement edited = document.firstChildElement("Edited");
	QDomElement editedVersion = edited.firstChildElement("Version");
	mEditedVersion = ViVersion(editedVersion.firstChildElement("Major").text().toInt(), editedVersion.firstChildElement("Minor").text().toInt(), editedVersion.firstChildElement("Patch").text().toInt());*/
}
