#include <visigmoidactivationfunction.h>
#include <vilogger.h>
#include <math.h>

ViSigmoidActivationFunction::ViSigmoidActivationFunction(double shape)
	: ViActivationFunction(0, 1)
{
	setShape(shape);
}

ViSigmoidActivationFunction::ViSigmoidActivationFunction(const ViSigmoidActivationFunction &other)
	: ViActivationFunction(other)
{
	mShape = other.mShape;
	setRange(other.minimum(), other.maximum());
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

double ViSigmoidActivationFunction::execute(const double &input, const int &inputCount)
{
	return 1 / (1 + exp(-(this->*function)(input)));
}

ViSigmoidActivationFunction* ViSigmoidActivationFunction::clone()
{
	return new ViSigmoidActivationFunction(*this);
}

ViElement ViSigmoidActivationFunction::exportData()
{
	ViElement element = ViActivationFunction::exportData();
	element.addChild("Shape", shape());
	return element;
}

bool ViSigmoidActivationFunction::importData(ViElement element)
{
	if(ViActivationFunction::importData(element))
	{
		ViElement shape = element.child("Shape");
		if(shape.isNull())
		{
			LOG("The shape could not be imported", QtCriticalMsg);
		}
		else
		{
			setShape(shape.toReal());
		}
		return true;
	}
	return false;
}

#ifdef __cplusplus
extern "C"
{
#endif

ViActivationFunction* create()
{
	return new ViSigmoidActivationFunction();
}

#ifdef __cplusplus
}
#endif
