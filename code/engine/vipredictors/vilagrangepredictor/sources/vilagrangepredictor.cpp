#include <vilagrangepredictor.h>
#include <vilogger.h>
#include <vieigen.h>

ViLagrangePredictor::ViLagrangePredictor()
	: ViPredictor()
{
	addParameterName("Window Size");
}

ViLagrangePredictor::ViLagrangePredictor(const ViLagrangePredictor &other)
	: ViPredictor(other)
{
}

ViLagrangePredictor::~ViLagrangePredictor()
{
}

void ViLagrangePredictor::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else
	{
		LOG("Invalid parameter for this predictor.", QtCriticalMsg);
		exit(-1);
	}
}

bool ViLagrangePredictor::validParameters()
{
	return mWindowSize > 1;
}

bool ViLagrangePredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViError *error)
{
	/*static int i, j, k;
	static mpfr::mpreal value1, value2, x, scaling;

	scaling = size + predictionCount - 1;

	mpfr::mpreal xValues[size];
	for(i = 0; i < size; ++i) xValues[i] = i / scaling;

	for(i = 0; i < predictionCount; ++i)
	{
		x = (size + i) / scaling;
		value1 = 0;
		for(j = 0; j < size; ++j)
		{
			value2 = 1;
			for(k = 0; k < size; ++k)
			{
				if(j != k) value2 *= (x - xValues[k]) / (xValues[j] - xValues[k]);
			}
			value1 += samples[j] * value2;
		}
		predictedSamples[i] = value1.toDouble();
	}*/

	static int i, j, k;
	static qreal value1, value2, x, scaling;

	scaling = size + predictionCount - 1;

	qreal xValues[size];
	for(i = 0; i < size; ++i) xValues[i] = i / scaling;

	for(i = 0; i < predictionCount; ++i)
	{
		x = (size + i) / scaling;
		value1 = 0;
		for(j = 0; j < size; ++j)
		{
			value2 = 1;
			for(k = 0; k < size; ++k)
			{
				if(j != k) value2 *= (x - xValues[k]) / (xValues[j] - xValues[k]);
			}
			value1 += samples[j] * value2;
		}
		predictedSamples[i] = value1;
	}

	//if(error != NULL) error

	return true;
}
