#include <viaverageactivationfunction.h>

ViAverageActivationFunction::ViAverageActivationFunction()
	: ViActivationFunction(-1, 1)
{
}

double ViAverageActivationFunction::execute(const double &input, const int &inputCount)
{
	return input / inputCount;
}

ViElement ViAverageActivationFunction::exportData()
{
	return ViActivationFunction::exportData();
}

bool ViAverageActivationFunction::importData(ViElement element)
{
	return ViActivationFunction::importData(element);
}
