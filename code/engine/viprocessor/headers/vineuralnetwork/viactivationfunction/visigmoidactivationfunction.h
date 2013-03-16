#ifndef VISIGMOIDACTIVATIONFUNCTION_H
#define VISIGMOIDACTIVATIONFUNCTION_H

#include <viactivationfunctiontype.h>

/*
	http://brange.me/2012/06/27/the-sigmoid-or-activation-function-and-their-uses/
*/

class ViSigmoidActivationFunction : public ViActivationFunctionType<ViSigmoidActivationFunction>
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

		static ViSigmoidActivationFunction* create(){return new ViSigmoidActivationFunction();}

	protected:

		void changeFunction();

		double changeNormal(const double &input);
		double changeCenter(const double &input);
		double changeShape(const double &input);
		double changeCenterShape(const double &input);

		double execute(const double &input, const int &inputCount);

	private:

		double (ViSigmoidActivationFunction::*function)(const double &input);

		double mShape;
		double mCenter;

};

#endif
