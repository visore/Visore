#include "viprocessorthread.h"
#include "vipcmconverter.h"

#define DEFAULT_WINDOW_SIZE 2048

ViProcessorThread::ViProcessorThread(ViProcessorThread::Execution execution, bool notify)
	: QThread()
{
	setExecution(execution);
	setNotification(notify);
	mWindowSize = DEFAULT_WINDOW_SIZE;
	mReadStream = NULL;
	mWriteStream = NULL;
}

void ViProcessorThread::setNotification(bool notify)
{
	if(notify)
	{
		ViProcessorThread::execute = &ViProcessorThread::notifyExecute;
	}
	else
	{
		ViProcessorThread::execute = &ViProcessorThread::normalExecute;
	}
}

void ViProcessorThread::setExecution(ViProcessorThread::Execution execution)
{
	if(execution == ViProcessorThread::Observation)
	{
		ViProcessorThread::process = &ViProcessorThread::observationProcess;
	}
	else if(execution == ViProcessorThread::Modification)
	{
		ViProcessorThread::process = &ViProcessorThread::modificationProcess;
	}
	else if(execution == ViProcessorThread::ObservationModification)
	{
		ViProcessorThread::process = &ViProcessorThread::observationModificationProcess;
	}
}

void ViProcessorThread::setWindowSize(int windowSize)
{
	mWindowSize = windowSize;
	updateChunks();
	updateProcessors();
}

void ViProcessorThread::setStream(ViAudioConnection::Direction direction, ViAudioBufferStream *stream)
{
	int sampleSize;
	if(direction == ViAudioConnection::Input)
	{
		mReadStream = stream;
		sampleSize = mReadStream->buffer()->format().sampleSize();
		if(sampleSize == 8)
		{
			pcmToReal = &ViPcmConverter<double>::pcmToReal8;
		}
		else if(sampleSize == 16)
		{
			pcmToReal = &ViPcmConverter<double>::pcmToReal16;
		}
		else if(sampleSize == 24)
		{
			pcmToReal = &ViPcmConverter<double>::pcmToReal24;
		}
		else if(sampleSize == 32)
		{
			pcmToReal = &ViPcmConverter<double>::pcmToReal32;
		}
	}
	else
	{
		mWriteStream = stream;
		sampleSize = mWriteStream->buffer()->format().sampleSize();
		if(sampleSize == 8)
		{
			realToPcm = &ViPcmConverter<double>::realToPcm8;
		}
		else if(sampleSize == 16)
		{
			realToPcm = &ViPcmConverter<double>::realToPcm16;
		}
		else if(sampleSize == 24)
		{
			realToPcm = &ViPcmConverter<double>::realToPcm24;
		}
		else if(sampleSize == 32)
		{
			realToPcm = &ViPcmConverter<double>::realToPcm32;
		}
	}
	updateChunks();
	updateProcessors();
}

bool ViProcessorThread::attach(ViAudioConnection::Direction direction, ViProcessor *processor)
{
	if(mProcessors.add(direction, processor))
	{
		updateProcessors();
	}
	return false;
}

void ViProcessorThread::updateProcessors()
{
	if(mReadStream != NULL)
	{
		ViAudioFormat format = mReadStream->buffer()->format();
		QList<ViProcessor*> processors = mProcessors.processors();
		for(int i = 0; i < processors.size(); ++i)
		{
			processors[i]->setWindowSize(mWindowSize);
			processors[i]->setFormat(format);
		}
	}
}

void ViProcessorThread::updateChunks()
{
	if(mReadStream != NULL)
	{
		mInputChunk.resize(mWindowSize * (mReadStream->buffer()->format().sampleSize() / 8));
	}
	mRealChunk.resize(mWindowSize);
	if(mWriteStream != NULL)
	{
		mOutputChunk.resize(mWindowSize * (mWriteStream->buffer()->format().sampleSize() / 8));
	}
}

void ViProcessorThread::removeAll()
{
	mProcessors.removeAll();
}

void ViProcessorThread::run()
{
	(this->*execute)();
}

void ViProcessorThread::notifyExecute()
{
	emit progressed(0);
	qint64 size;
	qint64 progress = 0;
	do
	{
		size = mReadStream->bufferSize();
		mInputChunk.setSize(mReadStream->read(&mInputChunk));
		if(mInputChunk.size() > 0)
		{
			(this->*process)();
			progress += mInputChunk.size();
			emit progressed((progress * 100) / size);
		}
	}
	while(mInputChunk.size() > 0);
	emit progressed(100);
	emit finished();
}

void ViProcessorThread::normalExecute()
{
	do
	{
		mInputChunk.setSize(mReadStream->read(&mInputChunk));
		if(mInputChunk.size() > 0)
		{
			(this->*process)();
		}
	}
	while(mInputChunk.size() > 0);
}

void ViProcessorThread::observationProcess()
{
	mRealChunk.setSize(pcmToReal(mInputChunk.data(), mRealChunk.data(), mInputChunk.size()));
	mProcessors.observeInput(&mRealChunk);
	mProcessors.observeOutput(&mRealChunk);
}

void ViProcessorThread::modificationProcess()
{
	mRealChunk.setSize(pcmToReal(mInputChunk.data(), mRealChunk.data(), mInputChunk.size()));
	mProcessors.manipulateInput(&mRealChunk);
	mOutputChunk.setSize(realToPcm(mRealChunk.data(), mOutputChunk.data(), mRealChunk.size()));
	mWriteStream->write(&mOutputChunk);
}

void ViProcessorThread::observationModificationProcess()
{
	mRealChunk.setSize(pcmToReal(mInputChunk.data(), mRealChunk.data(), mInputChunk.size()));
	mProcessors.observeInput(&mRealChunk);
	mProcessors.manipulateInput(&mRealChunk);
	mProcessors.observeOutput(&mRealChunk);
	mOutputChunk.setSize(realToPcm(mRealChunk.data(), mOutputChunk.data(), mRealChunk.size()));
	mWriteStream->write(&mOutputChunk);
}
