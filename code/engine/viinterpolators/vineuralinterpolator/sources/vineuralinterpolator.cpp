#include <vineuralinterpolator.h>

#define DEFAULT_WINDOW_SIZE 32

ViNeuralInterpolator::ViNeuralInterpolator(ViFann *network)
	: ViInterpolator()
{
	mNetwork = network;
	mInput = NULL;
	setWindowSize(DEFAULT_WINDOW_SIZE);

	addParameterName("Window Size");
}

ViNeuralInterpolator::ViNeuralInterpolator(const ViNeuralInterpolator &other)
	: ViInterpolator(other)
{
	mNetwork = new ViFann(*other.mNetwork);
	mInput = NULL;
	setWindowSize(other.mWindowSize);
}

ViNeuralInterpolator::~ViNeuralInterpolator()
{
	delete mNetwork;
	delete [] mInput;
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
	if(mInput != NULL) delete [] mInput;
	mInput = new qreal[mWindowSize];
}

void ViNeuralInterpolator::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
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

bool ViNeuralInterpolator::interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error)
{
	// Train with the uncorrupted samples
	int offset = 0;

	/*while(devide(leftSamples, leftSize, outputSize, offset, mInput, mWindowSize, mOutput, outputSize))
	{
		++offset;
		mNetwork->train(mInput, mOutput);
	}*/
}

bool ViNeuralInterpolator::devide(qreal *samples, const int &sampleSize, const int &offset, qreal *input, const int &windowSize, qreal *output, const int &outputSize)
{
	if(offset + (2 * windowSize) + outputSize > sampleSize) return false;
	static int i, j, end;

	// Read left inputs
	for(i = 0; i < windowSize; ++i) input[i] = samples[i + offset];

	// Read outputs
	output = samples + offset + windowSize;

	// Read right inputs
	end = 2 * windowSize;
	j = offset + outputSize;
	for(i = windowSize; i < end; ++i) input[i] = samples[i + j];

	return true;
}
