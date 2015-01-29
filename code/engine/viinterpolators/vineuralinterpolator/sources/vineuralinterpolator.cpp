#include <vineuralinterpolator.h>
#include <vimanager.h>
#include <vinoisecreator.h>

#define DEFAULT_WINDOW_SIZE 32
#define DEFAULT_EPOCHS 50
#define MAX_HIDDEN_LAYERS 10

ViNeuralInterpolator::ViNeuralInterpolator(Mode mode)
	: ViInterpolator()
{
	mInput = NULL;
	setWindowSize(DEFAULT_WINDOW_SIZE);
	setMode(mode);
	setEpochs(DEFAULT_EPOCHS);

	addParameterName("Epochs");
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

void ViNeuralInterpolator::setEpochs(int epochs)
{
	mEpochs = epochs;
}

void ViNeuralInterpolator::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setEpochs(value);
	else if(number == 1) setWindowSize(value);
	else if(number >= 2 && number <= MAX_HIDDEN_LAYERS)
	{
		mHiddenNeurons[number - 2] = value;
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
	if(mMode == Incremental) initializeIncremental(channelCount);
	else if(mMode == Recurrent) initializeRecurrent(channelCount);
	else if(mMode == SeparateBatch) initializeSeparateBatch(channelCount);
	else if(mMode == MaximumBatch) initializeMaximumBatch(channelCount);
	else if(mMode == Interpolation) initializeInterpolation(channelCount);
}

void ViNeuralInterpolator::initializeIncremental(const int &channelCount)
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
	network->setLearningRate(0.01);
	//network->setLearningMomentum(0.1);
	network->setStopEpochs(1);
	network->setStopMse(0.000000);
	if(!network->isValid())
	{
		LOG("Invalid neural network.", QtFatalMsg);
		exit(-1);
	}

	mNetworks.append(network);
	mInput = new qreal[mInputs];
	mOutput.append(new qreal[mOutputs]);
}

void ViNeuralInterpolator::initializeRecurrent(const int &channelCount)
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
	network->setStructure(ViFann::Standard, neurons);
	network->setTraining(ViFann::Fixed, ViFann::Incremental);
	network->setActivation(ViFann::Elliot);
	network->setWeights(ViFann::Random);
	network->setLearningRate(0.01);
	//network->setLearningMomentum(0.0);
	network->setStopEpochs(1);
	network->setStopMse(0.000000);
	if(!network->isValid())
	{
		LOG("Invalid neural network.", QtFatalMsg);
		exit(-1);
	}

	mNetworks.append(network);
	mInput = new qreal[mInputs];
	mOutput.append(new qreal[mOutputs]);
}

void ViNeuralInterpolator::initializeSeparateBatch(const int &channelCount)
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

	for(int i = 1; i <= mOutputs; ++i)
	{
		neurons.last() = i;
		ViFann *network = new ViFann(false);
		network->setStructure(ViFann::Standard, neurons);
		network->setTraining(ViFann::Fixed, ViFann::RProp);
		network->setActivation(ViFann::Elliot);
		network->setWeights(ViFann::Random);
		network->setStopEpochs(mEpochs);
		network->setStopMse(0.000000);
		if(!network->isValid())
		{
			LOG("Invalid neural network.", QtFatalMsg);
			exit(-1);
		}

		mNetworks.append(network);
		mOutput.append(new qreal[i]);
	}
	mInput = new qreal[mInputs];
}

void ViNeuralInterpolator::initializeMaximumBatch(const int &channelCount)
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
	network->setTraining(ViFann::Fixed, ViFann::RProp);
	network->setActivation(ViFann::Elliot);
	network->setWeights(ViFann::Random);
	network->setStopEpochs(mEpochs);
	network->setStopMse(0.000000);
	if(!network->isValid())
	{
		LOG("Invalid neural network.", QtFatalMsg);
		exit(-1);
	}

	mNetworks.append(network);
	mOutput.append(new qreal[mOutputs]);
	mInput = new qreal[mInputs];
}

void ViNeuralInterpolator::initializeInterpolation(const int &channelCount)
{
	int i;
	clear();

	mOutputs = ViNoiseCreator::maximumNoiseSize();

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
		network->setStructure(ViFann::Standard, neurons);
		network->setActivation(ViFann::Elliot);
		network->setWeights(ViFann::Random);
		network->setTraining(ViFann::Fixed, ViFann::RProp);
		network->setStopEpochs(mEpochs);
		network->setStopMse(0.000000);
		if(!network->isValid())
		{
			LOG("Invalid neural network.", QtFatalMsg);
			exit(-1);
		}

		mNetworks.append(network);
		mOutput.append(new qreal[i]);
	}

	mTrainCount = 0;
}

bool ViNeuralInterpolator::interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel)
{
	if(mMode == Incremental) interpolateIncremental(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize, error, channel);
	else if(mMode == Recurrent) interpolateRecurrent(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize, error, channel);
	else if(mMode == SeparateBatch) interpolateSeparateBatch(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize, error, channel);
	else if(mMode == MaximumBatch) interpolateMaximumBatch(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize, error, channel);
	else if(mMode == Interpolation) interpolateInterpolation(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize, error, channel);
}

bool ViNeuralInterpolator::interpolateIncremental(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel)
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

	/*int count = qMin(mInputs, mOutputs);
	
	qreal inputMean = 0;
	for(counter = 0; counter < count; ++counter)
	{
		inputMean += (leftSamples + (leftSize - count))[counter];
	}
	inputMean /= count;
	
	qreal outputMean = 0;
	for(counter = 0; counter < count; ++counter)
	{
		outputMean += (outputSamples + (outputSize - count))[counter];
	}
	outputMean /= count;


	qreal r = 0;
	for(counter = 0; counter < count; ++counter)
	{
		r += ((leftSamples + (leftSize - count))[counter] - inputMean) * ((outputSamples + (outputSize - count))[counter] - outputMean);
	}

	qreal divide1 = 0, divide2 = 0;
	for(counter = 0; counter < count; ++counter)
	{
		divide1 += qPow((leftSamples + (leftSize - count))[counter] - inputMean, 2);
		divide2 += qPow((outputSamples + (outputSize - count))[counter] - outputMean, 2);
		//cout<<(leftSamples + (leftSize - count))[counter]<<"\t"<<(outputSamples + (outputSize - count))[counter]<<endl;
	}
	//cout<<endl<<endl;

	r /= (qSqrt(divide1) * qSqrt(divide2));

	static int c1 = 0;
	static qreal rr1 = 0;
	static int c2 = 0;
	static qreal rr2= 0;

	if(qIsNaN(r)) return;

	if(r < 0)
	{
		++c1;
		rr1 += r;
	}
	else
	{
		++c2;
		rr2 += r;
	}

	cout <<" pppppppppppppppppppppppppppppppp:   " << r << "\t\t"<< (rr1/c1)<<"\t"<< (rr2/c2) <<endl;
*/
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

bool ViNeuralInterpolator::interpolateRecurrent(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel)
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

bool ViNeuralInterpolator::interpolateSeparateBatch(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel)
{
	trainSeparateBatch(leftSamples, leftSize, rightSamples, rightSize, outputSize, 256, 6);

	ViFann *network = mNetworks[outputSize - 1];
	qreal *output = mOutput[outputSize - 1];
	network->run(leftSamples + (leftSize - mInputs), output);
	for(int i = 0; i < outputSize; ++i)
	{
		outputSamples[i] = output[i];
	}
}

bool ViNeuralInterpolator::interpolateMaximumBatch(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel)
{
	trainMaximumBatch(leftSamples, leftSize, rightSamples, rightSize, outputSize, 256, 8);

	ViFann *network = mNetworks.first();
	qreal *output = mOutput.first();
	network->run(leftSamples + (leftSize - mInputs), output);
	for(int i = 0; i < outputSize; ++i)
	{
		outputSamples[i] = output[i];
	}

/*









	static int c1 = 0;
	static qreal rr1 = 0;
	static int c2 = 0;
	static qreal rr2= 0;

	int leftOffset, windowSize, halfSteps, counter;
	int trainCount = 2048;
	int stepSize = 1;

	counter = 0;
	windowSize = mInputs + mOutputs;
	if(trainCount > 0)
	{
		halfSteps = trainCount * stepSize;
		leftOffset = leftSize - halfSteps - windowSize;
	}
	if(trainCount <= 0 || leftOffset < 0)
	{
		trainCount = floor((leftSize - windowSize) / qreal(stepSize));
		halfSteps = trainCount * stepSize;
		leftOffset = leftSize - halfSteps - windowSize;
	}


	while(devideBatch((qreal*) leftSamples, leftSize, leftOffset, mInput, mInputs, output, mOutputs))
	{
		leftOffset += stepSize;
		++counter;

		int count = qMin(mInputs, mOutputs);

		qreal inputMean = 0;
		for(int i = 0; i < count; ++i)
		{
			inputMean += mInput[i];
		}
		inputMean /= count;

		qreal outputMean = 0;
		for(int i = 0; i < count; ++i)
		{
			outputMean += output[i];
		}
		outputMean /= count;


		qreal r = 0;
		for(int i = 0; i < count; ++i)
		{
			r += (mInput[i] - inputMean) * (output[i] - outputMean);
		}

		qreal divide1 = 0, divide2 = 0;
		for(int i = 0; i < count; ++i)
		{
			divide1 += qPow(mInput[i] - inputMean, 2);
			divide2 += qPow(output[i] - outputMean, 2);
		}

		r /= (qSqrt(divide1) * qSqrt(divide2));

		if(qIsNaN(r)) return;
		if(r < 0)
		{
			++c1;
			rr1 += r;
		}
		else
		{
			++c2;
			rr2 += r;
		}

	}






	cout <<" pppppppppppppppppppppppppppppppp:\t\t" << "\t\t"<< (rr1/c1)<<"\t"<< (rr2/c2) <<endl;*/
}

void ViNeuralInterpolator::trainSeparateBatch(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, int trainCount, const int &stepSize)
{
	int leftOffset, windowSize, halfSteps, counter;

	ViFann *network = mNetworks[outputSize - 1];
	qreal *output = mOutput[outputSize - 1];

	counter = 0;
	windowSize = mInputs + outputSize;
	if(trainCount > 0)
	{
		halfSteps = trainCount * stepSize;
		leftOffset = leftSize - halfSteps - windowSize;
	}
	if(trainCount <= 0 || leftOffset < 0)
	{
		trainCount = floor((leftSize - windowSize) / qreal(stepSize));
		halfSteps = trainCount * stepSize;
		leftOffset = leftSize - halfSteps - windowSize;
	}

	network->createTrain(trainCount);
	while(devideBatch((qreal*) leftSamples, leftSize, leftOffset, mInput, mInputs, output, outputSize))
	{
		network->setTrainInput(counter, mInput);
		network->setTrainOutput(counter, output);
		leftOffset += stepSize;
		++counter;
	}
	network->train();
	network->deleteTrain();
}

void ViNeuralInterpolator::trainMaximumBatch(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, int trainCount, const int &stepSize)
{
	int leftOffset, windowSize, halfSteps, counter;

	ViFann *network = mNetworks.first();
	qreal *output = mOutput.first();

	counter = 0;
	windowSize = mInputs + mOutputs;
	if(trainCount > 0)
	{
		halfSteps = trainCount * stepSize;
		leftOffset = leftSize - halfSteps - windowSize;
	}
	if(trainCount <= 0 || leftOffset < 0)
	{
		trainCount = floor((leftSize - windowSize) / qreal(stepSize));
		halfSteps = trainCount * stepSize;
		leftOffset = leftSize - halfSteps - windowSize;
	}

	network->createTrain(trainCount);
	while(devideBatch((qreal*) leftSamples, leftSize, leftOffset, mInput, mInputs, output, mOutputs))
	{
		network->setTrainInput(counter, mInput);
		network->setTrainOutput(counter, output);
		leftOffset += stepSize;
		++counter;
	}
	network->train();
	network->deleteTrain();
}

bool ViNeuralInterpolator::devideBatch(const qreal *samples, const int &sampleSize, const int &offset, qreal *input, const int &leftInputs, qreal *output, const int &outputSize)
{
	if(offset + leftInputs + outputSize >= sampleSize) return false;
	static size_t size = sizeof(qreal);

	// Copy left inputs
	memcpy(input, samples + offset, size * leftInputs);

	// Copy outputs
	memcpy(output, samples + offset + leftInputs, size * outputSize);

	return true;
}

bool ViNeuralInterpolator::interpolateInterpolation(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel)
{
	train(leftSamples, leftSize, rightSamples, rightSize, outputSize, 768, 8); // 768, 8
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

void ViNeuralInterpolator::printTrainMse()
{
	ViFann::printTrainMse();
}
