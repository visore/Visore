#include <viconstantpredictor.h>
#include <visystemsolver.h>
#include <vilogger.h>

ViConstantPredictor::ViConstantPredictor(const Mode &mode)
	: ViPredictor()
{
	setMode(mode);
	addParameterName("Window Size");
}

ViConstantPredictor::ViConstantPredictor(const ViConstantPredictor &other)
	: ViPredictor(other)
{
	setMode(other.mMode);
}

ViConstantPredictor::~ViConstantPredictor()
{
}

QString ViConstantPredictor::name(QString replace, bool spaced)
{
	QString name = ViPredictor::name(replace, spaced) + " (";

	if(mMode == Zero) name += "Zero Mode";
	else if(mMode == Mean) name += "Mean Mode";
	else if(mMode == Last) name += "Last Mode";
	else if(mMode == Random) name += "Random Mode";

	name += ")";

	if(spaced) return name;
	else return name.replace(" ", "");
}

void ViConstantPredictor::setMode(const Mode &mode)
{
	mMode = mode;
	if(mMode == Zero) predictPointer = &ViConstantPredictor::predictZero;
	else if(mMode == Mean) predictPointer = &ViConstantPredictor::predictMean;
	else if(mMode == Last) predictPointer = &ViConstantPredictor::predictLast;
	else if(mMode == Random) predictPointer = &ViConstantPredictor::predictRandom;
}

void ViConstantPredictor::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else
	{
		LOG("Invalid parameter for this predictor.", QtCriticalMsg);
		exit(-1);
	}
}

bool ViConstantPredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount)
{
	(this->*predictPointer)(samples, size, predictedSamples, predictionCount);
	return true;
}

void ViConstantPredictor::predictZero(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount)
{
	static int i;
	for(i = 0; i < predictionCount; ++i) predictedSamples[i] = 0;
}

void ViConstantPredictor::predictMean(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount)
{
	static int i;
	static qreal mean;
	mean = 0;
	for(i = 0; i < size; ++i) mean += samples[i];
	mean /= size;
	for(i = 0; i < predictionCount; ++i) predictedSamples[i] = mean;
}

void ViConstantPredictor::predictLast(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount)
{
	static int i;
	static qreal last;
	last = samples[size - 1];
	for(i = 0; i < predictionCount; ++i) predictedSamples[i] = last;
}

void ViConstantPredictor::predictRandom(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount)
{
	static int i;
	static qreal mean, variance;
	mean = 0;
	for(i = 0; i < size; ++i) mean += samples[i];
	mean /= size;
	variance = 0;
	for(i = 0; i < size; ++i) variance += qPow(samples[i] - mean, 2);
	variance /= size + 1;
	for(i = 0; i < predictionCount; ++i) predictedSamples[i] = generateNoise(variance);
}

qreal ViConstantPredictor::generateNoise(const qreal &variance)
{
	static bool hasSpare = false;
	static qreal spare;

	if(hasSpare)
	{
		hasSpare = false;
		return spare * variance;
	}

	hasSpare = true;
	static qreal u, v, s;
	do
	{
		u = (rand() / ((double) RAND_MAX)) * 2 - 1;
		v = (rand() / ((double) RAND_MAX)) * 2 - 1;
		s = u * u + v * v;
	}
	while(s >= 1 || s == 0);

	s = qSqrt(-2.0 * qLn(s) / s);
	spare = v * s;
	return variance * u * s;
}
