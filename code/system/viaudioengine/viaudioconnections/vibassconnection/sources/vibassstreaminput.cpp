#include "vibassstreaminput.h"

ViBassStreamInputThread::ViBassStreamInputThread(ViBassStreamInput *parent, ViAudioBuffer *buffer, ViAudioMetaData *metaData, ViAudioDevice *device)
{
	mParent = parent;
	mBuffer = buffer;
	mMetaData = metaData;
	mDevice = device;
}

void ViBassStreamInputThread::run()
{
	if(!BASS_RecordInit(-1))
	{
		mParent->setErrorParameters("ViBassStreamInput - Initializing Error", "Could not initialize the Bass recording device", ViErrorInfo::Fatal);
		return;
	}
	HRECORD recordHandle = BASS_RecordStart(44100, 2, 0, 0, 0);
	if(!recordHandle)
	{
		mParent->setErrorParameters("ViBassStreamInput - Stream Input Error", "The stream could not be opened", ViErrorInfo::Fatal);
		return;
	}
	while(true/*BASS_ChannelIsActive(recordHandle)*/)
	{
		DWORD len = BASS_ChannelGetData(recordHandle, NULL, BASS_DATA_AVAILABLE);
		char *array = new char[len];
		DWORD result = BASS_ChannelGetData(recordHandle, array, len);
		if(BASS_ErrorGetCode() != BASS_OK)
		{
			mParent->setErrorParameters("ViBassStreamInput - Buffer Read Error", "The supporting buffer could not be read", ViErrorInfo::Fatal);
			return;
		}
		ViAudioBufferChunk chunk(array);
		mBuffer->write(&chunk, len);
	}
	/*if(!BASS_StreamFree(recordHandle))
	{
		mParent->setErrorParameters("ViBassStreamInput - Memory Release Error", "The supporting Bass handle could not be released", ViErrorInfo::Fatal);
		return;
	}
	if(!BASS_RecordFree())
	{
		mParent->setErrorParameters("ViBassStreamInput - Recording Memory Release Error", "The supporting Bass recording device could not be released", ViErrorInfo::Fatal);
		return;
	}*/
}

ViBassStreamInput::ViBassStreamInput(ViAudioBuffer *buffer, ViAudioMetaData *metaData, ViAudioDevice *device)
	: ViStreamInput(buffer, metaData, device)
{
	mThread = new ViBassStreamInputThread(this, mBuffer, mMetaData, mDevice);
}

ViBassStreamInput::~ViBassStreamInput()
{
	if(mThread != NULL)
	{
		if(mThread->isRunning())
		{
			mThread->quit();
		}
		delete mThread;
		mThread = NULL;
	}
}

void ViBassStreamInput::start()
{
	mThread->start();
}
