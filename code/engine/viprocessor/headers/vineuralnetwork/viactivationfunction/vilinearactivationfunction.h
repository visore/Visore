#ifndef VILINEARACTIVATIONFUNCTION_H
#define VILINEARACTIVATIONFUNCTION_H

#include <viactivationfunction.h>

class ViLinearActivationFunction : public ViActivationFunction
{

	public:

		ViLinearActivationFunction(double gradient = 0.5);

		double gradient();

		double setGradient(double gradient);

	protected:

		double execute(double input);

	private:

		double mGradient;

};

#endif
