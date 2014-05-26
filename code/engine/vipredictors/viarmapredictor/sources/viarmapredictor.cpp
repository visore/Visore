#include <viarmapredictor.h>
#include <vilogger.h>

#define DEFAULT_WINDOW_SIZE 1024

ViArmaPredictor::ViArmaPredictor()
	: ViPredictor()
{
	mGretl = new ViGretl(ViGretl::ARMA);
	setWindowSize(DEFAULT_WINDOW_SIZE);

	addParameterName("Window Size");
	addParameterName("AR Order");
	addParameterName("I Order");
	addParameterName("MA Order");
}

ViArmaPredictor::ViArmaPredictor(const ViArmaPredictor &other)
	: ViPredictor(other)
{
	mGretl = new ViGretl(*other.mGretl);
	setWindowSize(DEFAULT_WINDOW_SIZE);
}

ViArmaPredictor::~ViArmaPredictor()
{
}

void ViArmaPredictor::setWindowSize(const int &size)
{
	int windowSize = size;
	mGretl->adjustWindowSize(windowSize);
	ViPredictor::setWindowSize(windowSize);
}

void ViArmaPredictor::setDegree(const ViGretl::Type &type, const int &degree)
{
	mGretl->setDegree(type, degree);
}

void ViArmaPredictor::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else if(number == 1) setDegree(ViGretl::AR, value);
	else if(number == 2) setDegree(ViGretl::I, value);
	else if(number == 3) setDegree(ViGretl::MA, value);
	else
	{
		LOG("Invalid parameter for this predictor.", QtCriticalMsg);
		exit(-1);
	}
}

bool ViArmaPredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount)
{
	mGretl->forecast(samples, size, predictedSamples, predictionCount);
	return true;
}
