#include "vibassfileinput.h"

ViBassFileInputThread::ViBassFileInputThread(ViBassFileInput *parent, ViAudioBuffer *buffer, ViAudioMetaData *metaData, QString filePath)
{
	mParent = parent;
	mBuffer = buffer;
	mMetaData = metaData;
	mFilePath = filePath;
	mFileHandle = BASS_StreamCreateFile(false, mFilePath.toUtf8().data(), 0, 0, BASS_STREAM_DECODE);	
	if(!mFileHandle)
	{
		mParent->setErrorParameters("ViBassFileInput - File Input Error", "The file(" + mFilePath + ") could not be opened", ViErrorInfo::Fatal);
		return;
	}
}

ViBassFileInputThread::~ViBassFileInputThread()
{
	if(!BASS_StreamFree(mFileHandle))
	{
		mParent->setErrorParameters("ViBassFileInput - Memory Release Error", "The supporting Bass handle could not be released", ViErrorInfo::Fatal);
		return;
	}
}

void ViBassFileInputThread::run()
{
	int bufferSize = mBuffer->bufferHeadStart();
	bufferSize = pow(2, ceil(log(bufferSize) / log(2))); //Make sure it is a power of 2
	while(!kbHit() && BASS_ChannelIsActive(mFileHandle))
	{
		char *array = new char[bufferSize];
		DWORD result = BASS_ChannelGetData(mFileHandle, array, bufferSize);
		if(BASS_ErrorGetCode() != BASS_OK)
		{
			mParent->setErrorParameters("ViBassFileInput - Buffer Read Error", "The supporting buffer could not be read", ViErrorInfo::Fatal);
			return;
		}
		ViAudioBufferChunk chunk(array);
		mBuffer->write(&chunk, bufferSize);
	}
}

int ViBassFileInputThread::kbHit()
{
	int result;
	fd_set rfds;
	struct timeval tv;
	struct termios term, oterm;
	tcgetattr(0, &oterm);
	memcpy(&term, &oterm, sizeof(term));
	cfmakeraw(&term);
	tcsetattr(0, TCSANOW, &term);
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);
	tv.tv_sec = tv.tv_usec = 0;
	result = select(1, &rfds, NULL, NULL, &tv);
	tcsetattr(0, TCSANOW, &oterm);
	return result;
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
	}
	QWORD length = BASS_ChannelGetLength(mFileHandle, BASS_POS_BYTE);
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
	}
}

ViBassFileInput::ViBassFileInput(ViAudioBuffer *buffer, ViAudioMetaData *metaData, QString filePath)
	: ViFileInput(buffer, metaData, filePath)
{
	mThread = new ViBassFileInputThread(this, mBuffer, mMetaData, mFilePath);
	mThread->readMetaData();
}

ViBassFileInput::~ViBassFileInput()
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

void ViBassFileInput::start()
{
	mThread->start();
}
