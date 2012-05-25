#ifndef VIFOURIERFIXEDCALCULATOR_H
#define VIFOURIERFIXEDCALCULATOR_H

#include <qmath.h> 
#include "FFTRealFixLen.h"
#include "vifouriercalculator.h"

template <int T>
class ViFourierFixedCalculator : public ViFourierCalculator
{

	public:
		
		ViFourierFixedCalculator();
		void forward();
		void inverse();
		void rescale();

	protected:

		ffft::FFTRealFixLen<T> mFourierTransform;

};

#include "../sources/vifourierfixedcalculator.cpp"

#endif
