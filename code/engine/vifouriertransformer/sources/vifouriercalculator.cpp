#include "vifouriercalculator.h"

ViFourierCalculator::ViFourierCalculator()
{
	mInput = 0;
	mOutput = 0;
	mSize = 0;
}

void ViFourierCalculator::setData(const double input[], double output[])
{
	mInput = input;
	mOutput = output;
}

void ViFourierCalculator::setData(double input[])
{
	mOutput = input;
}

void ViFourierCalculator::setSize(const int size)
{
	mSize = size;
}

int ViFourierCalculator::size() const
{
	return mSize;
}
