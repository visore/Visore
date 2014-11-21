#include <vineuralinterpolator.h>
#include <vimanager.h>
#include <vinoisecreator.h>

#define DEFAULT_WINDOW_SIZE 32
#define MAX_HIDDEN_LAYERS 10

ViNeuralInterpolator::ViNeuralInterpolator(Mode mode)
	: ViInterpolator()
{
	mInput = NULL;
	setWindowSize(DEFAULT_WINDOW_SIZE);
	setMode(mode);

	addParameterName("Window Size");

	for(int i = 0; i < MAX_HIDDEN_LAYERS; ++i)
	{
		addParameterName("Hidden Layer " + QString::number(i + 1));
		mHiddenNeurons.append(0);
	}
}

ViNeuralInterpolator::ViNeuralInterpolator(const ViNeuralInterpolator &other)
	: ViInterpolator(other)
{
	mInput = NULL;
	setWindowSize(other.mWindowSize);
}

ViNeuralInterpolator::~ViNeuralInterpolator()
{
	clear();
}

QString ViNeuralInterpolator::name(QString replace, bool spaced)
{
	return ViInterpolator::name(replace, spaced);
}

void ViNeuralInterpolator::setWindowSize(const int &size)
{
	mWindowSize = size;
	//if(mInput != NULL) delete [] mInput;
	//mInput = new qreal[mWindowSize];
}

void ViNeuralInterpolator::setMode(Mode mode)
{
	mMode = mode;
}

void ViNeuralInterpolator::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else if(number >= 1 && number <= MAX_HIDDEN_LAYERS)
	{
		mHiddenNeurons[number - 1] = value;
	}

	else
	{
		LOG("Invalid parameter for this interpolator.", QtCriticalMsg);
		exit(-1);
	}
}

bool ViNeuralInterpolator::validParameters()
{
	return true;
}

void ViNeuralInterpolator::clear()
{
	int i;

	for(i = 0; i < mNetworks.size(); ++i) delete mNetworks[i];
	mNetworks.clear();

	if(mInput != NULL)
	{
		delete [] mInput;
		mInput = NULL;
	}

	for(i = 0; i < mOutput.size(); ++i) delete [] mOutput[i];
	mOutput.clear();

	/*for(int i = 0; i < MAX_HIDDEN_LAYERS; ++i)
	{
		mHiddenNeurons[i] = 0;
	}*/
}

void ViNeuralInterpolator::initialize(const int &channelCount)
{
	if(mMode == IncrementalPrediction) initializeIncrementalPrediction(channelCount);
	else if(mMode == SetPrediction) initializeSetPrediction(channelCount);
	else if(mMode == IncrementalRecurrentPrediction) initializeIncrementalRecurrentPrediction(channelCount);
	else if(mMode == IncrementalSetPrediction) initializeIncrementalSetPrediction(channelCount);
	else if(mMode == Interpolation) initializeInterpolation(channelCount);
}

void ViNeuralInterpolator::initializeIncrementalPrediction(const int &channelCount)
{
	clear();

	mInputs = mWindowSize;
	mOutputs = ViNoiseCreator::maximumNoiseSize();

	QList<int> neurons;
	neurons.append(mInputs);
	for(int i = 0; i < MAX_HIDDEN_LAYERS; ++i)
	{
		if(mHiddenNeurons[i] == 0) break;
		neurons.append(mHiddenNeurons[i]);
	}
	neurons.append(mOutputs);

	ViFann *network = new ViFann(false);
	network->setStructure(ViFann::Standard, neurons);
	network->setTraining(ViFann::Fixed, ViFann::Incremental);
	network->setActivation(ViFann::Elliot);
	network->setWeights(ViFann::Random);
	//network->setWeights(ViFann::WidrowNguyen);
	//network->setLearningRate(0.2);
	//network->setLearningMomentum(0.1);
	network->setStopEpochs(1);
	network->setStopMse(0.000000);
	//network->setStopStagnation(0.0000001, 5);
	if(!network->isValid())
	{
		LOG("Invalid neural network.", QtFatalMsg);
		exit(-1);
	}

	mNetworks.append(network);
	mInput = new qreal[mInputs];
	mOutput.append(new qreal[mOutputs]);
}

void ViNeuralInterpolator::initializeSetPrediction(const int &channelCount)
{
	clear();

	mInputs = mWindowSize;
	mOutputs = ViNoiseCreator::maximumNoiseSize();

	QList<int> neurons;
	neurons.append(mInputs);
	for(int i = 0; i < MAX_HIDDEN_LAYERS; ++i)
	{
		if(mHiddenNeurons[i] == 0) break;
		neurons.append(mHiddenNeurons[i]);
	}
	neurons.append(mOutputs);

	ViFann *network = new ViFann(false);
	network->setStructure(ViFann::Shortcut, neurons);
	network->setTraining(ViFann::Fixed, ViFann::QuickProp);
	network->setActivation(ViFann::Elliot);
	network->setWeights(ViFann::Random);
	//network->setWeights(ViFann::WidrowNguyen);
	//network->setLearningRate(0.2);
	//network->setLearningMomentum(0.1);
	network->setStopEpochs(100);
	network->setStopMse(0.000000);
	network->setStopStagnation(0.0000001, 5);
	if(!network->isValid())
	{
		LOG("Invalid neural network.", QtFatalMsg);
		exit(-1);
	}

	mNetworks.append(network);
	mInput = new qreal[mInputs];
	mOutput.append(new qreal[mOutputs]);
}

void ViNeuralInterpolator::initializeIncrementalRecurrentPrediction(const int &channelCount)
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

	ViFann *network = new ViFann(false);
	network->setStructure(ViFann::Shortcut, neurons);
	network->setTraining(ViFann::Fixed, ViFann::Incremental);
	network->setActivation(ViFann::Elliot);
	network->setWeights(ViFann::Random);
	//network->setWeights(ViFann::WidrowNguyen);
	//network->setLearningRate(0.2);
	//network->setLearningMomentum(0.1);
	network->setStopEpochs(1);
	network->setStopMse(0.000000);
	network->setStopStagnation(0.0000001, 5);
	if(!network->isValid())
	{
		LOG("Invalid neural network.", QtFatalMsg);
		exit(-1);
	}

	mNetworks.append(network);
	mInput = new qreal[mInputs];
	mOutput.append(new qreal[mOutputs]);
}

void ViNeuralInterpolator::initializeIncrementalSetPrediction(const int &channelCount)
{

}

void ViNeuralInterpolator::initializeInterpolation(const int &channelCount)
{
	int i;
	clear();

	mOutputs = 50;

	mInputs = mWindowSize;
	mLeftInputs = ceil(mInputs / 2);
	mRightInputs = mInputs - mLeftInputs;

	QList<int> neurons;
	neurons.append(mInputs);
	for(int i = 0; i < MAX_HIDDEN_LAYERS; ++i)
	{
		if(mHiddenNeurons[i] == 0) break;
		neurons.append(mHiddenNeurons[i]);
	}
	neurons.append(mOutputs);

	mInput = new qreal[mInputs];
	for(i = 1; i <= mOutputs; ++i)
	{
		neurons.last() = i;
		ViFann *network = new ViFann(false);
		network->setStructure(ViFann::Shortcut, neurons);
		network->setActivation(ViFann::Elliot);
		network->setWeights(ViFann::Random);
		//network->setWeights(ViFann::WidrowNguyen);
		//network->setLearningRate(0.2);
		//network->setLearningMomentum(0.1);
		network->setTraining(ViFann::Fixed, ViFann::QuickProp);
		network->setStopEpochs(50);
		network->setStopMse(0.0000001);
		network->setStopStagnation(0.0000001, 5);
		network->setStopNeurons(10);
		if(!network->isValid())
		{
			LOG("Invalid neural network.", QtFatalMsg);
			exit(-1);
		}
		//network->loadFromFile(ViManager::tempOtherPath() + QString::number(i) + ".fann");

		mNetworks.append(network);

		mOutput.append(new qreal[i]);
	}

	mTrainCount = 0;
}

bool ViNeuralInterpolator::interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel)
{
	if(mMode == IncrementalPrediction) interpolateIncrementalPrediction(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize, error, channel);
	else if(mMode == SetPrediction) interpolateSetPrediction(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize, error, channel);
	else if(mMode == IncrementalRecurrentPrediction) interpolateIncrementalRecurrentPrediction(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize, error, channel);
	else if(mMode == IncrementalSetPrediction) interpolateIncrementalSetPrediction(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize, error, channel);
	else if(mMode == Interpolation) interpolateInterpolation(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize, error, channel);
}

bool ViNeuralInterpolator::interpolateIncrementalPrediction(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel)
{
	ViFann *network = mNetworks.first();
	int counter = 0;
	int total = mInputs + mOutputs;

	network->createTrain(leftSize - total);
	while(counter < leftSize - total)
	{
		network->setTrainInput(counter, leftSamples + counter);
		network->setTrainOutput(counter, leftSamples + counter + mInputs);
		++counter;
	}

	network->train();
	network->deleteTrain();

	qreal *output = mOutput.first();
	network->run(leftSamples + (leftSize - mInputs), output);
	for(counter = 0; counter < outputSize; ++counter)
	{
		outputSamples[counter] = output[counter];
	}

	/*ViFann *network = mNetworks.first();
	int counter = 0;
	int total = mInputs + mOutputs;

	while(counter < leftSize - total)
	{
		network->train(leftSamples + counter, leftSamples + counter + mInputs);
		++counter;
	}

	qreal *output = mOutput.first();
	network->run(leftSamples + (leftSize - mInputs), output);
	for(counter = 0; counter < outputSize; ++counter)
	{
		outputSamples[counter] = output[counter];
	}*/
}

bool ViNeuralInterpolator::interpolateSetPrediction(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel)
{
	ViFann *network = mNetworks.first();
	int counter = 0;
	int total = mInputs + mOutputs;

	network->createTrain(leftSize - total);
	while(counter < leftSize - total)
	{
		network->setTrainInput(counter, leftSamples + counter);
		network->setTrainOutput(counter, leftSamples + counter + mInputs);
		++counter;
	}

	network->train();
	network->deleteTrain();

	qreal *output = mOutput.first();
	network->run(leftSamples + (leftSize - mInputs), output);
	for(counter = 0; counter < outputSize; ++counter)
	{
		outputSamples[counter] = output[counter];
	}
}

bool ViNeuralInterpolator::interpolateIncrementalRecurrentPrediction(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel)
{
	ViFann *network = mNetworks.first();
	int i, j, counter = 0, end = mInputs - 1;
	int total = mInputs + mOutputs;
	qreal out = 0;

	network->createTrain(leftSize - total);
	while(counter < leftSize - total)
	{
		network->setTrainInput(counter, leftSamples + counter);
		network->setTrainOutput(counter, leftSamples + counter + mInputs);
		++counter;
	}

	network->train();
	network->deleteTrain();

	qreal *output = mOutput.first();
	counter = leftSize - end - 1;
	for(i = 0; i <= end; ++i)
	{
		mInput[i] = leftSamples[counter + i];
	}
	network->run(mInput, output);
	out = outputSamples[0] = output[0];

	for(i = 1; i < outputSize; ++i)
	{
		for(j = 1; j < mInputs; ++j)
		{
			mInput[j-1] = mInput[j];
		}
		mInput[mInputs - 1] = out;

		network->run(mInput, output);
		out = outputSamples[i] = output[0];
	}
}

bool ViNeuralInterpolator::interpolateIncrementalSetPrediction(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel)
{

}

bool ViNeuralInterpolator::interpolateInterpolation(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel)
{
	//mNetworks[outputSize - 1]->setWeights(ViFann::Random);

	train(leftSamples, leftSize, rightSamples, rightSize, outputSize, 768, 2); // 768, 2
	//for(int i = 1; i <= mOutputs; ++i) train(leftSamples, leftSize, rightSamples, rightSize, i, 10000,4);
	//train(leftSamples, leftSize, rightSamples, rightSize, outputSize, 64, 4); // 768, 2

	devide(leftSamples, leftSize, rightSamples, mInput, mLeftInputs, mRightInputs);
	mNetworks[outputSize - 1]->run(mInput, outputSamples);
}

void ViNeuralInterpolator::train(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const int &trainCount, const int &stepSize)
{
	static int halfTrainCount, leftOffset, rightOffset, windowSize, halfSteps, counter;

	ViFann *network = mNetworks[outputSize - 1];
	qreal *output = mOutput[outputSize - 1];

	counter = 0;
	windowSize = mInputs + outputSize;
	halfTrainCount = floor(trainCount / 2.0);

	if(halfTrainCount > 0)
	{
		halfSteps = halfTrainCount * stepSize;
		leftOffset = leftSize - halfSteps - windowSize;
		rightOffset = rightSize - halfSteps - windowSize;
	}

	if(halfTrainCount <= 0 || leftOffset < 0 || rightOffset < 0)
	{
		halfTrainCount = qMin(floor((leftSize - windowSize) / qreal(stepSize)), floor((rightSize - windowSize) / qreal(stepSize)));
		halfSteps = halfTrainCount * stepSize;
		leftOffset = leftSize - halfSteps - windowSize;
		rightOffset = rightSize - halfSteps - windowSize;
	}

	network->createTrain(halfTrainCount * 2);

	while(devide((qreal*) leftSamples, leftSize, leftOffset, mInput, mLeftInputs, mRightInputs, output, outputSize))
	{
		network->setTrainInput(counter, mInput);
		network->setTrainOutput(counter, output);
		//network->train(mInput, output);
		leftOffset += stepSize;
		++counter;
	}

	qreal *reversed = new qreal[rightSize];
	reverse(rightSamples, reversed, rightSize);
	while(devide((qreal*) reversed, rightSize, rightOffset, mInput, mRightInputs, mLeftInputs, output, outputSize))
	{
		network->setTrainInput(counter, mInput);
		network->setTrainOutput(counter, output);
		rightOffset += stepSize;
		++counter;
	}
	delete [] reversed;

	//network->train(true);
	int trains = network->train();
	mTrainCount += trains;
	network->deleteTrain();
	//cout<<"Training epochs: " << trains << " (" << mTrainCount << ") " << outputSize << endl;
	//network->saveToFile(ViManager::tempOtherPath() + QString::number(outputSize) + ".fann");
}

bool ViNeuralInterpolator::devide(const qreal *samples, const int &sampleSize, const int &offset, qreal *input, const int &leftInputs, const int &rightInputs, qreal *output, const int &outputSize)
{
	if(offset + leftInputs + rightInputs + outputSize >= sampleSize) return false;
	static size_t size = sizeof(qreal);

	// Copy left inputs
	memcpy(input, samples + offset, size * leftInputs);

	// Copy outputs
	memcpy(output, samples + offset + leftInputs, size * outputSize);

	// Copy right inputs
	memcpy(input + leftInputs, samples + offset + leftInputs + outputSize, size * rightInputs);

	return true;
}

void ViNeuralInterpolator::devide(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, qreal *input, const int &leftInputs, const int &rightInputs)
{
	static size_t size = sizeof(qreal);

	// Copy left inputs
	memcpy(input, leftSamples + (leftSize - leftInputs), size * leftInputs);

	// Copy right inputs
	memcpy(input + leftInputs, rightSamples, size * rightInputs);
}

void ViNeuralInterpolator::reverse(const qreal *input, qreal *output, const int &size)
{
	static int i, end;
	end = size - 1;
	for(i = 0; i < size; ++i) output[i] = input[end - i];
}

ViNeuralInterpolator* ViNeuralInterpolator::clone()
{
	return new ViNeuralInterpolator(*this);
}
