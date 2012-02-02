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
	/*if(!BASS_SetDevice(3))
	{
		setErrorParameters("ViBassStreamOutput - Device Error", "Cannot stream to the selected output device", ViErrorInfo::Fatal);
	}*/
	mHandle = BASS_StreamCreate(mMetaData->frequency(), mMetaData->channels(), 0, STREAMPROC_PUSH, 0);
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
}

ViBassStreamOutput::~ViBassStreamOutput()
{
	if(!BASS_StreamFree(mHandle))
	{
		setErrorParameters("ViBassStreamOutput - Memory Release Error", "The supporting Bass handle could not be released", ViErrorInfo::Fatal);
	}
	if(mReceiver != NULL)
	{
		delete mReceiver;
		mReceiver = NULL;
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

qint64 ViBassStreamOutput::setPosition(ViAudioTransmission::ViTransmissionType type, qint64 position)
{
	if(type == ViAudioTransmission::Seconds)
	{
		position = position/mSecondsInByte;
	}
	else if(type == ViAudioTransmission::Milliseconds)
	{
		position = position/(mSecondsInByte*1000.0);
	}
	if(position % 2 != 0) //Bass can only play even bytes
	{
		position -= 1;
	}
	if(!mStream->isValidPosition(position))
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

	qint64 read = mStream->setPosition(position);
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
	return read;
}

qint64 ViBassStreamOutput::position(ViAudioTransmission::ViTransmissionType type)
{

}
