#include <vimahalanobisnoisedetector.h>
#include <vigretl.h>

#define DEFAULT_WINDOW_SIZE 64
#define AMPLIFIER 10

ViMahalanobisNoiseDetector::ViMahalanobisNoiseDetector()
	: ViNoiseDetector()
{
	ViGretl::initialize();
	setWindowSize(DEFAULT_WINDOW_SIZE);
}

void ViMahalanobisNoiseDetector::setWindowSize(int size)
{
	mWindowSize = size;
	setOffset((mWindowSize / 2) + 1);
}

void ViMahalanobisNoiseDetector::setParameters(qreal param1)
{
	setWindowSize(param1);
}

void ViMahalanobisNoiseDetector::calculateNoise(QQueue<qreal> &samples)
{
	static int i;

	while(samples.size() >= mWindowSize)
	{
		DATASET *mGretlData;
		mGretlData = create_new_dataset(2, mWindowSize, 0);
		for(i = 0; i < mWindowSize; ++i)
		{
			mGretlData->Z[1][i] = samples[i];
		}

		int *mGretlParameters = gretl_list_new(1);
		mGretlParameters[1] = 1;
		int err;

		MahalDist *dis = get_mahal_distances(mGretlParameters, mGretlData, OPT_NONE, NULL, &err);
		setNoise(dis->d[(mWindowSize / 2) + 1] / 5);

		free_mahal_dist(dis);
		destroy_dataset(mGretlData);
		free(mGretlParameters);

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
