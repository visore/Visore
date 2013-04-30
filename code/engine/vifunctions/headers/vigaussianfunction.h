#ifndef VIGAUSSIANFUNCTION_H
#define VIGAUSSIANFUNCTION_H

#include <vifunction.h>

/*
http://www.matrixlab-examples.com/normal-distribution-calculator.html
*/

class ViGaussianFunction : public ViFunction
{

	public:

		ViGaussianFunction();
		ViGaussianFunction(qreal height, qreal center, qreal deviation);
		ViGaussianFunction(const ViGaussianFunction &other);
		virtual ~ViGaussianFunction();
		
		virtual void setHeight(qreal height);
		virtual void setMean(qreal center);
		virtual void setDeviation(qreal deviation);

		qreal height();
		qreal mean();
		qreal deviation();

		qreal calculate(qreal x);

	private:

		qreal mHeight;
		qreal mMean;
		qreal mDeviation;

		//Additional variable to save computational time
		qreal mMinusTwoTimesDeviationSquared;

};

class ViGaussianDistribution : public ViGaussianFunction
{

	public:

		ViGaussianDistribution();
		ViGaussianDistribution(qreal center, qreal deviation);
		ViGaussianDistribution(const ViGaussianDistribution &other);

		void setHeight(qreal height);
		void setDeviation(qreal deviation);

};

#endif
