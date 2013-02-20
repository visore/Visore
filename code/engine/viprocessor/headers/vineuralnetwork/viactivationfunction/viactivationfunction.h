#ifndef VIACTIVATIONFUNCTION_H
#define VIACTIVATIONFUNCTION_H

#include <visingleton.h>

class ViActivationFunction : public ViSingleton<ViActivationFunction>
{

	friend class ViSingleton<ViActivationFunction>;

	public:

		ViActivationFunction(double functionMinimum = 0, double functionMaximum = 1);
		virtual ~ViActivationFunction();

		virtual void setRange(double minimum, double maximum);
		virtual void setMinimum(double minimum);
		virtual void setMaximum(double maximum);

		double minimum();
		double maximum();

		double calculate(double input);

	protected:

		virtual double execute(double input) = 0;

		void setFunctionRange(double minimum, double maximum);
		void setFunctionMinimum(double minimum);
		void setFunctionMaximum(double maximum);

	private:

		double mFunctionMinimum;
		double mFunctionMaximum;
		double mMinimum;
		double mMaximum;

};

#endif
