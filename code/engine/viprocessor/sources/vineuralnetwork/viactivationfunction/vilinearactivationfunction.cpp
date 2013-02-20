#include <vilinearactivationfunction.h>

ViLinearActivationFunction::ViLinearActivationFunction(double gradient)
	: ViActivationFunction(-1, 1)
{
	setGradient(gradient);
}

double ViLinearActivationFunction::execute(double input)
{
	return mGradient * input;
}

double ViLinearActivationFunction::gradient()
{
	return mGradient;
}

double ViLinearActivationFunction::setGradient(double gradient)
{
	if(gradient < minimum())
	{
		mGradient = minimum();
	}
	else if(gradient > maximum())
	{
		mGradient = maximum();
	}
	else
	{
		mGradient = gradient;
	}
	return mGradient;
}
