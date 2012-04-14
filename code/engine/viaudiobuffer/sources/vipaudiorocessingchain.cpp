#include "viaudioprocessingchain.h"

ViAudioProcessingChain::ViAudioProcessingChain::ViAudioProcessingChain()
{
	mOriginalBuffer = new ViAudioBuffer();
	mCorrectedBuffer = new ViAudioBuffer();
	mOriginalReadStream = mOriginalBuffer->createReadStream();
	mCorrectedWriteStream = mCorrectedBuffer->createWriteStream();
	ViObject::connectDirect(mOriginalBuffer, SIGNAL(changed(int)), this, SLOT(originalBufferChanged(int)));
	ViObject::connectDirect(mCorrectedBuffer, SIGNAL(changed(int)), this, SLOT(correctedBufferChanged(int)));
}

ViAudioProcessingChain::~ViAudioProcessingChain()
{
	if(mOriginalBuffer != NULL)
	{
		ViObject::disconnect(mOriginalBuffer, SIGNAL(changed(int)), this, SLOT(originalBufferChanged(int)));
		delete mOriginalBuffer;
		mOriginalBuffer = NULL;
	}
	if(mCorrectedBuffer != NULL)
	{
		ViObject::disconnect(mCorrectedBuffer, SIGNAL(changed(int)), this, SLOT(correctedBufferChanged(int)));
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

void ViAudioProcessingChain::originalBufferChanged(int size)
{
int id = -1;
	++id;
	bool lastParallel = false;
	if(id < mOriginalProcessors.size())
	{
		QList<ViProcessor*> processors;
		lastParallel = mOriginalProcessors.processors(id, &processors);
		for(int i = 0; i < processors.size(); ++i)
		{
			processors[i]->update(size);
		}
	}
	if(lastParallel || id >= mOriginalProcessors.size())
	{
		ViAudioBufferChunk chunk;
		mOriginalReadStream->read(&chunk, size);
		mCorrectedWriteStream->write(&chunk, size);
	}
}

void ViAudioProcessingChain::correctedBufferChanged(int size)
{
	//emit changeFinished(startIndex, size);
	mStreamOutput->bufferChanged(size);
}

void ViAudioProcessingChain::attachInput(ViAudioInput *input)
{
	mInput = input;
	mInput->initialize();
}

void ViAudioProcessingChain::attachStreamOutput(ViAudioOutput *output)
{
	mStreamOutput = output;
	mStreamOutput->initialize();
}

void ViAudioProcessingChain::attachFileOutput(ViAudioOutput *output)
{
	mFileOutput = output;
	mFileOutput->initialize();
}

void ViAudioProcessingChain::attachOriginalProcessor(ViProcessor *processor, ViProcessorList::ViProcessorExecution execution)
{
	processor->initialize(mOriginalBuffer);
	mOriginalProcessors.append(processor, execution);
}

void ViAudioProcessingChain::attachCorrectedProcessor(ViProcessor *processor, ViProcessorList::ViProcessorExecution execution)
{
	processor->initialize(mCorrectedBuffer);
	mCorrectedProcessors.append(processor, execution);
}

void ViAudioProcessingChain::reset()
{
	mOriginalBuffer->clear();
	mCorrectedBuffer->clear();
}
