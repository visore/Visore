#ifndef VIDIRECTACTIVATIONFUNCTION_H
#define VIDIRECTACTIVATIONFUNCTION_H

#include <viactivationfunction.h>

class ViDirectActivationFunction : public ViActivationFunction
{

	public:

		ViDirectActivationFunction();
		ViDirectActivationFunction(const ViDirectActivationFunction &other);

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

		ViDirectActivationFunction* clone();

	protected:

		double execute(const double &input);
		double executeDerivative(const double &output);

};

#endif
