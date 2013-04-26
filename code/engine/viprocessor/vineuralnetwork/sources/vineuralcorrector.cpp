#include <vineuralcorrector.h>
#include <viscaler.h>
#include <qmath.h>

#include <viactivationfunctionmanager.h>
#include <vierrorfunctionmanager.h>
#include <viweightinitializermanager.h>

#define INPUT_SAMPLES 4
#define TRAINER_SAMPLES 3 //Needs to be odd

ViNeuralCorrector::ViNeuralCorrector()
	: ViModifyProcessor(false) //Make sure that the processor is not automatically writing the sample again
{
	mNetwork = NULL;
	mTrainer = NULL;
	mProvider = NULL;
}

ViNeuralCorrector::~ViNeuralCorrector()
{
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

void ViNeuralCorrector::initialize()
{
	if(mNetwork != NULL)
	{
		delete mNetwork;
	}
	if(mTrainer != NULL)
	{
		delete mTrainer;
	}
	if(mProvider != NULL)
	{
		delete mProvider;
	}

	mFactory.setActivationFunction(ViActivationFunctionManager::create("SigmoidActivationFunction"));
	mFactory.addLayer(INPUT_SAMPLES + 1);
	mFactory.addLayer(2);
	mFactory.addLayer(1);
	mNetwork = mFactory.create();

	mTrainer = ViTrainerManager::createDefault();
	mTrainer->setNetwork(mNetwork);
	mTrainer->addErrorFunction(ViErrorFunctionManager::create("RootMeanSquaredError"));

	mProvider = ViTargetProviderManager::createDefault();

	ViWeightInitializer *weightInitializer = ViWeightInitializerManager::create("Random");
	weightInitializer->initialize(mNetwork, mTrainer->learningRate());
	delete weightInitializer;

	mReadBuffer.clear();
	mFirstWrite = true;
}

void ViNeuralCorrector::execute()
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
			mWriteBuffer.append(ViScaler::scale(mReadBuffer[i], 0, 1, -1, 1));
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
	}
}

void ViNeuralCorrector::finalize()
{
	//Write the remaining couple of samples
	write(mWriteBuffer);
	mWriteBuffer.clear();

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
