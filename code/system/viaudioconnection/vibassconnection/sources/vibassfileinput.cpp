#include "vibassfileinput.h"

ViBassFileInputThread::ViBassFileInputThread(ViBassFileInput *parent, ViAudioBuffer *buffer, QString filePath, ViAudioMetaData *metaData)
{
	mParent = parent;
	mBuffer = buffer;
	mFilePath = filePath;
	mMetaData = metaData;
}

void ViBassFileInputThread::run()
{
	/*if(!BASS_Init(-1, 44100, 0, 0, 0))
	{
		mParent->setErrorParameters("ViBassFileInput - Device Error", "Can't initialize Bass device", ViErrorInfo::Fatal);
		return;
	}*/
	HSTREAM fileHandle = BASS_StreamCreateFile(false, mFilePath.toUtf8().data(), 0, 0, BASS_STREAM_DECODE);	
	if(fileHandle == 0)
	{
		mParent->setErrorParameters("ViBassFileInput - File Input Error", "The file(" + mFilePath + ") could not be opened", ViErrorInfo::Fatal);
		return;
	}
	//readMetaData(fileHandle);
	while(!kbHit() && BASS_ChannelIsActive(fileHandle))
	{
		char *array = new char[BUFFERSIZE];
		DWORD result = BASS_ChannelGetData(fileHandle, array, BUFFERSIZE);
		if(BASS_ErrorGetCode() != BASS_OK)
		{
			mParent->setErrorParameters("ViBassFileInput - Buffer Read Error", "The supporting buffer could not be read", ViErrorInfo::Fatal);
			return;
		}
		//auto_ptr<ViAudioBufferChunk> chunk(new ViAudioBufferChunk(array));
		ViAudioBufferChunk *i = new ViAudioBufferChunk(array);
		mBuffer->write(i, BUFFERSIZE);
	}


	if(!BASS_StreamFree(fileHandle))
	{
		mParent->setErrorParameters("ViBassFileInput - Memory Release Error", "The supporting Bass handle could not be released", ViErrorInfo::Fatal);
		return;
	}
	/*if(!BASS_Free())
	{
		mParent->setErrorParameters("ViBassFileInput - Memory Release Error", "The Bass resources could not be released", ViErrorInfo::Fatal);
		return;
	}*/



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

void ViBassFileInputThread::readMetaData(DWORD handle)
{
	BASS_CHANNELINFO info;
	if(!BASS_ChannelGetInfo(handle, &info))
	{
		mParent->setErrorParameters("ViBassFileInput - Metadata Error", "Can't retrieve the metadata info from the specified file(" + mFilePath + ")", ViErrorInfo::NonFatal);
	}
	else
	{
		mMetaData->setFrequency(info.freq);
		mMetaData->setChannels(info.chans);
	}
	QWORD length = BASS_ChannelGetLength(handle, BASS_POS_BYTE);
	if(length == -1)
	{
		mParent->setErrorParameters("ViBassFileInput - Metadata Length Error", "Can't retrieve the length(bytes) of the specified file(" + mFilePath + ")", ViErrorInfo::NonFatal);
	}
	else
	{
		mMetaData->setBytes(length);
		double time = BASS_ChannelBytes2Seconds(handle, length);
		if(time < 0)
		{
			mParent->setErrorParameters("ViBassFileInput - Metadata Length Error", "Can't retrieve the length(seconds) of the specified file(" + mFilePath + ")", ViErrorInfo::NonFatal);
		}
		else
		{
			mMetaData->setMilliseconds(time * 1000);
		}
	}
	DWORD level = BASS_ChannelGetLevel(handle);
	if(level == -1)
	{
		mParent->setErrorParameters("ViBassFileInput - Metadata Level Error", "Can't retrieve the level(peek amplitude) from the specified file(" + mFilePath + ")", ViErrorInfo::NonFatal);
	}
	else
	{
		DWORD left = LOWORD(level);
		DWORD right = HIWORD(level);
		ViAudioLevel audioLevel(left, right);
		mMetaData->setLevel(audioLevel);
	}
}

ViBassFileInput::ViBassFileInput()
	: ViFileInput()
{
	mThread = NULL;
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
if(!BASS_Init(-1, 44100, 0, 0, 0))
	{
		setErrorParameters("ViBassFileInput - Device Error", "Can't initialize Bass device", ViErrorInfo::Fatal);
		return;
	}
	
QObject::connect(mBuffer, SIGNAL(r(ViAudioBufferChunk*, int)), this, SLOT(ha(ViAudioBufferChunk*, int)), Qt::BlockingQueuedConnection);
hh = BASS_StreamCreate(44100,2, 0, STREAMPROC_PUSH, 0);
BASS_ChannelPlay(hh, false);
	mThread = new ViBassFileInputThread(this, mBuffer, "/home/visore/Desktop/a.mp3", &mMetaData);
	mThread->start();
/*while(mThread->isRunning());
BASS_Free();*/
}


void ViBassFileInput::ha(ViAudioBufferChunk* b, int size)
{
	char *c = new char[size];
	mBuffer->read(c, size);
	DWORD d = BASS_StreamPutData(hh,c, size);

}
