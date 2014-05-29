#include <vilagrangepredictor.h>
#include <vilogger.h>

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

bool ViLagrangePredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount)
{
	static int i, j, k;
	static qreal value1, value2, x, scaling, scaledJ, scaleK;

	scaling = size + predictionCount - 1;

	for(i = 0; i < predictionCount; ++i)
	{
		x = (size + i) / scaling;
		value1 = 0;
		for(j = 0; j < size; ++j)
		{
			scaledJ = j / scaling;
			value2 = 1;
			for(k = 0; k < size; ++k)
			{
				if(j != k)
				{
					scaleK = k / scaling;
					value2 *= (x - scaleK) / (scaledJ - scaleK);
				}
			}
			value1 += samples[j] * value2;
		}
		predictedSamples[i] = value1;
	}

	return true;
}
