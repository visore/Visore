#include <vipredictor.h>
#include <vilogger.h>

ViPredictor::ViPredictor()
{
	mMinimumSamples = 1;
}

ViPredictor::ViPredictor(const int &minimumSamples)
{
	mMinimumSamples = minimumSamples;
}

ViPredictor::ViPredictor(const ViPredictor &other)
{
	mMinimumSamples = other.mMinimumSamples;
}

ViPredictor::~ViPredictor()
{
}

qreal ViPredictor::predict(const ViSampleChunk &samples)
{
	return predict(samples, samples.size());
}

qreal ViPredictor::predict(const ViSampleChunk &samples, const int &index)
{
	if(enoughData(samples, index))
	{
		return predictValue(samples.data(), index);
	}
	return 0;
}

QList<qreal> ViPredictor::predict(const int &length, const ViSampleChunk &samples)
{
	return predict(length, samples, samples.size());
}

QList<qreal> ViPredictor::predict(const int &length, const ViSampleChunk &samples, const int &index)
{
	if(enoughData(samples, index))
	{
		return predictValues(samples.data(), index, length);
	}
	return QList<qreal>();
}

void ViPredictor::setMinimumSamples(const int &samples)
{
	mMinimumSamples = samples;
}

bool ViPredictor::enoughData(const ViSampleChunk &samples, const int &startIndex) const
{
	if(startIndex >= mMinimumSamples)
	{
		return true;
	}
	LOG("Not enough data available.", QtCriticalMsg);
	return false;
}
