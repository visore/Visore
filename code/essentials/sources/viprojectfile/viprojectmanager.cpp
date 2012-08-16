#include <viprojectmanager.h>

ViProjectManager::ViProjectManager()
	: QObject()
{
	reset();
}

void ViProjectManager::setProjectName(QString name)
{
	mProject.setProjectName(name);
}

void ViProjectManager::setFilePath(QString filePath)
{
	mProject.setFilePath(filePath);
}

void ViProjectManager::setSideCount(short count)
{
	mSideCount = count;
	mCurrentSide = 1;
}

void ViProjectManager::setPlayback(bool play)
{
	mPlayback = play;
}

ViProject* ViProjectManager::project()
{
	return &mProject;
}

void ViProjectManager::start()
{
	emit started();
	setStatus("Please start the record");
}

void ViProjectManager::startRecord()
{
	setStatus("Waiting for song to start");
}

void ViProjectManager::endRecord()
{
	if(mCurrentSide == mSideCount)
	{
		emit finished();
	}
	else
	{
		++mCurrentSide;
		if(mCurrentSide % 2 == 0)
		{
			setStatus("Please turn the record over");
		}
		else
		{
			setStatus("Please start the next record");
		}
	}
}

void ViProjectManager::startSong()
{
	setStatus("Processing song");
	if(mPlayback)
	{
		emit startPlayback();
	}
}

void ViProjectManager::endSong()
{
	setStatus("Waiting for song to start");
}

void ViProjectManager::reset()
{
	mSideCount = 0;
	mCurrentSide = 0;
	mPlayback = false;
}

void ViProjectManager::setStatus(QString status)
{
	emit statusChanged(status);
}
