#include <vineuralinterpolator.h>
#include <vimanager.h>

#define DEFAULT_WINDOW_SIZE 32

ViNeuralInterpolator::ViNeuralInterpolator()
	: ViInterpolator()
{
	mInput = NULL;
	setWindowSize(DEFAULT_WINDOW_SIZE);

	addParameterName("Window Size");

	addParameterName("l1");
	addParameterName("l2");
	addParameterName("l3");
	addParameterName("l4");
	addParameterName("l5");
	l1=l2=l3=l4=l5=0;
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
	/*QString mode;

	if(mMode == Zero) mode = "Zero";
	else if(mMode == Random) mode = "Random";
	else if(mMode == Cosine) mode = "Cosine";

	QString name = ViInterpolator::name(replace, spaced) + " (" + mode + ")";
	if(spaced) return name;
	else return name.replace(" ", "");*/
	return ViInterpolator::name(replace, spaced);
}

void ViNeuralInterpolator::setWindowSize(const int &size)
{
	mWindowSize = size;
	//if(mInput != NULL) delete [] mInput;
	//mInput = new qreal[mWindowSize];
}

void ViNeuralInterpolator::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);

	else if(number==1)l1=value;
	else if(number==2)l2=value;
	else if(number==3)l3=value;
	else if(number==4)l4=value;
	else if(number==5)l5=value;

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
}

void ViNeuralInterpolator::initialize(const int &channelCount)
{
	int i;
	clear();

	mOutputs = 50;

	mInputs = mWindowSize;
	mLeftInputs = ceil(mInputs / 2);
	mRightInputs = mInputs - mLeftInputs;

	QList<int> neurons;
	neurons.append(mInputs);
	if(l1>0)
	{
		neurons.append(l1);
		if(l2>0)
		{
			neurons.append(l2);
			if(l3>0)
			{
				neurons.append(l3);
				if(l4>0)
				{
					neurons.append(l4);
					if(l5>0)
					{
						neurons.append(l5);
					}
				}
			}
		}
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
		network->setStopEpochs(500);
		network->setStopMse(0.000000);
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
