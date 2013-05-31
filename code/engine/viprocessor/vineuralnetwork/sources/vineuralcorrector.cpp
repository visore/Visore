#include <vineuralcorrector.h>
#include <visamplechanneler.h>
#include <viscaler.h>
#include <qmath.h>

#define TRAINER_LEFT_SAMPLES 2
#define TRAINER_RIGHT_SAMPLES 2

ViNeuralCorrectorThread::ViNeuralCorrectorThread(ViNeuralNetwork *network, ViTrainer *trainer, ViTargetProvider *provider)
	: QThread()
{
	mNetwork = network;
	mTrainer = trainer;
	mProvider = provider;

	mData = NULL;

	mDataOffset = 0;
	mTargetLeftOffset = 0;
	mTargetRightOffset = 0;

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
}

void ViNeuralCorrectorThread::setData(ViSampleChunk *data)
{
	mData = data;
}

void ViNeuralCorrectorThread::setOffsets(int data, int targetLeft, int targetRight)
{
	mDataOffset = data;
	mTargetLeftOffset = targetLeft;
	mTargetRightOffset = targetRight;
}

ViNeuralNetwork* ViNeuralCorrectorThread::network()
{
	return ViNeuralCorrectorThread::mNetwork;
}

void ViNeuralCorrectorThread::run()
{
	int inputs = mNetwork->inputCount();
	for(int i = 0; i < inputs; ++i)
	{
		mNetwork->setInput(i, mData->at(i + mDataOffset));
	}
	mNetwork->setInput(inputs, mNetwork->output());

	for(int i = 0; i < TRAINER_LEFT_SAMPLES; ++i)
	{
		mLeftTargetData[i] = mData->at(mTargetLeftOffset + i);
	}

	for(int i = 0; i < TRAINER_RIGHT_SAMPLES; ++i)
	{
		mRightTargetData[i] = mData->at(mTargetRightOffset + i);
	}

	mProvider->setData(&mLeftTargetData, &mRightTargetData);
	mTrainer->setTargetValues({mProvider->calculate()});
	for(int i = 0; i < 1; ++i)
	{
		mTrainer->trainSingle();
	}
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

	int mChannels = 1;
	if(mSeparateChannels)
	{
		mChannels = object()->format(type()).channelCount();
		executePointer = &ViNeuralCorrector::executeWithChannels;
	}
	else
	{
		executePointer = &ViNeuralCorrector::executeWithoutChannels;
	}

	mReadBuffer.clear();
	mFirstWrite = true;

	int inputSamples = mNetwork->inputCount();
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
		mTrainer->setNetwork(mNetwork);
		ViNeuralCorrectorThread *thread = new ViNeuralCorrectorThread(mNetwork->clone(), mTrainer->clone(), mProvider->clone());
		thread->setOffsets(mDataOffset, mTargetLeftOffset, mTargetRightOffset);
		mThreads.append(thread);
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
		mFirstWrite = false;
		for(int i = 0; i < mLeftSamples * mChannels; ++i)
		{
			mWriteBuffer.append(ViScaler::scale(mReadBuffer[i], 0, 1, -1, 1));
		}
		write(mWriteBuffer);
		mWriteBuffer.clear();
	}

	while(mReadBuffer.size() >= mMinimumSamples)
	{
		QVector<qreal> subSamples = mReadBuffer.mid(0, mMinimumSamples);
		QList<ViSampleChunk> channels = ViSampleChanneler<qreal>::split(subSamples.data(), subSamples.size(), mChannels);

		mReadBuffer.remove(0, mChannels);

		for(int c = 0; c < mChannels; ++c)
		{
			mThreads[c]->setData(&channels[c]);
			mThreads[c]->run();
			mWriteBuffer.append(ViScaler::scale(mThreads[c]->network()->output(), 0, 1, -1, 1));
		}

		/*for(int c = 0; c < mChannels; ++c)
		{
			mThreads[c]->setData(&channels[c]);
			mThreads[c]->start();
		}
		for(int c = 0; c < mChannels; ++c)
		{
			while(mThreads[c]->isRunning());
			mWriteBuffer.append(ViScaler::scale(mThreads[c]->network()->output(), 0, 1, -1, 1));
		}*/


		if(mWriteBuffer.size() == mWriteSamples)
		{
			write(mWriteBuffer);
			mWriteBuffer.clear();
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
		mFirstWrite = false;
		for(int i = 0; i < mLeftSamples; ++i)
		{
			mWriteBuffer.append(ViScaler::scale(mReadBuffer[i], 0, 1, 1, -1));
		}
		write(mWriteBuffer);
		mWriteBuffer.clear();
	}

	while(mReadBuffer.size() >= mMinimumSamples)
	{
		ViSampleChunk wrapper(mReadBuffer.data(), mMinimumSamples, false);
		mThreads[0]->setData(&wrapper);
		mThreads[0]->run();
		mWriteBuffer.append(ViScaler::scale(mThreads[0]->network()->output(), 0, 1, -1, 1));

		if(mWriteBuffer.size() == mWriteSamples)
		{
			write(mWriteBuffer);
			mWriteBuffer.clear();
		}
		mReadBuffer.pop_front();
	}
}

void ViNeuralCorrector::execute()
{
	(this->*executePointer)();
}

void ViNeuralCorrector::finalize()
{
	//Write the remaining couple of samples
	write(mWriteBuffer);
	mWriteBuffer.clear();

	viDeleteAll(mThreads);
}
