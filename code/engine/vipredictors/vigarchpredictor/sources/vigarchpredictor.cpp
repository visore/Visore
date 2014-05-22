#include <vigarchpredictor.h>
#include <vilogger.h>

#define DEFAULT_WINDOW_SIZE 1024

ViGarchPredictor::ViGarchPredictor()
	: ViPredictor()
{
	mGretl = new ViGretl(ViGretl::GARCH);
	setWindowSize(DEFAULT_WINDOW_SIZE);

	addParameterName("Window Size");
	addParameterName("GARCH Order");
	addParameterName("ARCH Order");
}

ViGarchPredictor::ViGarchPredictor(const ViGarchPredictor &other)
	: ViPredictor(other)
{
	mGretl = new ViGretl(*other.mGretl);
	setWindowSize(DEFAULT_WINDOW_SIZE);
}

ViGarchPredictor::~ViGarchPredictor()
{
}

void ViGarchPredictor::setWindowSize(const int &size)
{
	int windowSize = size;
	mGretl->adjustWindowSize(windowSize);
	ViPredictor::setWindowSize(windowSize);
}

void ViGarchPredictor::setDegree(const ViGretl::Type &type, const int &degree)
{
	mGretl->setDegree(type, degree);
}

void ViGarchPredictor::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else if(number == 1) setDegree(ViGretl::GARCH, value);
	else if(number == 2) setDegree(ViGretl::ARCH, value);
	else
	{
		LOG("Invalid parameter for this predictor.", QtCriticalMsg);
		exit(-1);
	}
}

void ViGarchPredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount)
{
	mGretl->forecast(samples, size, predictedSamples, predictionCount);
}
