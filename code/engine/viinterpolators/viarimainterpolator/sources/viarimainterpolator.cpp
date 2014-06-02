#include <viarimainterpolator.h>
#include <vilogger.h>

ViArimaInterpolator::ViArimaInterpolator(const Estimation &estimation)
	: ViInterpolator()
{
	mGretl = new ViGretl(ViGretl::ARIMA);
	setEstimation(estimation);

	addParameterName("Window Size");
	addParameterName("AR Degree");
	addParameterName("I Degree");
	addParameterName("MA Degree");
}

ViArimaInterpolator::ViArimaInterpolator(const ViArimaInterpolator &other)
	: ViInterpolator(other)
{
	mGretl = new ViGretl(*other.mGretl);
	setEstimation(other.mEstimation);
}

ViArimaInterpolator::~ViArimaInterpolator()
{
}

void ViArimaInterpolator::setEstimation(const Estimation &estimation)
{
	mEstimation = estimation;
	if(mEstimation == Fixed) mGretl->setCriteria(ViGretl::None);
	else if(mEstimation == MSE) mGretl->setCriteria(ViGretl::MSE);
	else if(mEstimation == AC) mGretl->setCriteria(ViGretl::AC);
	else if(mEstimation == AIC) mGretl->setCriteria(ViGretl::AIC);
	else if(mEstimation == AICC) mGretl->setCriteria(ViGretl::AICC);
	else if(mEstimation == BIC) mGretl->setCriteria(ViGretl::BIC);
	else if(mEstimation == HQC) mGretl->setCriteria(ViGretl::HQC);
}

QString ViArimaInterpolator::name(QString replace, bool spaced)
{
	QString estimation;

	if(mEstimation == Fixed) estimation = "Fixed Estimation";
	else if(mEstimation == MSE) estimation = "Best MSE Estimation";
	else if(mEstimation == AC) estimation = "Best AC Estimation";
	else if(mEstimation == AIC) estimation = "Best AIC Estimation";
	else if(mEstimation == AICC) estimation = "Best AICC Estimation";
	else if(mEstimation == BIC) estimation = "Best BIC Estimation";
	else if(mEstimation == HQC) estimation = "Best HQC Estimation";

	QString name = ViInterpolator::name(replace, spaced) + " (" + estimation + ")";
	if(spaced) return name;
	else return name.replace(" ", "");
}

void ViArimaInterpolator::setDegree(const int &arDegree, const int &iDegree, const int &maDegree)
{
	setArDegree(arDegree);
	setIDegree(iDegree);
	setMaDegree(maDegree);
}

void ViArimaInterpolator::setArDegree(const int &degree)
{
	mGretl->setDegree(ViGretl::AR, degree);
}

void ViArimaInterpolator::setIDegree(const int &degree)
{
	mGretl->setDegree(ViGretl::I, degree);
}

void ViArimaInterpolator::setMaDegree(const int &degree)
{
	mGretl->setDegree(ViGretl::MA, degree);
}

void ViArimaInterpolator::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else if(number == 1) setArDegree(value);
	else if(number == 2) setIDegree(value);
	else if(number == 3) setMaDegree(value);
	else
	{
		LOG("Invalid parameter for this interpolator.", QtCriticalMsg);
		exit(-1);
	}
}

bool ViArimaInterpolator::validParameters()
{
	return validParameters(mWindowSize, mGretl->degree(ViGretl::AR), mGretl->degree(ViGretl::I), mGretl->degree(ViGretl::MA));
}

bool ViArimaInterpolator::validParameters(const int &windowSize, const int &arDegree, const int &iDegree, const int &maDegree)
{
	if(windowSize < 2 || windowSize % 2 != 0) return false;
	if(arDegree == 0 && maDegree == 0) return false;
	return true;
}

bool ViArimaInterpolator::validParameters(const int &leftSize, const int &rightSize, const int &arDegree, const int &iDegree, const int &maDegree)
{
	return validParameters(leftSize + rightSize, arDegree, iDegree, maDegree);
}

bool ViArimaInterpolator::interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	static int i;
	static bool leftOk, rightOk;

	qreal leftOutput[outputSize];
	qreal rightOutput[outputSize];

	leftOk = mGretl->forecast(leftSamples, leftSize, leftOutput, outputSize);
	rightOk = mGretl->backcast(rightSamples, rightSize, rightOutput, outputSize);
	if(leftOk && rightOk)
	{
		for(i = 0; i < outputSize; ++i) outputSamples[i] = (leftOutput[i] + rightOutput[i]) / 2;
	}
	else if(leftOk)
	{
		for(i = 0; i < outputSize; ++i) outputSamples[i] = leftOutput[i];
	}
	else if(rightOk)
	{
		for(i = 0; i < outputSize; ++i) outputSamples[i] = rightOutput[i];
	}
	else
	{
		for(i = 0; i < outputSize; ++i) outputSamples[i] = 0;
		return false;
	}
	return true;
}

ViArimaInterpolator* ViArimaInterpolator::clone()
{
	return new ViArimaInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViArimaInterpolator* create()
{
	return new ViArimaInterpolator();
}

#ifdef __cplusplus
}
#endif
