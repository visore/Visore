#include <vimahalanobisnoisedetector.h>

#define DEFAULT_WINDOW_SIZE 64

ViMahalanobisNoiseDetector::ViMahalanobisNoiseDetector()
	: ViNoiseDetector()
{
	ViGretl::initialize();
	mGretlData = NULL;

	setScale(10);
	setWindowSize(DEFAULT_WINDOW_SIZE);
	addParameter("Window Size");

	mGretlParameters = gretl_list_new(1);
	mGretlParameters[1] = 1;

	QObject::connect(this, SIGNAL(parameterChanged(QString,qreal)), this, SLOT(changeParameter(QString,qreal)));
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

bool ViMahalanobisNoiseDetector::validParameters()
{
	return parameter("Window Size") > 0;
}

void ViMahalanobisNoiseDetector::changeParameter(QString name, qreal value)
{
	if(name == "Window Size") setWindowSize(value);
	else
	{
		LOG("The noise detector doe not have a parameter named " + name + ".", QtFatalMsg);
		exit(-1);
	}
}

void ViMahalanobisNoiseDetector::detect(QVector<qreal> &samples, QVector<qreal> &noise, const int &channel)
{
	static int i, error;

	while(samples.size() >= mWindowSize)
	{
		for(i = 0; i < mWindowSize; ++i) mGretlData->Z[1][i] = samples[i];

		MahalDist *distance = get_mahal_distances(mGretlParameters, mGretlData, OPT_NONE, NULL, &error);

		if(error) noise.append(0);
		else noise.append(distance->d[mHalfWindowSize]);

		free_mahal_dist(distance);
		samples.removeFirst();
	}
}
