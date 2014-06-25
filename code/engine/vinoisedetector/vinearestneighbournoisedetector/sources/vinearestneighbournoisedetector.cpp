#include <vinearestneighbournoisedetector.h>

#define DEFAULT_K 32

ViNearestNeighbourNoiseDetector::ViNearestNeighbourNoiseDetector()
	: ViNoiseDetector()
{
	setScale(2);
	setK(DEFAULT_K);
	addParameter("K");
}

ViNearestNeighbourNoiseDetector::ViNearestNeighbourNoiseDetector(const ViNearestNeighbourNoiseDetector &other)
	: ViNoiseDetector(other)
{
	mK = other.mK;
	mHalfK = other.mHalfK;
}

void ViNearestNeighbourNoiseDetector::setK(const int &k)
{
	mK = k;
	mHalfK = qFloor(mK / 2.0);
	setOffset(mHalfK);
}

bool ViNearestNeighbourNoiseDetector::validParameters()
{
	return parameter("K") > 0;
}

void ViNearestNeighbourNoiseDetector::initialize()
{
	setK(parameter("K"));
}

void ViNearestNeighbourNoiseDetector::detect(QVector<qreal> &samples, QVector<qreal> &noise)
{
	static int i;
	static qreal totalDifference;

	while(samples.size() >= mK)
	{
		const qreal &currentValue = samples[mHalfK];
		totalDifference = 0;

		for(i = 0; i < mHalfK; ++i) totalDifference += abs(currentValue - samples[i]);
		for(i = mHalfK + 1; i < mK; ++i) totalDifference += abs(currentValue - samples[i]);

		// Devide by mK, since we want the mean of all differences
		// Devide 2, since the maximum distance between samples can be 2
		noise.append(totalDifference / mK);

		samples.removeFirst();
	}
}
