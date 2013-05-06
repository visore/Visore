#include <vineuralcorrector.h>
#include <visamplechanneler.h>
#include <viscaler.h>
#include <qmath.h>

#include <viactivationfunctionmanager.h>
#include <vierrorfunctionmanager.h>
#include <viweightinitializermanager.h>
#include <viinterpolatormanager.h>
#include <viinterpolationtargetprovider.h>

#define INPUT_SAMPLES 128
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
	for(int i = 0; i < INPUT_SAMPLES; ++i)
	{
		mNetwork->setInput(i, mData->at(i + mDataOffset));
	}
	mNetwork->setInput(INPUT_SAMPLES, mNetwork->output());

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

	if(TRAINER_LEFT_SAMPLES > INPUT_SAMPLES)
	{
		mDataOffset = TRAINER_LEFT_SAMPLES - INPUT_SAMPLES;
		mLeftSamples = TRAINER_LEFT_SAMPLES;
		mTargetLeftOffset = 0;
	}
	else
	{
		mDataOffset = 0;
		mLeftSamples = INPUT_SAMPLES;
		mTargetLeftOffset = INPUT_SAMPLES - TRAINER_LEFT_SAMPLES;
	}
	mTargetRightOffset = mLeftSamples + 1;
	mMinimumSamples = (mLeftSamples + 1 + TRAINER_RIGHT_SAMPLES) * mChannels;
	mWriteSamples = qFloor(sampleCount() / qreal(mChannels)) * mChannels;

	mFactory.setActivationFunction(ViActivationFunctionManager::create("SigmoidActivationFunction"));
	mFactory.addLayer(INPUT_SAMPLES + 1);
	mFactory.addLayer(64);
	mFactory.addLayer(32);
	mFactory.addLayer(1);

	ViWeightInitializer *weightInitializer = ViWeightInitializerManager::create("RandomWeightInitializer");
	ViNeuralNetwork *network;
	ViTrainer *trainer;

	for(int i = 0; i < mChannels; ++i)
	{
		trainer = ViTrainerManager::createDefault();

		network = mFactory.create();
		weightInitializer->initialize(network, trainer->learningRate());

		trainer->addErrorFunction(ViErrorFunctionManager::create("RootMeanSquaredError"));
		trainer->setNetwork(network);

		ViInterpolationTargetProvider *provider = (ViInterpolationTargetProvider*) ViTargetProviderManager::create("InterpolationTargetProvider");
		ViInterpolator *interpolator = ViInterpolatorManager::create("CubicInterpolator");

		provider->setInterpolator(interpolator);


		ViNeuralCorrectorThread *thread = new ViNeuralCorrectorThread(network, trainer, provider);
		thread->setOffsets(mDataOffset, mTargetLeftOffset, mTargetRightOffset);
		mThreads.append(thread);
	}

	delete weightInitializer;
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
