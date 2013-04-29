#include <vineuralcorrector.h>
#include <visamplechanneler.h>
#include <viscaler.h>
#include <qmath.h>

#include <viactivationfunctionmanager.h>
#include <vierrorfunctionmanager.h>
#include <viweightinitializermanager.h>

#define INPUT_SAMPLES 16
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
	for(int i = 0; i < 10; ++i)
	{
		mTrainer->trainSingle();
	}
}

ViNeuralCorrector::ViNeuralCorrector()
	: ViModifyProcessor(false) //Make sure that the processor is not automatically writing the sample again
{
	enableSeparateChannels();
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

	int channels = 1;
	if(mSeparateChannels)
	{
		channels = object()->format(type()).channelCount();
	}

	mFactory.setActivationFunction(ViActivationFunctionManager::create("SigmoidActivationFunction"));
	mFactory.addLayer(INPUT_SAMPLES + 1);
	mFactory.addLayer(8);
	mFactory.addLayer(1);

	ViWeightInitializer *weightInitializer = ViWeightInitializerManager::create("Random");
	ViNeuralNetwork *network;
	ViTrainer *trainer;

	for(int i = 0; i < channels; ++i)
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
}

void ViNeuralCorrector::executeWithChannels()
{
	static int channelCount = mThreads.size();

	ViSampleChunk &newSamples = samples();
	for(int i = 0; i < newSamples.size(); ++i)
	{
		mReadBuffer.append(ViScaler::scale(newSamples[i], -1, 1, 0, 1));
	}

	//Ensures that the first window of samples is written immediatly, since they can't be corrected
	if(mFirstWrite && mReadBuffer.size() >= INPUT_SAMPLES * channelCount)
	{
		mFirstWrite = false;
		for(int i = 0; i < INPUT_SAMPLES * channelCount; ++i)
		{
			mWriteBuffer.append(ViScaler::scale(mReadBuffer[i], 0, 1, -1, 1));
		}
		write(mWriteBuffer);
		mWriteBuffer.clear();
	}

	static int halfTargetSamples = qCeil(TRAINER_SAMPLES / 2.0);
	static int minimumSamples = (INPUT_SAMPLES + halfTargetSamples) * channelCount;

	while(mReadBuffer.size() >= minimumSamples)
	{
		QVector<qreal> subSamples = mReadBuffer.mid(0, minimumSamples);
		QList<ViSampleChunk> channels = ViSampleChanneler<qreal>::split(subSamples.data(), subSamples.size(), channelCount);

		mReadBuffer.remove(0, channelCount);
		for(int c = 0; c < channelCount; ++c)
		{
			mThreads[c]->setData(&channels[c]);
			mThreads[c]->start();
			//mThreads[c]->run();
		}
		for(int c = 0; c < channelCount; ++c)
		{
			while(mThreads[c]->isRunning());
			/*{
				mThreads[c]->wait(1000);
			}*/
			mWriteBuffer.append(ViScaler::scale(mThreads[c]->network()->output(), 0, 1, -1, 1));
		}

		static int writeSamples = qFloor(sampleCount() / qreal(channelCount)) * channelCount;
		if(mWriteBuffer.size() == writeSamples)
		{
			write(mWriteBuffer);
			mWriteBuffer.clear();
		}
	}
}

void ViNeuralCorrector::executeWithoutChannels()
{


/*
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

	static int halfTargetSamples = qCeil(TRAINER_SAMPLES / 2.0);
	while(mReadBuffer.size() >= (INPUT_SAMPLES + halfTargetSamples + 1))
	{
		for(int i = 0; i < INPUT_SAMPLES; ++i)
		{
			mNetwork->setInput(i, mReadBuffer[i]);
		}
		mNetwork->setInput(INPUT_SAMPLES, mNetwork->output());

		ViSampleChunk targetBuffer(TRAINER_SAMPLES);
		qint64 targetOffset = INPUT_SAMPLES - halfTargetSamples + 1;
		for(int i = 0; i < TRAINER_SAMPLES; ++i)
		{
			targetBuffer[i] = mReadBuffer[targetOffset + i];
		}

		mProvider->setData(targetBuffer);
		mTrainer->setTargetValues({mProvider->calculate()});
		mTrainer->trainSingle();

		mWriteBuffer.append(ViScaler::scale(mNetwork->output(), 0, 1, -1, 1));
		if(mWriteBuffer.size() == INPUT_SAMPLES)
		{
			write(mWriteBuffer);
			mWriteBuffer.clear();
		}

		mReadBuffer.removeFirst();
	}*/
}

void ViNeuralCorrector::execute()
{
	executeWithChannels();
}

void ViNeuralCorrector::finalize()
{
	//Write the remaining couple of samples
	write(mWriteBuffer);
	mWriteBuffer.clear();

	viDeleteAll(mThreads);
}
