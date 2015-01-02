#include <vineuralpredictor.h>
#include <vilogger.h>

#define DEFAULT_WINDOW_SIZE 32
#define MAX_HIDDEN_LAYERS 10

ViNeuralPredictor::ViNeuralPredictor(Mode mode)
	: ViPredictor()
{
	setWindowSize(DEFAULT_WINDOW_SIZE);
	setMode(mode);

	addParameterName("Window Size");

	for(int i = 0; i < MAX_HIDDEN_LAYERS; ++i)
	{
		addParameterName("Hidden Layer " + QString::number(i + 1));
		mHiddenNeurons.append(0);
	}

	mNetwork1 = NULL;
	mNetwork2 = NULL;
	mInput = NULL;
	mOutput = NULL;
}

ViNeuralPredictor::ViNeuralPredictor(const ViNeuralPredictor &other)
	: ViPredictor(other)
{
}

ViNeuralPredictor::~ViNeuralPredictor()
{
	clear();
}

void ViNeuralPredictor::setMode(Mode mode)
{
	mMode = mode;
}

void ViNeuralPredictor::setWindowSize(const int &size)
{
	mInputs = size;
	ViPredictor::setWindowSize(mInputs + 1);
	setOffset(mInputs);
}

void ViNeuralPredictor::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else if(number >= 1 && number <= MAX_HIDDEN_LAYERS)
	{
		mHiddenNeurons[number - 1] = value;
	}

	else
	{
		LOG("Invalid parameter for this predictor.", QtCriticalMsg);
		exit(-1);
	}
}

void ViNeuralPredictor::clear()
{
	delete mNetwork1;
	delete mNetwork2;
	delete [] mInput;
	delete [] mOutput;
}

void ViNeuralPredictor::initialize(const int &channelCount, const int &predictionCount)
{
	if(mMode == IncrementalSet) initializeIncrementalSet(channelCount, predictionCount);
	else if(mMode == IncrementalRecurrent) initializeIncrementalRecurrent(channelCount, predictionCount);
}

void ViNeuralPredictor::initializeIncrementalSet(const int &channelCount, const int &predictionCount)
{
	clear();

	mInputs = mWindowSize;
	mOutputs = predictionCount;

	QList<int> neurons;
	neurons.append(mInputs);
	for(int i = 0; i < MAX_HIDDEN_LAYERS; ++i)
	{
		if(mHiddenNeurons[i] == 0) break;
		neurons.append(mHiddenNeurons[i]);
	}
	neurons.append(mOutputs);

	mNetwork1 = new ViFann(false);
	mNetwork1->setStructure(ViFann::Standard, neurons);
	mNetwork1->setTraining(ViFann::Fixed, ViFann::Incremental);
	mNetwork1->setActivation(ViFann::Elliot);
	mNetwork1->setWeights(ViFann::Random);
	mNetwork1->setLearningRate(0.01);
	mNetwork1->setStopEpochs(1);
	mNetwork1->setStopMse(0.000000);
	if(!mNetwork1->isValid())
	{
		LOG("Invalid neural network.", QtFatalMsg);
		exit(-1);
	}

	mNetwork2 = new ViFann(false);
	mNetwork2->setStructure(ViFann::Standard, neurons);
	mNetwork2->setTraining(ViFann::Fixed, ViFann::Incremental);
	mNetwork2->setActivation(ViFann::Elliot);
	mNetwork2->setWeights(ViFann::Random);
	mNetwork2->setLearningRate(0.01);
	mNetwork2->setStopEpochs(1);
	mNetwork2->setStopMse(0.000000);
	if(!mNetwork2->isValid())
	{
		LOG("Invalid neural network.", QtFatalMsg);
		exit(-1);
	}

	mInput = new qreal[mInputs];
	mOutput = new qreal[mOutputs];

	setWindowSize(mWindowSize + mOutputs);
}

void ViNeuralPredictor::initializeIncrementalRecurrent(const int &channelCount, const int &predictionCount)
{
	clear();

	mInputs = mWindowSize + 1;
	mOutputs = 1;

	QList<int> neurons;
	neurons.append(mInputs);
	for(int i = 0; i < MAX_HIDDEN_LAYERS; ++i)
	{
		if(mHiddenNeurons[i] == 0) break;
		neurons.append(mHiddenNeurons[i]);
	}
	neurons.append(mOutputs);

	mNetwork1 = new ViFann(false);
	mNetwork1->setStructure(ViFann::Standard, neurons);
	mNetwork1->setTraining(ViFann::Fixed, ViFann::Incremental);
	mNetwork1->setActivation(ViFann::Elliot);
	mNetwork1->setWeights(ViFann::Random);
	mNetwork1->setLearningRate(0.01);
	mNetwork1->setStopEpochs(1);
	mNetwork1->setStopMse(0.000000);
	if(!mNetwork1->isValid())
	{
		LOG("Invalid neural network.", QtFatalMsg);
		exit(-1);
	}

	mNetwork2 = new ViFann(false);
	mNetwork2->setStructure(ViFann::Standard, neurons);
	mNetwork2->setTraining(ViFann::Fixed, ViFann::Incremental);
	mNetwork2->setActivation(ViFann::Elliot);
	mNetwork2->setWeights(ViFann::Random);
	mNetwork2->setLearningRate(0.01);
	mNetwork2->setStopEpochs(1);
	mNetwork2->setStopMse(0.000000);
	if(!mNetwork2->isValid())
	{
		LOG("Invalid neural network.", QtFatalMsg);
		exit(-1);
	}

	mInput = new qreal[mInputs];
	mOutput = new qreal[mOutputs];

	setWindowSize(mWindowSize + 2);
}

bool ViNeuralPredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViError *error, const int &channel)
{
	/*mNetworks[channel]->train(samples, samples[size - 1]);
	mNetworks[channel]->run(samples, predictedSamples, predictionCount);*/

	/*mNetworks[channel]->run(samples, predictedSamples);
	mNetworks[channel]->train(samples, samples + mInputs);*/

	if(mMode == IncrementalSet) return predictIncrementalSet(samples, size, predictedSamples, predictionCount, error, channel);
	else if(mMode == IncrementalRecurrent) return predictIncrementalRecurrent(samples, size, predictedSamples, predictionCount, error, channel);
}

bool ViNeuralPredictor::predictIncrementalSet(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViError *error, const int &channel)
{
	ViFann *network;
	if(channel == 0) network = mNetwork1;
	else network = mNetwork2;

	int counter = 0;

	network->run(samples + mOutputs, mOutput);
	for(counter = 0; counter < predictionCount; ++counter)
	{
		predictedSamples[counter] = mOutput[counter];
	}

	for(counter = 0; counter < mOutputs; ++counter)
	{
		mOutput[counter] = samples[counter + mInputs];
	}
	network->train(samples, mOutput);

	return true;
}

bool ViNeuralPredictor::predictIncrementalRecurrent(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViError *error, const int &channel)
{
	ViFann *network;
	if(channel == 0) network = mNetwork1;
	else network = mNetwork2;

	int i, j;
	qreal out = 0;

	for(i = 0; i < mInputs; ++i)
	{
		mInput[i] = samples[i + 1];
	}
	network->run(mInput, mOutput);
	out = predictedSamples[0] = mOutput[0];

	for(i = 1; i < predictionCount; ++i)
	{
		for(j = 1; j < mInputs; ++j)
		{
			mInput[j-1] = mInput[j];
		}
		mInput[mInputs - 1] = out;

		network->run(mInput, mOutput);
		out = predictedSamples[i] = mOutput[0];
	}

	mOutput[0] = samples[size - 1];
	network->train(samples, mOutput);

	return true;
}
