#include "viaverager.h"

ViAverager::ViAverager()
{
	clear();
}

void ViAverager::add(qreal value)
{
	++mCounter;
	mAverage = (mAverage * (mCounter - 1) + value) / mCounter;
}

qreal ViAverager::average()
{
	return mAverage;
}

void ViAverager::clear()
{
	mCounter = 0;
	mAverage = 0;
}

qreal ViAverager::average(QList<qreal> values)
{
	qreal result = 0;
	for(int i = 0; i < values.size(); ++i)
	{
		result += values[i];
	}
	return result / values.size();
}
