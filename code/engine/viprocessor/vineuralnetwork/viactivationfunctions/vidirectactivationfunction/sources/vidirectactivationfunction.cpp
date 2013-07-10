#include <vidirectactivationfunction.h>

ViDirectActivationFunction::ViDirectActivationFunction()
	: ViActivationFunction(0, 1)
{
}

ViDirectActivationFunction::ViDirectActivationFunction(const ViDirectActivationFunction &other)
	: ViActivationFunction(other)
{
}

double ViDirectActivationFunction::execute(const double &input)
{
	return input;
}

double ViDirectActivationFunction::executeDerivative(const double &output)
{
	return 1;
}

ViDirectActivationFunction* ViDirectActivationFunction::clone()
{
	return new ViDirectActivationFunction(*this);
}

ViElement ViDirectActivationFunction::exportData()
{
	return ViActivationFunction::exportData();
}

bool ViDirectActivationFunction::importData(ViElement element)
{
	return ViActivationFunction::importData(element);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViActivationFunction* create()
{
	return new ViDirectActivationFunction();
}

#ifdef __cplusplus
}
#endif
