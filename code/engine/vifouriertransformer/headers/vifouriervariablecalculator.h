#ifndef VIFOURIERVARIABLECALCULATOR_H
#define VIFOURIERVARIABLECALCULATOR_H

#include "FFTReal.h"
#include "vifouriercalculator.h"

class ViFourierVariableCalculator : public ViFourierCalculator
{

	public:

		ViFourierVariableCalculator();
		~ViFourierVariableCalculator();
		void setSize(int size);
		void forward();
		void inverse();
		void rescale();

	protected:

		ffft::FFTReal<float> *mFourierTransform;

};

class ViFourierVariableForwardCalculator : public ViFourierVariableCalculator
{

	public:

		ViFourierVariableForwardCalculator();
		void calculate();

};

class ViFourierVariableInverseCalculator : public ViFourierVariableCalculator
{

	public:

		ViFourierVariableInverseCalculator();
		void calculate();

};

class ViFourierVariableRescaleCalculator : public ViFourierVariableCalculator
{

	public:

		ViFourierVariableRescaleCalculator();
		void calculate();

};

#endif
