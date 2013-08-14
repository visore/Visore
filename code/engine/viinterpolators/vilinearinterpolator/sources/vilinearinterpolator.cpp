#include <vilinearinterpolator.h>
#include<vilogger.h>
#define DECAY 0.9

ViLinearInterpolator::~ViLinearInterpolator()
{
}

bool ViLinearInterpolator::interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	if(leftSize == 0)
	{
		if(rightSize == 0) return false;

		qreal value = rightValue(rightSamples, rightSize);
		qreal delta = rightDelta(rightSamples, rightSize);
		int i = outputSize - 1;
		while(i >= 0)
		{
			value += delta;
			outputSamples[i] = value;
			value *= DECAY;
			delta *= DECAY;
			--i;
		}
	}
	else if(rightSize == 0)
	{
		qreal value = leftValue(leftSamples, leftSize);
		qreal delta = leftDelta(leftSamples, leftSize);
		int i = 0;
		while(i < outputSize)
		{
			value += delta;
			outputSamples[i] = value;
			value *= DECAY;
			delta *= DECAY;
			++i;
		}
	}
	else
	{
		qreal value = middleValue(leftSamples, leftSize, rightSamples, rightSize);
		qreal delta = middleDelta(leftSamples, leftSize, rightSamples, rightSize, outputSize);
		LOG("qqq: "+QString::number(value)+" "+QString::number(delta)+ " "+QString::number(leftDelta(leftSamples, leftSize))+" "+QString::number(rightDelta(rightSamples, rightSize)));
		int i = 0;
		while(i < outputSize)
		{
			value += delta;
			outputSamples[i] = value;
			++i;
		}
	}
	return true;
}

qreal ViLinearInterpolator::leftValue(const qreal *leftSamples, const int &leftSize)
{
	return leftSamples[leftSize - 1];
}

qreal ViLinearInterpolator::rightValue(const qreal *rightSamples, const int &rightSize)
{
	return rightSamples[0];
}

qreal ViLinearInterpolator::middleValue(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize)
{
	return leftSamples[leftSize - 1];
}

qreal ViLinearInterpolator::leftDelta(const qreal *leftSamples, const int &leftSize)
{
	if(leftSize < 2) return 0;
	return leftSamples[leftSize - 1] - leftSamples[leftSize - 2];
}

qreal ViLinearInterpolator::rightDelta(const qreal *rightSamples, const int &rightSize)
{
	if(rightSize < 2) return 0;
	return rightSamples[0] - rightSamples[1];
}

qreal ViLinearInterpolator::middleDelta(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize)
{
	return (rightSamples[0] - leftSamples[leftSize - 1]) / (outputSize + 1);
}

ViLinearInterpolator* ViLinearInterpolator::clone()
{
	return new ViLinearInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViLinearInterpolator* create()
{
	return new ViLinearInterpolator();
}

#ifdef __cplusplus
}
#endif
