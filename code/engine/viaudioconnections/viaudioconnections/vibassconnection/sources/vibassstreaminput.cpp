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
	mRecordHandle = 0;
}

ViBassStreamInput::~ViBassStreamInput()
{
	free();
}

void ViBassStreamInput::initialize()
{
	free();
	if(!BASS_RecordInit(-1))
	{
		setErrorParameters("ViBassStreamInput - Initializing Error", "Could not initialize the Bass recording device", ViErrorInfo::Fatal);
	}
	globalStream = mStream;
	DWORD flags = 0;
	if(mMetaData->bitDepth() == 8)
	{
		flags = BASS_RECORD_PAUSE|BASS_SAMPLE_8BITS;
	}
	else if(mMetaData->bitDepth() == 16)
	{
		flags = BASS_RECORD_PAUSE;
	}
	else if(mMetaData->bitDepth() == 32)
	{
		flags = BASS_RECORD_PAUSE|BASS_SAMPLE_FLOAT;
	}
	else
	{
		setErrorParameters("ViBassStreamInput - Bit Depth", "A bit depth of " + QString::number(mMetaData->bitDepth()) + " is not supported", ViErrorInfo::Fatal);
	}
	mRecordHandle = BASS_RecordStart(mMetaData->frequency(), mMetaData->channels(), flags, &recordingCallback, 0);
}

void ViBassStreamInput::free()
{
	if(!BASS_StreamFree(mRecordHandle) && mRecordHandle != 0)
	{
		setErrorParameters("ViBassStreamInput - Memory Release Error", "The supporting Bass handle could not be released", ViErrorInfo::Fatal);
	}
	BASS_RecordFree();
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
