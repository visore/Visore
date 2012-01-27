#include "vibassfileoutput.h"

ViBassFileOutputReceiver::ViBassFileOutputReceiver(ViBassFileOutput *parent, ViAudioBuffer *buffer, HSTREAM handle)
{
	mParent = parent;
	mBuffer = buffer;
	mHandle = handle;
	ViObject::connect(mBuffer, SIGNAL(changed(int, int)), this, SLOT(changeReceived(int, int)));
}

void ViBassFileOutputReceiver::changeReceived(int startIndex, int size)
{
	ViAudioBufferChunk chunk;
	mBuffer->read(&chunk, size);
	if(BASS_StreamPutData(mHandle, chunk.data(), size) == -1)
	{
		mParent->setErrorParameters("ViBassFileOutput - Stream Error", "The specified data could not be written to the stream", ViErrorInfo::Fatal);
	}
}

ViBassFileOutput::ViBassFileOutput(ViAudioBuffer *buffer, ViAudioMetaData *metaData, QString filePath)
	: ViFileOutput(buffer, metaData, filePath)
{
	mHandle = BASS_StreamCreate(mMetaData->frequency(), mMetaData->channels(), 0, STREAMPROC_PUSH, 0);
	if(mHandle == 0)
	{
		setErrorParameters("ViBassFileOutput - Output Error", "The stream could not be opened", ViErrorInfo::Fatal);
	}
	mReceiver = new ViBassFileOutputReceiver(this, mBuffer, mHandle);
}

ViBassFileOutput::~ViBassFileOutput()
{
	if(!BASS_StreamFree(mHandle))
	{
		setErrorParameters("ViBassFileOutput - Memory Release Error", "The supporting Bass handle could not be released", ViErrorInfo::Fatal);
	}
	if(mReceiver != NULL)
	{
		delete mReceiver;
		mReceiver = NULL;
	}
}

BOOL CALLBACK recordingCallback2(HRECORD handle, const void *buffer, DWORD length, void *user)
{
	return true;
}

void ViBassFileOutput::start()
{
	ViAudioEncoder *encoder = ViEncoderManager::encoder(mMetaData->format())[0];
	if(encoder == NULL)
	{
		cout << "kak man!!!!" << endl;
		return;
	}
	QString execution = encoder->string(mFilePath);



BASS_RecordInit(-1); // initialize default recording device
HRECORD record=BASS_RecordStart(44100, 2, 0, &recordingCallback2, 0);
execution = "lame --alt-preset standard - bass.mp3";ViLogger::debug("*"+execution+"*");
//BASS_Encode_Start(mHandle, "/home/visore/Desktop/testrec.wav", BASS_ENCODE_PCM, NULL, 0);
	BASS_Encode_Start(record, "execution", 0, NULL, 0);
cout<<"error: "<<BASS_ErrorGetCode()<<endl;



	if(mStatus == ViAudioTransmission::Running)
	{
		setErrorParameters("ViBassFileOutput - Start Error", "The file is already being written", ViErrorInfo::NonFatal);
		return;
	}
	else if(!BASS_ChannelPlay(mHandle, false))
	{
		setErrorParameters("ViBassFileOutput - Start Error", "The palyback stream could not be started", ViErrorInfo::MediumFatal);
		return;
	}
	mStatus = ViAudioTransmission::Running;

}

void ViBassFileOutput::stop()
{
	if(mStatus == ViAudioTransmission::Stopped)
	{
		setErrorParameters("ViBassFileOutput - Stop Error", "The file writing was already being stopped", ViErrorInfo::NonFatal);
		return;
	}
	else if(!BASS_ChannelStop(mHandle))
	{
		setErrorParameters("ViBassFileOutput - Stop Error", "The palyback stream could not be stopped", ViErrorInfo::MediumFatal);
		return;
	}
	mStatus = ViAudioTransmission::Stopped;
	mBuffer->restartRead();
}

void ViBassFileOutput::pause()
{
}
