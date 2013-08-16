#ifndef VISPLINEINTERPOLATOR_H
#define VISPLINEINTERPOLATOR_H

#include <viinterpolator.h>

class ViSplineInterpolator : public ViInterpolator
{

	public:

		ViSplineInterpolator();

		// Eg: order-2 = quadratic
		// Eg: order-3 = qubic
		void setOrder(const int &order);
		int order();

		virtual ViSplineInterpolator* clone();

	protected:

		virtual bool interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);

		int xValue(const int &number);
		qreal yValue(const int &number);

		qreal calculateMultiplier(const int &derivative, const int &parameterNumber);

	private:

		int mOrder;
		int mLeftSize;
		int mRightSize;
		int mOutputSize;
		qreal *mLeftSamples;
		qreal *mRightSamples;

};

#endif
