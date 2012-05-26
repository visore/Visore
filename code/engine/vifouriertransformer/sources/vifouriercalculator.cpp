#include "vifouriercalculator.h"

ViFourierCalculator::ViFourierCalculator()
{
	mInput = 0;
	mOutput = 0;
	mSize = 0;
}

void ViFourierCalculator::setData(double input[], double output[])
{
	mInput = input;
	mOutput = output;
}

void ViFourierCalculator::setSize(int size)
{
	mSize = size;
}

int ViFourierCalculator::size()
{
	return mSize;
}
