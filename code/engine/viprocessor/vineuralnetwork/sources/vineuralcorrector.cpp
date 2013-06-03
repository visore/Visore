#include <vineuralcorrector.h>
#include <visamplechanneler.h>
#include <viscaler.h>
#include <qmath.h>

#define TRAINER_LEFT_SAMPLES 2
#define TRAINER_RIGHT_SAMPLES 2

#define MAX_THREAD_CHUNKS 4096
#define MIN_THREAD_CHUNKS 2048

ViNeuralCorrectorThread::ViNeuralCorrectorThread(ViNeuralNetwork *network, ViTrainer *trainer, ViTargetProvider *provider)
	: QThread()
{
	mNetwork = network;
	mTrainer = trainer;
	mProvider = provider;
	mTrainer->setNetwork(mNetwork);

	mStop = false;

	mDataOffset = 0;
	mTargetLeftOffset = 0;
	mTargetRightOffset = 0;

	mOutputSize = 0;
	mOutputSample = 0;
	mOutput = NULL;

	mLeftTargetData.resize(TRAINER_LEFT_SAMPLES);
	mRightTargetData.resize(TRAINER_RIGHT_SAMPLES);
}

ViNeuralCorrectorThread::~ViNeuralCorrectorThread()
{
	delete mNetwork;
	mNetwork = NULL;
	delete mTrainer;
	mTrainer = NULL;
	delete mProvider;
	mProvider = NULL;
	viDeleteAll(mData);
}

bool ViNeuralCorrectorThread::setData(ViSampleChunk *data)
{
	QMutexLocker locker(&mMutex);
	if(mData.size() > MAX_THREAD_CHUNKS)
	{
		locker.unlock();
		mWaitCondition.wait(&mWaitMutex);
		locker.relock();
	}
	mData.enqueue(data);
	return true;
}

void ViNeuralCorrectorThread::setOffsets(int data, int targetLeft, int targetRight)
{
	mDataOffset = data;
	mTargetLeftOffset = targetLeft;
	mTargetRightOffset = targetRight;
}

void ViNeuralCorrectorThread::setOutputSize(int size)
{
	QMutexLocker locker(&mOutputMutex);
	mOutputSize = size;
	if(mOutput != NULL)
	{
		delete mOutput;
	}
	mOutput = new ViSampleChunk(mOutputSize);
}

ViSampleChunk* ViNeuralCorrectorThread::output()
{
	QMutexLocker locker(&mOutputMutex);
	if(mOutputs.isEmpty())
	{
		return NULL;
	}
	return mOutputs.dequeue();
}

bool ViNeuralCorrectorThread::hasOutput()
{
	QMutexLocker locker(&mOutputMutex);
	return !mOutputs.isEmpty();
}

ViNeuralNetwork* ViNeuralCorrectorThread::network()
{
	return ViNeuralCorrectorThread::mNetwork;
}

void ViNeuralCorrectorThread::run()
{			
	while(true)
	{
		mMutex.lock();
		bool noData = mData.isEmpty();
		mMutex.unlock();
		while(!noData)
		{
			mMutex.lock();
			ViSampleChunk *data = mData.dequeue();
			mMutex.unlock();
			int inputs = mNetwork->inputCount(false);
			for(int i = 0; i < inputs; ++i)
			{
				mNetwork->setInput(i, data->at(i + mDataOffset));
			}

			for(int i = 0; i < TRAINER_LEFT_SAMPLES; ++i)
			{
				mLeftTargetData[i] = data->at(mTargetLeftOffset + i);
			}

			for(int i = 0; i < TRAINER_RIGHT_SAMPLES; ++i)
			{
				mRightTargetData[i] = data->at(mTargetRightOffset + i);
			}

			mProvider->setData(&mLeftTargetData, &mRightTargetData);
			mTrainer->setTargetValues({mProvider->calculate()});
			for(int i = 0; i < 1; ++i)
			{
				mTrainer->trainSingle();
			}

			delete data;

			mOutputMutex.lock();
			if(mOutputSample == mOutputSize - 1)
			{
				mOutputs.append(mOutput);
				mOutputMutex.unlock();
				emit outputAvailable();
				mOutputMutex.lock();
				mOutput = new ViSampleChunk(mOutputSize);
				mOutputSample = 0;
			}
			mOutput->at(mOutputSample) = ViScaler::scale(mNetwork->output(), 0, 1, -1, 1);
			++mOutputSample;
			mOutputMutex.unlock();
			
			if(mData.size() < MIN_THREAD_CHUNKS)
			{
				mWaitCondition.wakeAll();
			}

			mMutex.lock();
			noData = mData.isEmpty();
			mMutex.unlock();
		}

		mMutex.lock();
		if(mStop)
		{
			mOutputMutex.lock();
			mOutput->resize(mOutputSample + 1);
			mOutputs.append(mOutput);
			mOutputMutex.unlock();
			emit outputAvailable();
			break;
		}
		mMutex.unlock();

		msleep(50);
	}
	mMutex.unlock();
}

void ViNeuralCorrectorThread::stop()
{
	QMutexLocker locker(&mMutex);
	mStop = true;
}

ViNeuralCorrector::ViNeuralCorrector()
	: ViModifyProcessor(false) //Make sure that the processor is not automatically writing the sample again
{
	mNetwork = NULL;
	mTrainer = NULL;
	mProvider = NULL;

	mChannels = 1;
	mMinimumSamples = 0;
	mLeftSamples = 0;
	mWriteSamples = 0;
	mDataOffset = 0;
	mTargetLeftOffset = 0;
	mTargetRightOffset = 0;
	mSeparateChannels = true;
}

ViNeuralCorrector::ViNeuralCorrector(ViNeuralNetwork *network, ViTrainer *trainer, ViTargetProvider *provider)
	: ViModifyProcessor(false) //Make sure that the processor is not automatically writing the sample again
{
	mNetwork = network;
	mTrainer = trainer;
	mProvider = provider;

	mChannels = 1;
	mMinimumSamples = 0;
	mLeftSamples = 0;
	mWriteSamples = 0;
	mDataOffset = 0;
	mTargetLeftOffset = 0;
	mTargetRightOffset = 0;
	mSeparateChannels = true;
}

ViNeuralCorrector::~ViNeuralCorrector()
{
	viDeleteAll(mThreads);
	if(mNetwork != NULL)
	{
		delete mNetwork;
		mNetwork = NULL;
	}
	if(mTrainer != NULL)
	{
		delete mTrainer;
		mTrainer = NULL;
	}
	if(mProvider != NULL)
	{
		delete mProvider;
		mProvider = NULL;
	}
}

void ViNeuralCorrector::enableSeparateChannels(bool enable)
{
	mSeparateChannels = enable;
}

void ViNeuralCorrector::disableSeparateChannels(bool disable)
{
	mSeparateChannels = !disable;
}

bool ViNeuralCorrector::writeOutput()
{
	for(int c = 0; c < mChannels; ++c)
	{
		if(!mThreads[c]->hasOutput())
		{
			return false;
		}
	}
	QList<ViSampleChunk*> channels;
	for(int c = 0; c < mChannels; ++c)
	{
		channels.append(mThreads[c]->output());
	}
	ViSampleChunk *merged = ViSampleChanneler<qreal>::merge(channels);
	viDeleteAll(channels);
	write(*merged);
	delete merged;
	return true;
}

void ViNeuralCorrector::initialize()
{
	if(mNetwork == NULL)
	{
		LOG("No neural network was specified.", QtCriticalMsg);
		return;
	}
	if(mTrainer == NULL)
	{
		LOG("No trainer was specified.", QtCriticalMsg);
		return;
	}
	if(mProvider == NULL)
	{
		LOG("No target provider was specified.", QtCriticalMsg);
		return;
	}

	viDeleteAll(mThreads);

	mCurrentWriteChannel = 0;
	if(mSeparateChannels)
	{
		mChannels = object()->format(type()).channelCount();
		executePointer = &ViNeuralCorrector::executeWithChannels;
	}
	else
	{
		mChannels = 1;
		executePointer = &ViNeuralCorrector::executeWithoutChannels;
	}

	mReadBuffer.clear();
	mFirstWrite = true;

	int inputSamples = mNetwork->inputCount(false);
	if(TRAINER_LEFT_SAMPLES > inputSamples)
	{
		mDataOffset = TRAINER_LEFT_SAMPLES - inputSamples;
		mLeftSamples = TRAINER_LEFT_SAMPLES;
		mTargetLeftOffset = 0;
	}
	else
	{
		mDataOffset = 0;
		mLeftSamples = inputSamples;
		mTargetLeftOffset = inputSamples - TRAINER_LEFT_SAMPLES;
	}
	mTargetRightOffset = mLeftSamples + 1;
	mMinimumSamples = (mLeftSamples + 1 + TRAINER_RIGHT_SAMPLES) * mChannels;
	mWriteSamples = qFloor(sampleCount() / qreal(mChannels)) * mChannels;
	
	for(int i = 0; i < mChannels; ++i)
	{
		ViNeuralCorrectorThread *thread = new ViNeuralCorrectorThread(mNetwork->clone(), mTrainer->clone(), mProvider->clone());
		thread->setOffsets(mDataOffset, mTargetLeftOffset, mTargetRightOffset);
		thread->setOutputSize(mWriteSamples);
		QObject::connect(thread, SIGNAL(outputAvailable()), this, SLOT(writeOutput()));
		mThreads.append(thread);
	}

	for(int c = 0; c < mChannels; ++c)
	{
		mThreads[c]->start();
	}
}

void ViNeuralCorrector::executeWithChannels()
{
	ViSampleChunk &newSamples = samples();
	for(int i = 0; i < newSamples.size(); ++i)
	{
		mReadBuffer.append(ViScaler::scale(newSamples[i], -1, 1, 0, 1));
	}

	//Ensures that the first window of samples is written immediatly, since they can't be corrected
	if(mFirstWrite && mReadBuffer.size() >= mLeftSamples * mChannels)
	{
		ViSampleChunk buffer;
		mFirstWrite = false;
		for(int i = 0; i < mLeftSamples * mChannels; ++i)
		{
			buffer.append(ViScaler::scale(mReadBuffer[i], 0, 1, -1, 1));
		}
		write(buffer);
	}

	bool pause = false;
	while(mReadBuffer.size() >= mMinimumSamples)
	{
		QVector<qreal> subSamples = mReadBuffer.mid(0, mMinimumSamples);
		QList<ViSampleChunk*> channels = ViSampleChanneler<qreal>::split(subSamples.data(), subSamples.size(), mChannels);
		mReadBuffer.remove(0, mChannels);

		for(int c = 0; c < mChannels; ++c)
		{
			mThreads[c]->setData(channels[c]);
		}
	}
}

void ViNeuralCorrector::executeWithoutChannels()
{
	ViSampleChunk &newSamples = samples();
	for(int i = 0; i < newSamples.size(); ++i)
	{
		mReadBuffer.append(ViScaler::scale(newSamples[i], -1, 1, 0, 1));
	}

	//Ensures that the first window of samples is written immediatly, since they can't be corrected
	if(mFirstWrite && mReadBuffer.size() >= mLeftSamples)
	{
		ViSampleChunk buffer;
		mFirstWrite = false;
		for(int i = 0; i < mLeftSamples; ++i)
		{
			buffer.append(ViScaler::scale(mReadBuffer[i], 0, 1, 1, -1));
		}
		write(buffer);
	}

	while(mReadBuffer.size() >= mMinimumSamples)
	{
		ViSampleChunk wrapper(mReadBuffer.data(), mMinimumSamples, false);
		mThreads[0]->setData(&wrapper);
		mThreads[0]->run();
		mReadBuffer.pop_front();
	}
}

void ViNeuralCorrector::execute()
{
	(this->*executePointer)();
}

void ViNeuralCorrector::finalize()
{
	for(int c = 0; c < mChannels; ++c)
	{
		mThreads[c]->stop();
	}

	for(int c = 0; c < mChannels; ++c)
	{
		while(mThreads[c]->isRunning());
	}

	//Write the remaining couple of samples
	while(writeOutput());

	viDeleteAll(mThreads);
}
