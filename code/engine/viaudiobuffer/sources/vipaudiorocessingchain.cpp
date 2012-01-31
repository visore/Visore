#include "viaudioprocessingchain.h"

ViAudioProcessingChain::ViAudioProcessingChain::ViAudioProcessingChain()
{
	mOriginalBuffer = new ViAudioBuffer();
	mCorrectedBuffer = new ViAudioBuffer();
	mOriginalReadStream = mOriginalBuffer->createReadStream();
	mCorrectedWriteStream = mCorrectedBuffer->createWriteStream();
	ViObject::connect(mOriginalBuffer, SIGNAL(changed(int, int)), this, SLOT(originalBufferChanged(int, int)));
	ViObject::connect(mCorrectedBuffer, SIGNAL(changed(int, int)), this, SLOT(correctedBufferChanged(int, int)));
}

ViAudioProcessingChain::~ViAudioProcessingChain()
{
	if(mOriginalBuffer != NULL)
	{
		ViObject::disconnect(mOriginalBuffer, SIGNAL(changed(int, int)), this, SLOT(originalBufferChanged(int, int)));
		delete mOriginalBuffer;
		mOriginalBuffer = NULL;
	}
	if(mCorrectedBuffer != NULL)
	{
		ViObject::disconnect(mCorrectedBuffer, SIGNAL(changed(int, int)), this, SLOT(correctedBufferChanged(int, int)));
		delete mCorrectedBuffer;
		mCorrectedBuffer = NULL;
	}
}

ViAudioBuffer* ViAudioProcessingChain::originalBuffer()
{
	return mOriginalBuffer;
}

ViAudioBuffer* ViAudioProcessingChain::correctedBuffer()
{
	return mCorrectedBuffer;
}

void ViAudioProcessingChain::originalBufferChanged(int startIndex, int size)
{
	ViAudioBufferChunk chunk;
	mOriginalReadStream->read(&chunk, size);
	mCorrectedWriteStream->write(&chunk, size);
}

void ViAudioProcessingChain::correctedBufferChanged(int startIndex, int size)
{
	//emit changeFinished(startIndex, size);
}

void ViAudioProcessingChain::attachInput(ViAudioInput *input)
{
	mInput = input;
}

void ViAudioProcessingChain::attachStreamOutput(ViAudioOutput *output)
{
	mStreamOutput = output;
}

void ViAudioProcessingChain::attachFileOutput(ViAudioOutput *output)
{
	mFileOutput = output;
}

void ViAudioProcessingChain::reset()
{
	mOriginalBuffer->clear();
	mCorrectedBuffer->clear();
}
