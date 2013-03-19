#include <viaverageactivationfunction.h>

ViAverageActivationFunction::ViAverageActivationFunction()
	: ViActivationFunction(-1, 1)
{
}

ViAverageActivationFunction::ViAverageActivationFunction(const ViAverageActivationFunction &other)
	: ViActivationFunction(other)
{
}

double ViAverageActivationFunction::execute(const double &input, const int &inputCount)
{
	return input / inputCount;
}

ViAverageActivationFunction* ViAverageActivationFunction::clone()
{
	return new ViAverageActivationFunction(*this);
}

ViElement ViAverageActivationFunction::exportData()
{
	return ViActivationFunction::exportData();
}

bool ViAverageActivationFunction::importData(ViElement element)
{
	return ViActivationFunction::importData(element);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViActivationFunction* create()
{
	return new ViAverageActivationFunction();
}

#ifdef __cplusplus
}
#endif
