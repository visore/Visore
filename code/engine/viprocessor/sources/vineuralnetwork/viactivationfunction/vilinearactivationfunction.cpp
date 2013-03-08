#include <vilinearactivationfunction.h>
#include <vilogger.h>

ViLinearActivationFunction::ViLinearActivationFunction(double gradient)
	: ViActivationFunction(-1, 1)
{
	setGradient(gradient);
}

double ViLinearActivationFunction::execute(const double &input, const int &inputCount)
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

ViElement ViLinearActivationFunction::exportData()
{
	ViElement element = ViActivationFunction::exportData();
	element.addChild("Gradient", gradient());
	return element;
}

bool ViLinearActivationFunction::importData(ViElement element)
{
	if(ViActivationFunction::importData(element))
	{
		ViElement gradient = element.child("Gradient");
		if(gradient.isNull())
		{
			LOG("The gradient could not be imported", QtCriticalMsg);
		}
		else
		{
			setGradient(gradient.toReal());
		}
		return true;
	}
	return false;
}
