#include <visigmoidactivationfunction.h>
#include <math.h>

ViSigmoidActivationFunction::ViSigmoidActivationFunction(double shape)
	: ViActivationFunction(0, 1)
{
	setShape(shape);
}

void ViSigmoidActivationFunction::setRange(double minimum, double maximum)
{
	ViActivationFunction::setRange(minimum, maximum);
	changeFunction();
}

void ViSigmoidActivationFunction::setMinimum(double minimum)
{
	ViActivationFunction::setMinimum(minimum);
	changeFunction();
}

void ViSigmoidActivationFunction::setMaximum(double maximum)
{
	ViActivationFunction::setMaximum(maximum);
	changeFunction();
}

double ViSigmoidActivationFunction::shape()
{
	return mShape;
}

void ViSigmoidActivationFunction::setShape(double shape)
{
	mShape = shape;
	changeFunction();
}

void ViSigmoidActivationFunction::changeFunction()
{
	if(minimum() != -1 || maximum() != 1)
	{
		mCenter = (minimum() + maximum()) / 2.0; // Calculate mean
		if(mShape != 1)
		{
			function = &ViSigmoidActivationFunction::changeCenterShape;
		}
		else
		{
			function = &ViSigmoidActivationFunction::changeCenter;
		}
	}
	else if(mShape != 1)
	{
		function = &ViSigmoidActivationFunction::changeShape;
	}
	else
	{
		function = &ViSigmoidActivationFunction::changeNormal;
	}
}

double ViSigmoidActivationFunction::changeNormal(const double &input)
{
	return input;
}

double ViSigmoidActivationFunction::changeCenter(const double &input)
{
	return input - mCenter;
}

double ViSigmoidActivationFunction::changeShape(const double &input)
{
	return input * mShape;
}

double ViSigmoidActivationFunction::changeCenterShape(const double &input)
{
	return mShape * (input - mCenter);
}

double ViSigmoidActivationFunction::execute(double input)
{
	return 1 / (1 + exp(-(this->*function)(input)));
}
