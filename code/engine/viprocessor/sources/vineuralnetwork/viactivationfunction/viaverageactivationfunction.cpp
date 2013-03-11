#include <viaverageactivationfunction.h>

ViAverageActivationFunction::ViAverageActivationFunction()
	: ViActivationFunction(CLASSNAME, -1, 1)
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

ViActivationFunction* ViAverageActivationFunction::clone()
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
