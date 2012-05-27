#ifndef VIFOURIERFIXEDCALCULATOR_H
#define VIFOURIERFIXEDCALCULATOR_H

#include "FFTRealFixLen.h"
#include "vifouriercalculator.h"

template <int T>
class ViFourierFixedCalculator : public ViFourierCalculator
{

	public:
		
		ViFourierFixedCalculator();
		void setSize(const int size);
		void forward();
		void inverse();
		void rescale();

	protected:

		ffft::FFTRealFixLen<T> mFourierTransform;

};

#endif
