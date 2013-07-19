#include <viaudioplayer.h>

ViAudioPlayer::ViAudioPlayer()
{
	clear();
	QObject::connect(&mOutput, SIGNAL(started()), this, SIGNAL(played()));
	QObject::connect(&mOutput, SIGNAL(paused()), this, SIGNAL(paused()));
	QObject::connect(&mOutput, SIGNAL(stopped()), this, SIGNAL(stopped()));
	QObject::connect(&mOutput, SIGNAL(positionChanged(ViAudioPosition)), this, SIGNAL(positionChanged(ViAudioPosition)));
	QObject::connect(&mOutput, SIGNAL(volumeChanged(int)), this, SIGNAL(volumeChanged(int)));
	QObject::connect(&mOutput, SIGNAL(underrun()), this, SLOT(checkUnderrun()));
}

ViAudioPlayer::~ViAudioPlayer()
{
	clear();
}

void ViAudioPlayer::clear()
{
	QMutexLocker locker(&mMutex);
	mFormat = ViAudioFormat();
	mOutput.clear();
	mOldDuration = 0;
	mObject.setNull();
}

void ViAudioPlayer::setData(ViAudioObjectPointer object, ViAudio::Type type, bool startPlaying)
{
	clear();
	QMutexLocker locker(&mMutex);
	mObject = object;
	bool hasBuffer = object->hasBuffer(type);
	ViBuffer *buffer = object->buffer(type);
	mOutput.setBuffer(buffer);
	if(!hasBuffer)
	{
		if(object->hasFile(type))
		{
			if(startPlaying)
			{
				QObject::connect(buffer, SIGNAL(formatChanged(ViAudioFormat)), this, SLOT(play()));
			}
			locker.unlock();
			QObject::connect(object.data(), SIGNAL(decoded()), this, SLOT(checkDuration()));
			object->decode(type);
		}
		else
		{
			LOG("No buffer or file available for playback.", QtCriticalMsg);
		}
	}
	else if(startPlaying)
	{
		locker.unlock();
		play();
	}
}

void ViAudioPlayer::setData(ViBuffer *buffer, bool startPlaying)
{
	clear();
	QMutexLocker locker(&mMutex);
	mOutput.setBuffer(buffer);
	if(startPlaying)
	{
		locker.unlock();
		play();
	}
}

void ViAudioPlayer::play()
{
	QMutexLocker locker(&mMutex);
	checkDuration();
	mFormat = mOutput.format();
	if(mOutput.buffer() != NULL)
	{
		QObject::connect(mOutput.buffer(), SIGNAL(formatChanged(ViAudioFormat)), this, SLOT(play()));
	}
	mOutput.start();
}

void ViAudioPlayer::pause()
{
	QMutexLocker locker(&mMutex);
	mOutput.pause();
}

void ViAudioPlayer::stop()
{
	QMutexLocker locker(&mMutex);
	mOutput.stop();
}

void ViAudioPlayer::changePosition(ViAudioPosition position)
{
	QMutexLocker locker(&mMutex);
	mOutput.setPosition(position);
}

void ViAudioPlayer::changeVolume(int volume)
{
	QMutexLocker locker(&mMutex);
	mOutput.setVolume(volume / 100.0);
}

void ViAudioPlayer::checkUnderrun()
{
	emit positionChanged(ViAudioPosition());
}

void ViAudioPlayer::checkDuration()
{
	if(!mObject.isNull())
	{
		QObject::disconnect(mObject.data(), SIGNAL(decoded()), this, SLOT(checkDuration()));
	}

	ViBuffer *buffer = mOutput.buffer();
	if(buffer!= NULL && mOldDuration != buffer->size())
	{
		mOldDuration = buffer->size();
		emit durationChanged(ViAudioPosition(mOldDuration, ViAudioPosition::Bytes, buffer->format()));
	}
}
