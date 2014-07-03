#include <vinearestneighbournoisedetector.h>

#define DEFAULT_K 32

ViNearestNeighbourNoiseDetector::ViNearestNeighbourNoiseDetector()
	: ViNoiseDetector()
{
	setScale(2);
	setK(DEFAULT_K);
	addParameter("K");

	QObject::connect(this, SIGNAL(parameterChanged(QString,qreal)), this, SLOT(changeParameter(QString,qreal)));
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

void ViNearestNeighbourNoiseDetector::changeParameter(QString name, qreal value)
{
	if(name == "K") setK(value);
	else
	{
		LOG("The noise detector doe not have a parameter named " + name + ".", QtFatalMsg);
		exit(-1);
	}
}

void ViNearestNeighbourNoiseDetector::detect(QVector<qreal> &samples, QVector<qreal> &noise, const int &channel)
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
