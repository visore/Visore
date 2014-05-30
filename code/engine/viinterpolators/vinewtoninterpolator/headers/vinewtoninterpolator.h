#ifndef VINEWTONINTERPOLATOR_H
#define VINEWTONINTERPOLATOR_H

#include <viinterpolator.h>
#include <vivector.h>

class ViNewtonInterpolator : public ViInterpolator
{

	public:

		ViNewtonInterpolator();
		ViNewtonInterpolator(const ViNewtonInterpolator &other);
		~ViNewtonInterpolator();

		void setParameter(const int &number, const qreal &value);
		bool validParameters();

		ViNewtonInterpolator* clone();

	protected:

		bool validParameters(const int &windowSize);
		bool validParameters(const int &leftSize, const int &rightSize);

		bool interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);

};

#endif
