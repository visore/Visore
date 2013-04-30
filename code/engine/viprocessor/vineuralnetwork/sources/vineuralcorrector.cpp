#include <vineuralcorrector.h>
#include <visamplechanneler.h>
#include <viscaler.h>
#include <qmath.h>

#include <viactivationfunctionmanager.h>
#include <vierrorfunctionmanager.h>
#include <viweightinitializermanager.h>

#define INPUT_SAMPLES 32
#define TRAINER_SAMPLES 3 //Needs to be odd

ViNeuralCorrectorThread::ViNeuralCorrectorThread(ViNeuralNetwork *network, ViTrainer *trainer, ViTargetProvider *provider)
	: QThread()
{
	mNetwork = network;
	mTrainer = trainer;
	mProvider = provider;

	mData = NULL;
	mTargetData.resize(TRAINER_SAMPLES);
	mTargetOffset = INPUT_SAMPLES - qCeil(TRAINER_SAMPLES / 2.0) + 1;
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

ViNeuralNetwork* ViNeuralCorrectorThread::network()
{
	return ViNeuralCorrectorThread::mNetwork;
}

void ViNeuralCorrectorThread::run()
{
	for(int i = 0; i < INPUT_SAMPLES; ++i)
	{
		mNetwork->setInput(i, mData->at(i));
	}
	mNetwork->setInput(INPUT_SAMPLES, mNetwork->output());

	for(int i = 0; i < TRAINER_SAMPLES; ++i)
	{
		mTargetData[i] = mData->at(mTargetOffset + i);
	}

	mProvider->setData(mTargetData);
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
	mHalfTargetSamples = qCeil(TRAINER_SAMPLES / 2.0);
	mMinimumSamples = 0;
	mWriteSamples = 0;
	mSeparateChannels = false;
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

	mFactory.setActivationFunction(ViActivationFunctionManager::create("SigmoidActivationFunction"));
	mFactory.addLayer(INPUT_SAMPLES + 1);
	mFactory.addLayer(16);
	mFactory.addLayer(8);
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

		mThreads.append(new ViNeuralCorrectorThread(network, trainer, ViTargetProviderManager::createDefault()));
	}

	delete weightInitializer;

	mReadBuffer.clear();
	mFirstWrite = true;

	mMinimumSamples = (INPUT_SAMPLES + mHalfTargetSamples) * mChannels;
	mWriteSamples = qFloor(sampleCount() / qreal(mChannels)) * mChannels;
}

void ViNeuralCorrector::executeWithChannels()
{
	ViSampleChunk &newSamples = samples();
	for(int i = 0; i < newSamples.size(); ++i)
	{
		mReadBuffer.append(ViScaler::scale(newSamples[i], -1, 1, 0, 1));
	}

	//Ensures that the first window of samples is written immediatly, since they can't be corrected
	if(mFirstWrite && mReadBuffer.size() >= INPUT_SAMPLES * mChannels)
	{
		mFirstWrite = false;
		for(int i = 0; i < INPUT_SAMPLES * mChannels; ++i)
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
			mThreads[c]->start();
			//mThreads[c]->run();
		}
		for(int c = 0; c < mChannels; ++c)
		{
			while(mThreads[c]->isRunning());
			/*{
				mThreads[c]->wait(1000);
			}*/
			mWriteBuffer.append(ViScaler::scale(mThreads[c]->network()->output(), 0, 1, -1, 1));
		}

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
	if(mFirstWrite && mReadBuffer.size() >= INPUT_SAMPLES)
	{
		mFirstWrite = false;
		for(int i = 0; i < INPUT_SAMPLES; ++i)
		{
			mWriteBuffer.append(ViScaler::scale(mReadBuffer[i], 0, 1, 1, -1));
		}
		write(mWriteBuffer);
		mWriteBuffer.clear();
	}

	while(mReadBuffer.size() >= (INPUT_SAMPLES + mHalfTargetSamples + 1))
	{
		ViSampleChunk wrapper(mReadBuffer.data(), mReadBuffer.size(), false);
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
