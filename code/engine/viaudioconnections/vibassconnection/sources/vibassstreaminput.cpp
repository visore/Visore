#include "vibassstreaminput.h"

ViAudioBuffer *globalBuffer = NULL;

BOOL CALLBACK recordingCallback(HRECORD handle, const void *buffer, DWORD length, void *user)
{
	char *newBuffer = new char[length];
	memcpy (newBuffer, buffer, length);
	ViAudioBufferChunk chunk(newBuffer);
	globalBuffer->write(&chunk, length);
	return true;
}

ViBassStreamInput::ViBassStreamInput(ViAudioBuffer *buffer, ViAudioMetaData *metaData, ViAudioDevice *device)
	: ViStreamInput(buffer, metaData, device)
{
	if(!BASS_RecordInit(-1))
	{
		setErrorParameters("ViBassStreamInput - Initializing Error", "Could not initialize the Bass recording device", ViErrorInfo::Fatal);
	}
	metaData->setFrequency(44100);
	metaData->setChannels(2);
	mRecordHandle = BASS_RecordStart(metaData->frequency(), metaData->channels(), BASS_RECORD_PAUSE, &recordingCallback, 0);
	globalBuffer = mBuffer;
}

ViBassStreamInput::~ViBassStreamInput()
{
	if(!BASS_StreamFree(mRecordHandle))
	{
		setErrorParameters("ViBassStreamInput - Memory Release Error", "The supporting Bass handle could not be released", ViErrorInfo::Fatal);
	}
	if(!BASS_RecordFree())
	{
		setErrorParameters("ViBassStreamInput - Recording Memory Release Error", "The supporting Bass recording device could not be released", ViErrorInfo::Fatal);
	}
}

void ViBassStreamInput::start()
{
	if(BASS_ChannelIsActive(mRecordHandle) == BASS_ACTIVE_PLAYING)
	{
		setErrorParameters("ViBassStreamInput - Start Error", "The device is already recording", ViErrorInfo::NonFatal);
	}
	else if(!BASS_ChannelPlay(mRecordHandle, false))
	{
		setErrorParameters("ViBassStreamInput - Start Error", "The recording stream could not be started", ViErrorInfo::MediumFatal);
	}
}

void ViBassStreamInput::stop()
{
	if(BASS_ChannelIsActive(mRecordHandle) == BASS_ACTIVE_STOPPED)
	{
		setErrorParameters("ViBassStreamInput - Stop Error", "The device is already stopped", ViErrorInfo::NonFatal);
	}
	else if(!BASS_ChannelStop(mRecordHandle))
	{
		setErrorParameters("ViBassStreamInput - Stop Error", "The recording stream could not be stopped", ViErrorInfo::MediumFatal);
	}
}

void ViBassStreamInput::pause()
{
	if(BASS_ChannelIsActive(mRecordHandle) == BASS_ACTIVE_PAUSED)
	{
		setErrorParameters("ViBassStreamInput - Pause Error", "The device is already paused", ViErrorInfo::NonFatal);
	}
	else if(!BASS_ChannelPause(mRecordHandle))
	{
		setErrorParameters("ViBassStreamInput - Pause Error", "The recording stream could not be paused", ViErrorInfo::MediumFatal);
	}
}
