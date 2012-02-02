#include "vibassfileinput.h"

ViBassFileInputThread::ViBassFileInputThread(ViBassFileInput *parent, ViAudioBufferStream *stream, ViAudioMetaData *metaData)
{
	mParent = parent;
	mStream = stream;
	mMetaData = metaData;
}

ViBassFileInputThread::~ViBassFileInputThread()
{
	if(!BASS_StreamFree(mFileHandle))
	{
		mParent->setErrorParameters("ViBassFileInput - Memory Release Error", "The supporting Bass handle could not be released", ViErrorInfo::Fatal);
	}
}

void ViBassFileInputThread::setFilePath(QString filePath)
{
	BASS_StreamFree(mFileHandle);
	mFilePath = filePath;
	mFileHandle = BASS_StreamCreateFile(false, mFilePath.toUtf8().data(), 0, 0, BASS_STREAM_DECODE);	
	if(!mFileHandle)
	{
		mParent->setErrorParameters("ViBassFileInput - File Input Error", "The file(" + mFilePath + ") could not be opened", ViErrorInfo::Fatal);
	}
	readMetaData();
}

void ViBassFileInputThread::run()
{
	mPaused = false;
	int bufferSize = mStream->bufferHeadStart();
	bufferSize = pow(2, ceil(log(bufferSize) / log(2))); //Make sure it is a power of 2
	while(BASS_ChannelIsActive(mFileHandle))
	{
		char *array = new char[bufferSize];
		DWORD result = BASS_ChannelGetData(mFileHandle, array, bufferSize);
		if(BASS_ErrorGetCode() != BASS_OK)
		{
			mParent->setErrorParameters("ViBassFileInput - Buffer Read Error", "The supporting buffer could not be read", ViErrorInfo::Fatal);
			return;
		}
		ViAudioBufferChunk chunk(array);
		mStream->write(&chunk, bufferSize);
		if(mPaused)
		{
			return;
		}
	}
}

void ViBassFileInputThread::pause()
{
	mPaused = true;
}

void ViBassFileInputThread::readMetaData()
{
	BASS_CHANNELINFO info;
	if(!BASS_ChannelGetInfo(mFileHandle, &info))
	{
		mParent->setErrorParameters("ViBassFileInput - Metadata Error", "Can't retrieve the metadata info from the specified file(" + mFilePath + ")", ViErrorInfo::NonFatal);
	}
	else
	{
		mMetaData->setFrequency(info.freq);
		mMetaData->setChannels(info.chans);
		if(info.flags == BASS_SAMPLE_FLOAT)
		{
			mMetaData->setBitDepth(32);
		}
		else if(info.flags == BASS_SAMPLE_8BITS)
		{
			mMetaData->setBitDepth(8);
		}
		else
		{
			mMetaData->setBitDepth(16);
		}
	}
	/*QWORD length = BASS_ChannelGetLength(mFileHandle, BASS_POS_BYTE);
	if(length == -1)
	{
		mParent->setErrorParameters("ViBassFileInput - Metadata Length Error", "Can't retrieve the length(bytes) of the specified file(" + mFilePath + ")", ViErrorInfo::NonFatal);
	}
	else
	{
		mMetaData->setBytes(length);
		double time = BASS_ChannelBytes2Seconds(mFileHandle, length);
		if(time < 0)
		{
			mParent->setErrorParameters("ViBassFileInput - Metadata Length Error", "Can't retrieve the length(seconds) of the specified file(" + mFilePath + ")", ViErrorInfo::NonFatal);
		}
		else
		{
			mMetaData->setMilliseconds(time * 1000);
		}
	}*/
}

ViBassFileInput::ViBassFileInput(ViAudioBuffer *buffer, ViAudioMetaData *metaData, QString filePath)
	: ViFileInput(buffer, metaData, filePath)
{
	mThread = NULL;
}

ViBassFileInput::~ViBassFileInput()
{
	free();
}

void ViBassFileInput::initialize()
{
	free();
	mThread = new ViBassFileInputThread(this, mStream, mMetaData);
	mThread->setFilePath(mFilePath);
}

void ViBassFileInput::free()
{
	if(mThread != NULL)
	{
		if(mThread->isRunning())
		{
			mThread->terminate();
		}
		delete mThread;
		mThread = NULL;
	}
}

void ViBassFileInput::start()
{
	if(mStatus == ViAudioTransmission::Running)
	{
		setErrorParameters("ViBassFileInput - Start Error", "The file is already playing", ViErrorInfo::NonFatal);
		return;
	}
	else if(mStatus == ViAudioTransmission::Stopped)
	{
		mThread = new ViBassFileInputThread(this, mStream, mMetaData);
	}
	mStatus = ViAudioTransmission::Running;
	mThread->start();	
}

void ViBassFileInput::stop()
{
	if(mStatus == ViAudioTransmission::Stopped)
	{
		setErrorParameters("ViBassFileInput - Stop Error", "The input is already stopped", ViErrorInfo::NonFatal);
		return;
	}
	mStatus = ViAudioTransmission::Stopped;
	mThread->terminate();
	while(!mThread->isFinished());
	delete mThread;
	mThread = NULL;
	mStream->restart();
}

void ViBassFileInput::pause()
{
	if(mStatus == ViAudioTransmission::Paused)
	{
		setErrorParameters("ViBassFileInput - Pause Error", "The input is already paused", ViErrorInfo::NonFatal);
		return;
	}
	mStatus = ViAudioTransmission::Paused;
	mThread->pause();
}
