#include <viprimitiveinterpolator.h>
#include <vimath.h>

ViPrimitiveInterpolator::ViPrimitiveInterpolator(const Mode &mode)
	: ViInterpolator()
{
	mMode = mode;
	if(mMode == Zero) interpolatePointer = &ViPrimitiveInterpolator::interpolateZero;
	else if(mMode == Cosine) interpolatePointer = &ViPrimitiveInterpolator::interpolateCosine;
	else if(mMode == Random)
	{
		interpolatePointer = &ViPrimitiveInterpolator::interpolateRandom;
		addParameterName("Window Size");
	}
}

QString ViPrimitiveInterpolator::name(QString replace, bool spaced)
{
	QString mode;

	if(mMode == Zero) mode = "Zero";
	else if(mMode == Random) mode = "Random";
	else if(mMode == Cosine) mode = "Cosine";

	QString name = ViInterpolator::name(replace, spaced) + " (" + mode + ")";
	if(spaced) return name;
	else return name.replace(" ", "");
}

void ViPrimitiveInterpolator::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else
	{
		LOG("Invalid parameter for this interpolator.", QtCriticalMsg);
		exit(-1);
	}
}

bool ViPrimitiveInterpolator::validParameters()
{
	return true;
}

bool ViPrimitiveInterpolator::interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel)
{
	(this->*interpolatePointer)(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize, error);
	return true;
}

void ViPrimitiveInterpolator::interpolateZero(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error)
{
	static int i;
	for(i = 0; i < outputSize; ++i) outputSamples[i] = 0;
}

void ViPrimitiveInterpolator::interpolateRandom(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error)
{
	ViMath<qreal>::noise(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize);
}

void ViPrimitiveInterpolator::interpolateCosine(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error)
{
	static qreal currentRatio, ratio;
	static int i;

	ratio = 1.0 / (outputSize + 1);
	for(i = 0; i < outputSize; ++i)
	{
		currentRatio = (1 - cos((ratio * (i + 1)) * M_PI)) / 2;
		outputSamples[i] = (leftSamples[leftSize - 1] * (1 - currentRatio)) + (rightSamples[0] * currentRatio);
	}
}

ViPrimitiveInterpolator* ViPrimitiveInterpolator::clone()
{
	return new ViPrimitiveInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViPrimitiveInterpolator* create()
{
	return new ViPrimitiveInterpolator();
}

#ifdef __cplusplus
}
#endif
