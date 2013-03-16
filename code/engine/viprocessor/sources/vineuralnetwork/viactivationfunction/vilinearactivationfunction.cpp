#include <vilinearactivationfunction.h>
#include <vilogger.h>

ViLinearActivationFunction::ViLinearActivationFunction(double gradient)
	: ViActivationFunctionType(-1, 1)
{
	setGradient(gradient);
}

ViLinearActivationFunction::ViLinearActivationFunction(const ViLinearActivationFunction &other)
	: ViActivationFunctionType(other)
{
	mGradient = other.mGradient;
}

double ViLinearActivationFunction::execute(const double &input, const int &inputCount)
{
	return mGradient * input;
}

ViLinearActivationFunction* ViLinearActivationFunction::clone()
{
	return new ViLinearActivationFunction(*this);
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
	ViElement element = ViActivationFunctionType::exportData();
	element.addChild("Gradient", gradient());
	return element;
}

bool ViLinearActivationFunction::importData(ViElement element)
{
	if(ViActivationFunctionType::importData(element))
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
