#ifndef VISIGMOIDACTIVATIONFUNCTION_H
#define VISIGMOIDACTIVATIONFUNCTION_H

#include <viactivationfunction.h>

/*
	http://brange.me/2012/06/27/the-sigmoid-or-activation-function-and-their-uses/
*/

class ViSigmoidActivationFunction : public ViActivationFunction
{

	public:

		ViSigmoidActivationFunction(double shape = 1);

		void setRange(double minimum, double maximum);
		void setMinimum(double minimum);
		void setMaximum(double maximum);

		double shape();

		void setShape(double shape);

	protected:

		void changeFunction();

		double changeNormal(const double &input);
		double changeCenter(const double &input);
		double changeShape(const double &input);
		double changeCenterShape(const double &input);

		double execute(double input);

	private:

		double (ViSigmoidActivationFunction::*function)(const double &input);

		double mShape;
		double mCenter;

};

#endif
