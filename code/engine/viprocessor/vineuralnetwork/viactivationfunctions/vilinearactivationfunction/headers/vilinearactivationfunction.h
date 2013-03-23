#ifndef VILINEARACTIVATIONFUNCTION_H
#define VILINEARACTIVATIONFUNCTION_H

#include <viactivationfunction.h>

class ViLinearActivationFunction : public ViActivationFunction
{

	public:

		ViLinearActivationFunction(double gradient = 0.5);
		ViLinearActivationFunction(const ViLinearActivationFunction &other);

		double gradient();

		double setGradient(double gradient);

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

		ViLinearActivationFunction* clone();

	protected:

		double execute(const double &input);
		double executeDerivative(const double &output);

	private:

		double mGradient;

};

#endif
