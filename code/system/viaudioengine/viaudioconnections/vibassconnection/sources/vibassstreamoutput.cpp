#include "vibassstreamoutput.h"

ViBassStreamOutputReceiver::ViBassStreamOutputReceiver(ViBassStreamOutput *parent, ViAudioBuffer *buffer, HSTREAM handle)
{
	mParent = parent;
	mBuffer = buffer;
	mHandle = handle;
	ViObject::connectDirect(mBuffer, SIGNAL(changed(int, int)), this, SLOT(changeReceived(int, int)));
}

void ViBassStreamOutputReceiver::changeReceived(int startIndex, int size)
{
	ViAudioBufferChunk chunk;
	mBuffer->read(&chunk, size);
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
		setErrorParameters("ViBassStreamOutput - Output Error", "The stream could not be opened", ViErrorInfo::Fatal);
	}
	if(!BASS_ChannelSetDevice(mHandle, 2))
	{cout<<"rrr: "<<BASS_ErrorGetCode()<<endl;
		setErrorParameters("ViBassStreamOutput - Device Error", "Cannot stream to the selected output device", ViErrorInfo::Fatal);
	}
	mReceiver = new ViBassStreamOutputReceiver(this, mBuffer, mHandle);
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

void ViBassStreamOutput::start()
{
	if(mStatus == ViAudioTransmission::Running)
	{
		setErrorParameters("ViBassStreamOutput - Start Error", "The device is already playing", ViErrorInfo::NonFatal);
	}
	else if(!BASS_ChannelPlay(mHandle, false))
	{
		setErrorParameters("ViBassStreamOutput - Start Error", "The palyback stream could not be started", ViErrorInfo::MediumFatal);
	}
	mStatus = ViAudioTransmission::Running;
}

void ViBassStreamOutput::stop()
{
	if(mStatus == ViAudioTransmission::Stopped)
	{
		setErrorParameters("ViBassStreamOutput - Stop Error", "The device is already stopped", ViErrorInfo::NonFatal);
	}
	else if(!BASS_ChannelStop(mHandle))
	{
		setErrorParameters("ViBassStreamOutput - Stop Error", "The palyback stream could not be stopped", ViErrorInfo::MediumFatal);
	}
	mStatus = ViAudioTransmission::Stopped;
	mBuffer->restartRead();
}

void ViBassStreamOutput::pause()
{
	if(mStatus == ViAudioTransmission::Paused)
	{
		setErrorParameters("ViBassStreamOutput - Pause Error", "The device is already paused", ViErrorInfo::NonFatal);
	}
	else if(!BASS_ChannelPause(mHandle))
	{
		setErrorParameters("ViBassStreamOutput - Pause Error", "The palyback stream could not be paused", ViErrorInfo::MediumFatal);
	}
	mStatus = ViAudioTransmission::Paused;
}
