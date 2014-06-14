#ifndef VINEWTONINTERPOLATOR_H
#define VINEWTONINTERPOLATOR_H

#include <viinterpolator.h>
#include <vieigen.h>

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

		void setType();

		bool interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error = NULL);

		class ViNewtonTypedBase;

	private:

		ViEigenBase *mEigen;
		ViNewtonTypedBase *mTyped;

};

#endif
