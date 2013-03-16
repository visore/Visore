#ifndef VILINEARACTIVATIONFUNCTION_H
#define VILINEARACTIVATIONFUNCTION_H

#include <viactivationfunctiontype.h>

class ViLinearActivationFunction : public ViActivationFunctionType<ViLinearActivationFunction>
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

		double execute(const double &input, const int &inputCount);

	private:

		double mGradient;

};

Q_DECLARE_METATYPE(ViLinearActivationFunction);

#endif
