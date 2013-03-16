#include <viaverageactivationfunction.h>

ViAverageActivationFunction::ViAverageActivationFunction()
	: ViActivationFunctionType(-1, 1)
{
}

ViAverageActivationFunction::ViAverageActivationFunction(const ViAverageActivationFunction &other)
	: ViActivationFunctionType(other)
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
	return ViActivationFunctionType::exportData();
}

bool ViAverageActivationFunction::importData(ViElement element)
{
	return ViActivationFunctionType::importData(element);
}
