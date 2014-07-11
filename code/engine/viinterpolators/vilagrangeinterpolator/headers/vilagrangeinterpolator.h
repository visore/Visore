#ifndef VILAGRANGEINTERPOLATOR_H
#define VILAGRANGEINTERPOLATOR_H

#include <viinterpolator.h>

class ViLagrangeInterpolator : public ViInterpolator
{

	public:

		ViLagrangeInterpolator();
		ViLagrangeInterpolator(const ViLagrangeInterpolator &other);
		~ViLagrangeInterpolator();

		void setParameter(const int &number, const qreal &value);
		bool validParameters();

		ViLagrangeInterpolator* clone();

	protected:

		bool validParameters(const int &windowSize);
		bool validParameters(const int &leftSize, const int &rightSize);

		bool interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel);

		void calculate(const qreal *x, const qreal *y, const int &size, qreal *output, const int &outputSize, const int &startX, const qreal &scaling);
};

#endif
