#ifndef VIAVERAGEACTIVATIONFUNCTION_H
#define VIAVERAGEACTIVATIONFUNCTION_H

#include <viactivationfunction.h>

class ViAverageActivationFunction : public ViActivationFunction
{

	public:

		ViAverageActivationFunction();
		ViAverageActivationFunction(const ViAverageActivationFunction &other);

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

		ViAverageActivationFunction* clone();

	protected:

		double execute(const double &input, const int &inputCount);

};

#endif
