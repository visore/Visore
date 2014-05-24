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

void ViLagrangePredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount)
{
	for(mI = 0; mI < predictionCount; ++mI)
	{
		mX = size + mI;
		mValue1 = 0;
		for(mJ = 0; mJ < size; ++mJ)
		{
			mValue2 = 1;
			for(mK = 0; mK < size; ++mK)
			{
				if(mJ != mK) mValue2 *= (mX - mK) / qreal(mJ - mK);
			}
			mValue1 += samples[mJ] * mValue2;
		}
		predictedSamples[mI] = mValue1;
	}
}
