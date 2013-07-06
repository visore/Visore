#include <vineuralcorrector.h>
#include <visamplechanneler.h>
#include <vineuralnetworkfactory.h>
#include <qmath.h>

ViNeuralCorrector::ViNeuralCorrector()
	: ViModifyProcessor(false) //Make sure that the processor is not automatically writing the sample again
{
	mNetwork = NULL;
	mTrainer = NULL;
	mProvider = NULL;

	mProviderLeftSamples = 0;
	mProviderRightSamples = 0;

	mMinimumSamples = 0;
	mLeftSamples = 0;
	mDataOffset = 0;
	mTargetLeftOffset = 0;
	mTargetRightOffset = 0;
}

ViNeuralCorrector::ViNeuralCorrector(ViNeuralNetwork *network, ViTrainer *trainer, ViTargetProvider *provider)
	: ViModifyProcessor(false) //Make sure that the processor is not automatically writing the sample again
{
	mNetwork = network;
	mTrainer = trainer;
	mProvider = provider;

	mProviderLeftSamples = mProvider->leftSamples();
	mProviderRightSamples = mProvider->rightSamples();

	mMinimumSamples = 0;
	mLeftSamples = 0;
	mDataOffset = 0;
	mTargetLeftOffset = 0;
	mTargetRightOffset = 0;
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

	viDeleteAll(mNetworks);
	viDeleteAll(mTrainers);
	viDeleteAll(mProviders);
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

	// All values that are read/written are scaled from [-1, 1] to [0, 1] and vice versa.
	data().setScaleRange(0, 1);
	data2().setScaleRange(0, 1);

	int channels = usedChannelCount();

	mFirstWrites.resize(channels);

	mReadBuffers.clear();
	mReadBuffers.resize(channels);

	mWriteBuffers.clear();
	mWriteBuffers.resize(channels);
	mWritePositions.resize(channels);

	viDeleteAll(mNetworks);
	viDeleteAll(mTrainers);
	viDeleteAll(mProviders);

	for(int i = 0; i < channels; ++i)
	{
		mFirstWrites[i] = true;

		mWriteBuffers[i] = ViSampleChunk(data().sampleCount() / channels);
		mWritePositions[i] = 0;

		ViNeuralNetwork *network = mNetwork->clone();
		ViTrainer *trainer = mTrainer->clone();
		trainer->setNetwork(network);
		mNetworks.append(network);
		mTrainers.append(trainer);
		mProviders.append(mProvider->clone());
	}

	int inputSamples = mNetwork->inputCount(false);
	if(mProviderLeftSamples > inputSamples)
	{
		mDataOffset = mProviderLeftSamples - inputSamples;
		mLeftSamples = mProviderLeftSamples;
		mTargetLeftOffset = 0;
	}
	else
	{
		mDataOffset = 0;
		mLeftSamples = inputSamples;
		mTargetLeftOffset = inputSamples - mProviderLeftSamples;
	}
	mTargetRightOffset = mLeftSamples + 1;
	mMinimumSamples = (mLeftSamples + 1 + mProviderRightSamples);
}

void ViNeuralCorrector::execute(int channel)
{
	mMutex.lock();
	ViSampleChunk &newSamples = currentSamples(channel);
	QVector<qreal> &readBuffer = mReadBuffers[channel];

	ViNeuralNetwork *network = mNetworks[channel];
	ViTrainer *trainer = mTrainers[channel];
	ViTargetProvider *provider = mProviders[channel];

	ViSampleChunk leftTargetData(mProviderLeftSamples);
	ViSampleChunk rightTargetData(mProviderRightSamples);

	int minimumSamples = mMinimumSamples;
	int dataOffset = mDataOffset;
	int providerLeftSamples = mProviderLeftSamples;
	int targetLeftOffset = mTargetLeftOffset;
	int providerRightSamples = mProviderLeftSamples;
	int targetRightOffset = mTargetLeftOffset;

	int &position = mWritePositions[channel];
	ViSampleChunk &writeBuffer = mWriteBuffers[channel];

	for(int i = 0; i < newSamples.size(); ++i)
	{
		readBuffer.append(newSamples[i]);
	}

	//Ensures that the first window of samples is written immediatly, since they can't be corrected
	if(mFirstWrites[channel] && readBuffer.size() >= mLeftSamples * usedChannelCount())
	{
		int size = mLeftSamples * usedChannelCount();
		ViSampleChunk buffer(size);
		mFirstWrites[channel] = false;
		for(int i = 0; i < size; ++i)
		{
			buffer[size] = readBuffer[i];
		}
		data2().enqueueSplitScaledSamples(buffer, channel);
	}
	mMutex.unlock();

	QVector<qreal> subSamples;
	while(readBuffer.size() >= minimumSamples)
	{
		subSamples = readBuffer.mid(0, minimumSamples);
		readBuffer.remove(0);

		//Set the network inputs
		int inputs = network->inputCount(false);
		for(int i = 0; i < inputs; ++i)
		{
			network->setInput(i, subSamples.at(i + dataOffset));
		}

		//Set the provider inputs
		for(int i = 0; i < providerLeftSamples; ++i)
		{
			leftTargetData[i] = subSamples.at(targetLeftOffset + i);
		}
		for(int i = 0; i < providerRightSamples; ++i)
		{
			rightTargetData[i] = subSamples.at(targetRightOffset + i);
		}
		provider->setData(&leftTargetData, &rightTargetData);

		// Run training
		trainer->setTargetValues({provider->calculate()});
		//for(int i = 0; i < 1; ++i)
		//{
		//	trainer->trainSingle();
		//}
		trainer->trainSingle();

		// Get output
		writeBuffer[position] = network->output();
		++position;
		if(position == writeBuffer.size())
		{
			writeScaled(writeBuffer, channel);
			position = 0;
		}
	}
}

void ViNeuralCorrector::finalize()
{
	// Write last samples
	for(int i = 0; i < usedChannelCount(); ++i)
	{
		mWriteBuffers[i].setSize(mWritePositions[i]);
		writeScaled(mWriteBuffers[i], i);
	}

	mReadBuffers.clear();
	mWriteBuffers.clear();
	mWritePositions.clear();
	mFirstWrites.clear();

	viDeleteAll(mNetworks);
	viDeleteAll(mTrainers);
	viDeleteAll(mProviders);
}

ViElement ViNeuralCorrector::exportData()
{
    ViElement root("Corrector");
    root.addChild("Name", name());
    if(mTrainer != NULL)
    {
        root.addChild(mTrainer->exportData());
    }
    if(mProvider != NULL)
    {
        root.addChild(mProvider->exportData());
    }
    if(mNetwork != NULL)
    {
        root.addChild(mNetwork->exportData());
    }
    return root;
}

bool ViNeuralCorrector::importData(ViElement element)
{
	if(element.name() == "Corrector" && element.child("Name").toString() == name())
    {
        if(mNetwork != NULL)
        {
            delete mNetwork;
        }
        ViNeuralNetworkFactory factory;
        mNetwork = factory.create(element);

        if(mTrainer != NULL)
        {
            delete mTrainer;
        }
        mTrainer = ViTrainerManager::create(element);

        if(mProvider != NULL)
        {
            delete mProvider;
        }
        mProvider = ViTargetProviderManager::create(element);

        return true;
    }
	return false;
}
