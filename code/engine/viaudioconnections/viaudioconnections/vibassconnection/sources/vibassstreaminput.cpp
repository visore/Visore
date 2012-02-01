#include "vibassstreaminput.h"

ViAudioBufferStream *globalStream = NULL;

BOOL CALLBACK recordingCallback(HRECORD handle, const void *buffer, DWORD length, void *user)
{
	char *newBuffer = new char[length];
	memcpy(newBuffer, buffer, length);
	ViAudioBufferChunk chunk(newBuffer);
	globalStream->write(&chunk, length);
	return true;
}

ViBassStreamInput::ViBassStreamInput(ViAudioBuffer *buffer, ViAudioMetaData *metaData, ViAudioDevice *device)
	: ViStreamInput(buffer, metaData, device)
{
	if(!BASS_RecordInit(-1))
	{
		setErrorParameters("ViBassStreamInput - Initializing Error", "Could not initialize the Bass recording device", ViErrorInfo::Fatal);
	}
	globalStream = mStream;
	mRecordHandle = BASS_RecordStart(mMetaData->frequency(), mMetaData->channels(), BASS_RECORD_PAUSE, &recordingCallback, 0);
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
	if(mStatus == ViAudioTransmission::Running)
	{
		setErrorParameters("ViBassStreamInput - Start Error", "The device is already recording", ViErrorInfo::NonFatal);
		return;
	}
	else if(mStatus == ViAudioTransmission::Stopped)
	{
		mStream->restart();
		mRecordHandle = BASS_RecordStart(mMetaData->frequency(), mMetaData->channels(), BASS_RECORD_PAUSE, &recordingCallback, 0);
	}
	if(!BASS_ChannelPlay(mRecordHandle, false))
	{
		setErrorParameters("ViBassStreamInput - Start Error", "The recording stream could not be started", ViErrorInfo::MediumFatal);
	}
	mStatus = ViAudioTransmission::Running;
}

void ViBassStreamInput::stop()
{
	if(mStatus == ViAudioTransmission::Stopped)
	{
		setErrorParameters("ViBassStreamInput - Stop Error", "The device is already stopped", ViErrorInfo::NonFatal);
		return;
	}
	if(!BASS_ChannelStop(mRecordHandle))
	{
		setErrorParameters("ViBassStreamInput - Stop Error", "The recording stream could not be stopped", ViErrorInfo::MediumFatal);
	}
	mStatus = ViAudioTransmission::Stopped;
}

void ViBassStreamInput::pause()
{
	if(mStatus == ViAudioTransmission::Paused)
	{
		setErrorParameters("ViBassStreamInput - Pause Error", "The device is already paused", ViErrorInfo::NonFatal);
		return;
	}
	if(!BASS_ChannelPause(mRecordHandle))
	{
		setErrorParameters("ViBassStreamInput - Pause Error", "The recording stream could not be paused", ViErrorInfo::MediumFatal);
	}
	mStatus = ViAudioTransmission::Paused;
}
