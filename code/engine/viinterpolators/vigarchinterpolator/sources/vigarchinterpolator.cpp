#include <vigarchinterpolator.h>
#include <vilogger.h>

ViGarchInterpolator::ViGarchInterpolator(const Estimation &estimation)
	: ViInterpolator()
{
	mGretl = new ViGretl(ViGretl::GARCH);
	setEstimation(estimation);

	addParameterName("Window Size");
	addParameterName("Arch Degree");
	addParameterName("Garch Degree");
}

ViGarchInterpolator::ViGarchInterpolator(const ViGarchInterpolator &other)
	: ViInterpolator(other)
{
	mGretl = new ViGretl(*other.mGretl);
	setEstimation(other.mEstimation);
}

ViGarchInterpolator::~ViGarchInterpolator()
{
}

void ViGarchInterpolator::setEstimation(const Estimation &estimation)
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

QString ViGarchInterpolator::name(QString replace, bool spaced)
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

void ViGarchInterpolator::setDegree(const int &archDegree, const int &garchDegree)
{
	setArchDegree(archDegree);
	setGarchDegree(garchDegree);
}

void ViGarchInterpolator::setArchDegree(const int &degree)
{
	mGretl->setDegree(ViGretl::ARCH, degree);
}

void ViGarchInterpolator::setGarchDegree(const int &degree)
{
	mGretl->setDegree(ViGretl::GARCH, degree);
}

void ViGarchInterpolator::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else if(number == 1) setArchDegree(value);
	else if(number == 2) setGarchDegree(value);
	else
	{
		LOG("Invalid parameter for this interpolator.", QtCriticalMsg);
		exit(-1);
	}
}

bool ViGarchInterpolator::validParameters()
{
	return validParameters(mWindowSize, mGretl->degree(ViGretl::ARCH), mGretl->degree(ViGretl::GARCH));
}

bool ViGarchInterpolator::validParameters(const int &windowSize, const int &archDegree, const int &garchDegree)
{
	if(mWindowSize < 2) return false;
	if(archDegree == 0 && garchDegree == 0) return false;
	return true;
}

bool ViGarchInterpolator::validParameters(const int &leftSize, const int &rightSize, const int &archDegree, const int &garchDegree)
{
	return validParameters(leftSize + rightSize, archDegree, garchDegree);
}

bool ViGarchInterpolator::interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel)
{
	return mGretl->interpolate(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize, error);
}

ViGarchInterpolator* ViGarchInterpolator::clone()
{
	return new ViGarchInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViGarchInterpolator* create()
{
	return new ViGarchInterpolator();
}

#ifdef __cplusplus
}
#endif
