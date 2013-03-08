#ifndef VIAVERAGEACTIVATIONFUNCTION_H
#define VIAVERAGEACTIVATIONFUNCTION_H

#include <viactivationfunction.h>

class ViAverageActivationFunction : public ViActivationFunction
{

	public:

		ViAverageActivationFunction();

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

	protected:

		double execute(const double &input, const int &inputCount);

};

#endif
