#include "vibassstreamoutput.h"

ViBassStreamOutput::ViBassStreamOutput(ViAudioBuffer *buffer, ViAudioMetaData *metaData, ViAudioDevice *device)
	: ViStreamOutput(buffer, metaData, device)
{
}

ViBassStreamOutput::~ViBassStreamOutput()
{
	if(mBuffer != NULL)
	{
		ViObject::disconnect(mBuffer, SIGNAL(changed(int, int)), this, SLOT(changeReceived(int, int)));
	}
	if(!BASS_StreamFree(mHandle))
	{
		setErrorParameters("ViBassStreamOutput - Memory Release Error", "The supporting Bass handle could not be released", ViErrorInfo::Fatal);
		return;
	}
}

void ViBassStreamOutput::start()
{
	//mHandle = BASS_StreamCreate(mMetaData.frequency(), mMetaData.channels(), 0, STREAMPROC_PUSH, 0);
	mHandle = BASS_StreamCreate(44100, 2, 0, STREAMPROC_PUSH, 0);
	if(mHandle == 0)
	{
		setErrorParameters("ViBassStreamOutput - Output Error", "The stream could not be opened", ViErrorInfo::Fatal);
		return;
	}
	if(!BASS_ChannelPlay(mHandle, false))
	{
		setErrorParameters("ViBassStreamOutput - Playback Error", "The stream could not be played back", ViErrorInfo::Fatal);
		return;
	}
	ViObject::connectThread(mBuffer, SIGNAL(changed(int, int)), this, SLOT(changeReceived(int, int)));
}

void ViBassStreamOutput::changeReceived(int startIndex, int size)
{
	ViAudioBufferChunk chunk;
	mBuffer->read(&chunk, size);
	if(BASS_StreamPutData(mHandle, chunk.data(), size) == -1)
	{
		setErrorParameters("ViBassStreamOutput - Stream Error", "The specified data could not be written to the stream", ViErrorInfo::Fatal);
		return;
	}
}
