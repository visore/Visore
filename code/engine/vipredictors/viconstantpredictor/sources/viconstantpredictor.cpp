#include <viconstantpredictor.h>
#include <vimath.h>
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
	ViMath<qreal>::noise(samples, size, predictedSamples, predictionCount);
}
