#include "vibassstreamoutput.h"

ViBassStreamOutputReceiver::ViBassStreamOutputReceiver(ViBassStreamOutput *parent, ViAudioBufferStream *stream, HSTREAM handle)
{
	mParent = parent;
	mStream = stream;
	mHandle = handle;
}

void ViBassStreamOutputReceiver::changeReceived(int size)
{
	ViAudioBufferChunk chunk;
	mStream->read(&chunk, size);
	if(BASS_StreamPutData(mHandle, chunk.data(), size) == -1)
	{
		mParent->setErrorParameters("ViBassStreamOutput - Stream Error", "The specified data could not be written to the stream", ViErrorInfo::Fatal);
	}
}

ViBassStreamOutput::ViBassStreamOutput(ViAudioBuffer *buffer, ViAudioMetaData *metaData, ViAudioDevice *device)
	: ViStreamOutput(buffer, metaData, device)
{
	mHandle = 0;
	mReceiver = NULL;
	mTimer = NULL;
}

ViBassStreamOutput::~ViBassStreamOutput()
{
	free();
}

void ViBassStreamOutput::initialize()
{
	free();
	mOldPosition = -1;
	/*if(!BASS_SetDevice(3))
	{
		setErrorParameters("ViBassStreamOutput - Device Error", "Cannot stream to the selected output device", ViErrorInfo::Fatal);
	}*/
	DWORD flags = 0;
	if(mMetaData->bitDepth() == 8)
	{
		flags = BASS_SAMPLE_8BITS;
	}
	else if(mMetaData->bitDepth() == 32)
	{
		flags = BASS_SAMPLE_FLOAT;
	}
	else if(mMetaData->bitDepth() != 16)
	{
		setErrorParameters("ViBassStreamOutput - Bit Depth", "A bit depth of " + QString::number(mMetaData->bitDepth()) + " is not supported", ViErrorInfo::Fatal);
	}
	mHandle = BASS_StreamCreate(mMetaData->frequency(), mMetaData->channels(), flags, STREAMPROC_PUSH, 0);
	if(mHandle == 0)
	{
		mSecondsInByte = -1;
		setErrorParameters("ViBassStreamOutput - Output Error", "The stream could not be opened", ViErrorInfo::Fatal);
	}
	else
	{
		mSecondsInByte = BASS_ChannelBytes2Seconds(mHandle, 1);
	}
	/*if(!BASS_ChannelSetDevice(mHandle, 2))
	{cout<<"rrr: "<<BASS_ErrorGetCode()<<endl;
		setErrorParameters("ViBassStreamOutput - Device Error", "Cannot stream to the selected output device", ViErrorInfo::Fatal);
	}*/
	mReceiver = new ViBassStreamOutputReceiver(this, mStream, mHandle);
	mTimer = new QTimer(this);
	ViObject::connect(mTimer, SIGNAL(timeout()), this, SLOT(checkPosition()));
	mTimer->start(POSITION_CHECK_INTERVAL);
}

void ViBassStreamOutput::free()
{
	if(mReceiver != NULL)
	{
		delete mReceiver;
		mReceiver = NULL;
	}
	if(!BASS_StreamFree(mHandle) && mHandle != 0)
	{
		setErrorParameters("ViBassStreamOutput - Memory Release Error", "The supporting Bass handle could not be released", ViErrorInfo::Fatal);
	}
	if(mTimer != NULL)
	{
		mTimer->stop();
		ViObject::disconnect(mTimer, SIGNAL(timeout()), this, SLOT(checkPosition()));
		delete mTimer;
		mTimer = NULL;
	}
}

void ViBassStreamOutput::bufferChanged(int size)
{
	mReceiver->changeReceived(size);
}

void ViBassStreamOutput::start()
{
	if(mStatus == ViAudioTransmission::Running)
	{
		setErrorParameters("ViBassStreamOutput - Start Error", "The device is already playing", ViErrorInfo::NonFatal);
		return;
	}
	else if(!BASS_ChannelPlay(mHandle, false))
	{
		setErrorParameters("ViBassStreamOutput - Start Error", "The palyback stream could not be started", ViErrorInfo::MediumFatal);
		return;
	}
	mStatus = ViAudioTransmission::Running;
}

void ViBassStreamOutput::stop()
{
	if(mStatus == ViAudioTransmission::Stopped)
	{
		setErrorParameters("ViBassStreamOutput - Stop Error", "The device is already stopped", ViErrorInfo::NonFatal);
		return;
	}
	else if(!BASS_ChannelStop(mHandle))
	{
		setErrorParameters("ViBassStreamOutput - Stop Error", "The palyback stream could not be stopped", ViErrorInfo::MediumFatal);
		return;
	}
	mStatus = ViAudioTransmission::Stopped;
	mStream->restart();
}

void ViBassStreamOutput::pause()
{
	if(mStatus == ViAudioTransmission::Paused)
	{
		setErrorParameters("ViBassStreamOutput - Pause Error", "The device is already paused", ViErrorInfo::NonFatal);
		return;
	}
	else if(!BASS_ChannelPause(mHandle))
	{
		setErrorParameters("ViBassStreamOutput - Pause Error", "The palyback stream could not be paused", ViErrorInfo::MediumFatal);
		return;
	}
	mStatus = ViAudioTransmission::Paused;
}

bool ViBassStreamOutput::setPosition(ViAudioPosition position)
{
	qint64 pos = position.bytes();
	if(pos % 2 != 0) //Bass can only play even bytes
	{
		pos -= 1;
	}
	if(!mStream->isValidPosition(pos))
	{
		setErrorParameters("ViBassStreamOutput - Position Error", "The position could not be set", ViErrorInfo::MediumFatal);
		return -1;
	}

	bool playing = false;
	if(mStatus == ViAudioTransmission::Running) //Only start playing if the stream wasn't stopped in the first place
	{
		playing = true;
	}
	if(!BASS_ChannelStop(mHandle))
	{
		setErrorParameters("ViBassStreamOutput - Position Error", "The palyback stream could not be stopped", ViErrorInfo::MediumFatal);
		return -1;
	}
	mStatus = ViAudioTransmission::Stopped;

	bool result = true;
	if(mStream->setPosition(pos) < 0)
	{
		result = false;
	}
	if(read < 0)
	{
		setErrorParameters("ViBassStreamOutput - Position Error", "The position could not be set", ViErrorInfo::MediumFatal);
	}
	else if(playing)
	{
		if(!BASS_ChannelPlay(mHandle, false))
		{
			setErrorParameters("ViBassStreamOutput - Position Error", "The palyback stream could not be started", ViErrorInfo::MediumFatal);
			return -1;
		}
		mStatus = ViAudioTransmission::Running;
	}
	return result;
}

ViAudioPosition ViBassStreamOutput::position()
{
	QWORD position = BASS_ChannelGetPosition(mHandle, BASS_POS_BYTE);
	if(position == -1)
	{
		setErrorParameters("ViBassStreamOutput - Position Error", "The palyback position could not be retrieved", ViErrorInfo::MediumFatal);
		return -1;
	}
	//Ensures that the position is not 0 if no data is available
	if(mBuffer->size() == 0)
	{
		position = -1;
	}
	return ViAudioPosition(position, mMetaData->bitDepth(), mSecondsInByte);
}

void ViBassStreamOutput::checkPosition()
{
	ViAudioPosition pos = position();
	if(pos != mOldPosition)
	{
		mOldPosition = pos;
		emit positionChanged(pos);
	}
}

qreal ViBassStreamOutput::volume()
{
	float result;
	if(!BASS_ChannelGetAttribute(mHandle, BASS_ATTRIB_VOL, &result))
	{
		setErrorParameters("ViBassStreamOutput - Volume Error", "The volume could not be retrieved", ViErrorInfo::MediumFatal);
		return -1;
	}
	return result;
}

void ViBassStreamOutput::setVolume(qreal volume)
{
	BASS_ChannelSetAttribute(mHandle, BASS_ATTRIB_VOL, volume);
}
