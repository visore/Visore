#ifndef VIAVERAGEACTIVATIONFUNCTION_H
#define VIAVERAGEACTIVATIONFUNCTION_H

#include <viactivationfunctiontype.h>

class ViAverageActivationFunction : public ViActivationFunctionType<ViAverageActivationFunction>
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

Q_DECLARE_METATYPE(ViAverageActivationFunction);

#endif
