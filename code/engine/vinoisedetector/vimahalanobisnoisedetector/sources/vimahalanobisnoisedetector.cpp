#include <vimahalanobisnoisedetector.h>

#define DEFAULT_WINDOW_SIZE 64
#define AMPLIFIER 0.2

ViMahalanobisNoiseDetector::ViMahalanobisNoiseDetector()
	: ViNoiseDetector()
{
	ViGretl::initialize();
	mGretlData = NULL;
	setWindowSize(DEFAULT_WINDOW_SIZE);

	setAmplification(AMPLIFIER);

	addParameterName("Window Size");

	mGretlParameters = gretl_list_new(1);
	mGretlParameters[1] = 1;
}

ViMahalanobisNoiseDetector::ViMahalanobisNoiseDetector(const ViMahalanobisNoiseDetector &other)
	: ViNoiseDetector(other)
{
	ViGretl::initialize();
	mGretlData = NULL;
	setWindowSize(other.mWindowSize);

	mGretlParameters = gretl_list_new(1);
	mGretlParameters[1] = 1;
}

ViMahalanobisNoiseDetector::~ViMahalanobisNoiseDetector()
{
	free(mGretlParameters);
	destroy_dataset(mGretlData);
}

void ViMahalanobisNoiseDetector::setWindowSize(int size)
{
	mWindowSize = size;
	mHalfWindowSize = qFloor(mWindowSize / 2.0);
	setOffset(mHalfWindowSize);

	if(mGretlData != NULL) destroy_dataset(mGretlData);
	mGretlData = create_new_dataset(2, mWindowSize, 0);
}

void ViMahalanobisNoiseDetector::setParameters(const qreal &param1)
{
	setWindowSize(param1);
}

void ViMahalanobisNoiseDetector::calculateNoise(QQueue<qreal> &samples)
{
	static int i, error;
	static MahalDist *distance;

	while(samples.size() >= mWindowSize)
	{
		for(i = 0; i < mWindowSize; ++i) mGretlData->Z[1][i] = samples[i];

		distance = get_mahal_distances(mGretlParameters, mGretlData, OPT_NONE, NULL, &error);

		if(error) setNoise(0);
		else
		{
			setNoise(distance->d[mHalfWindowSize]);
			free_mahal_dist(distance);
		}

		samples.removeFirst();
	}
}

ViMahalanobisNoiseDetector* ViMahalanobisNoiseDetector::clone()
{
	return new ViMahalanobisNoiseDetector(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViMahalanobisNoiseDetector* create()
{
	return new ViMahalanobisNoiseDetector();
}

#ifdef __cplusplus
}
#endif
