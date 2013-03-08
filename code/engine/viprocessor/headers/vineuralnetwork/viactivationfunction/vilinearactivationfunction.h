#ifndef VILINEARACTIVATIONFUNCTION_H
#define VILINEARACTIVATIONFUNCTION_H

#include <viactivationfunction.h>

class ViLinearActivationFunction : public ViActivationFunction
{

	public:

		ViLinearActivationFunction(double gradient = 0.5);

		double gradient();

		double setGradient(double gradient);

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

	protected:

		double execute(const double &input, const int &inputCount);

	private:

		double mGradient;

};

#endif
