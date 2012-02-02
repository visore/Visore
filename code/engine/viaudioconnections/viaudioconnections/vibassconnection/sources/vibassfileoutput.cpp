#include "vibassfileoutput.h"

ViBassFileOutput::ViBassFileOutput(ViAudioBuffer *buffer, ViAudioMetaData *metaData, QString filePath)
	: ViFileOutput(buffer, metaData, filePath)
{
}

ViBassFileOutput::~ViBassFileOutput()
{
}

void ViBassFileOutput::initialize()
{
}

void ViBassFileOutput::free()
{
}

void ViBassFileOutput::start()
{
	if(mStatus == ViAudioTransmission::Running)
	{
		setErrorParameters("ViBassFileOutput - Start Error", "The file is already being written", ViErrorInfo::NonFatal);
		return;
	}
	ViAudioEncoder *encoder = ViEncoderManager::encoder(mMetaData->format())[0];
	mStatus = ViAudioTransmission::Running;
	mProcess.start(encoder->string(mFilePath));
	for(int i = 0; i < mBuffer->size(); i += FILE_CHUNK_SIZE)
	{
		ViAudioBufferChunk chunk;
		int read = mStream->read(&chunk, FILE_CHUNK_SIZE);
		mProcess.write(chunk.data(), read);
		mProcess.waitForBytesWritten();
	}
	mProcess.close();
	mStatus = ViAudioTransmission::Stopped;
}

void ViBassFileOutput::stop()
{
	if(mStatus == ViAudioTransmission::Stopped)
	{
		setErrorParameters("ViBassFileOutput - Stop Error", "The file writing was already being stopped", ViErrorInfo::NonFatal);
		return;
	}
	mStatus = ViAudioTransmission::Stopped;
	mProcess.kill();
}

void ViBassFileOutput::pause()
{
	if(mStatus == ViAudioTransmission::Paused)
	{
		setErrorParameters("ViBassFileOutput - Pause Error", "File output is not allowed to be paused", ViErrorInfo::NonFatal);
	}
}
