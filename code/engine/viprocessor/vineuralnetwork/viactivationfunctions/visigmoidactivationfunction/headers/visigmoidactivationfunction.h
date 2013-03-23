#ifndef VISIGMOIDACTIVATIONFUNCTION_H
#define VISIGMOIDACTIVATIONFUNCTION_H

#include <viactivationfunction.h>

/*
	http://brange.me/2012/06/27/the-sigmoid-or-activation-function-and-their-uses/
*/

class ViSigmoidActivationFunction : public ViActivationFunction//public ViActivationFunctionType<ViSigmoidActivationFunction>
{

	public:

		ViSigmoidActivationFunction(double shape = 1);
		ViSigmoidActivationFunction(const ViSigmoidActivationFunction &other);
		~ViSigmoidActivationFunction(){}

		void setRange(double minimum, double maximum);
		void setMinimum(double minimum);
		void setMaximum(double maximum);

		double shape();

		void setShape(double shape);

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

		ViSigmoidActivationFunction* clone();

	protected:

		void changeFunction();

		double changeNormal(const double &input);
		double changeCenter(const double &input);
		double changeShape(const double &input);
		double changeCenterShape(const double &input);

		double changeDerivativeNormal(const double &output);
		double changeDerivativeCenter(const double &output);
		double changeDerivativeCenterShape(const double &output);

		double execute(const double &input);
		double executeDerivative(const double &output);

	private:

		double (ViSigmoidActivationFunction::*function)(const double &input);
		double (ViSigmoidActivationFunction::*derivativeFunction)(const double &output);

		double mShape;
		double mCenter;

};

#endif
