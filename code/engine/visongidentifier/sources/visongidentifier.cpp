#include "visongidentifier.h"
#include "vilogger.h"

ViSongIdentifier::ViSongIdentifier()
{
	mFound = false;
}

bool ViSongIdentifier::found()
{
	return mFound;
}

void ViSongIdentifier::detected(ViSongInfo info)
{
	LOG("Song info detected (" + info.artistName() + " - " + info.songTitle() + ").");
	emit songDetected(info);
}
