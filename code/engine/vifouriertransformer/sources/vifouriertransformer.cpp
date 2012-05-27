#include "vifouriertransformer.h"
#include "vifourierfixedcalculator.h"
#include "vifouriervariablecalculator.h"

ViFourierTransformer::ViFourierTransformer(const int size, QString const functionName)
{
	mWindowFunctions = ViWindowFunctionManager<double>::functions();
	mWindowFunction = 0;
	mCalculator = 0;
	mWindowData = 0;
	initialize();
	setSize(size);
	setWindowFunction(functionName);
}

ViFourierTransformer::~ViFourierTransformer()
{
	qDeleteAll(mFixedCalculators.begin(), mFixedCalculators.end());
	mFixedCalculators.clear();
	delete mVariableCalculator;
	if(mWindowFunction != 0)
	{
		delete mWindowFunction;
	}
	if(mWindowData != 0)
	{
		delete mWindowData;
	}
}

ViFourierTransformer::Initialization ViFourierTransformer::setSize(const int size)
{
	if(isValidSize(size))
	{
		mSize = size;
		if(mWindowData != NULL)
		{
			delete mWindowData;
		}
		mWindowData = new double[mSize];
		if(mWindowFunction != 0)
		{
			mWindowFunction->create(mSize);
		}
		int key = sizeToKey(mSize);
		if(mFixedCalculators.contains(key))
		{
			mCalculator = mFixedCalculators[key];
			return ViFourierTransformer::FixedSize;
		}
		else
		{
			mCalculator = mVariableCalculator;
			mCalculator->setSize(mSize);
			return ViFourierTransformer::VariableSize;
		}
	}
	mSize = 0;
	return ViFourierTransformer::InvalidSize;
}

bool ViFourierTransformer::setWindowFunction(const QString functionName)
{
	for(int i = 0; i < mWindowFunctions.size(); ++i)
	{
		if(functionName.trimmed().toLower().replace("function", "") == mWindowFunctions[i].trimmed().toLower().replace("function", ""))
		{
			if(mWindowFunction != 0)
			{
				delete mWindowFunction;
			}
			mWindowFunction = ViWindowFunctionManager<double>::createFunction(functionName);
			if(mWindowFunction != 0 && isValidSize(mSize))
			{
				mWindowFunction->create(mSize);
			}
			return true;
		}
	}
	return false;
}

QStringList ViFourierTransformer::windowFunctions() const
{
	return mWindowFunctions;
}

void ViFourierTransformer::transform(const double input[], double output[], Direction direction)
{
	if(direction == ViFourierTransformer::Forward)
	{
		forwardTransform(input, output);
	}
	else
	{
		inverseTransform(input, output);
	}
}

void ViFourierTransformer::forwardTransform(const double input[], double output[])
{
	if(mWindowFunction != 0)
	{
		mWindowFunction->apply(input, mWindowData, mSize);
		mCalculator->setData(mWindowData, output);
	}
	else
	{
		mCalculator->setData(input, output);
	}
	mCalculator->forward();
	conjugate(output); //FFTReal somehow has a negative imaginary part
}

void ViFourierTransformer::inverseTransform(const double input[], double output[])
{
	mCalculator->setData(input, output);
	mCalculator->inverse();
}

void ViFourierTransformer::rescale(double input[])
{
	mCalculator->setData(input);
	mCalculator->rescale();
}

void ViFourierTransformer::initialize()
{
	mFixedCalculators.insert(3, new ViFourierFixedCalculator<3>());
	mFixedCalculators.insert(4, new ViFourierFixedCalculator<4>());
	mFixedCalculators.insert(5, new ViFourierFixedCalculator<5>());
	mFixedCalculators.insert(6, new ViFourierFixedCalculator<6>());
	mFixedCalculators.insert(7, new ViFourierFixedCalculator<7>());
	mFixedCalculators.insert(8, new ViFourierFixedCalculator<8>());
	mFixedCalculators.insert(9, new ViFourierFixedCalculator<9>());
	mFixedCalculators.insert(10, new ViFourierFixedCalculator<10>());
	mFixedCalculators.insert(11, new ViFourierFixedCalculator<11>());
	mFixedCalculators.insert(12, new ViFourierFixedCalculator<12>());
	mFixedCalculators.insert(13, new ViFourierFixedCalculator<13>());
	mFixedCalculators.insert(14, new ViFourierFixedCalculator<14>());

	mVariableCalculator = new ViFourierVariableCalculator();
}

int ViFourierTransformer::sizeToKey(const int size) const
{
	double result = log(size) / double(log(2));
	if(result == double(int(result)))
	{
		return result;
	}
	return -1;
}

bool ViFourierTransformer::isValidSize(const int value) const
{
	return ((value > 0) && ((value & (~value + 1)) == value));
}

void ViFourierTransformer::conjugate(double input[]) const
{
	for(int i = mSize / 2 + 1; i < mSize; ++i)
	{
		input[i] = -input[i];
	}
}

void ViFourierTransformer::pad(double input[], const int numberOfSamples, const double value) const
{
	if(numberOfSamples < mSize)
	{
		for(int i = numberOfSamples; i < mSize; ++i)
		{
			input[i] = value;
		}
	}
}

void ViFourierTransformer::multiply(const double first[], const double second[], double output[]) const
{
	int last = mSize / 2;
	output[0] = first[0] * second[0];
	for(int i = 1; i < last; ++i)
	{
		output[i] = (first[i] * second[i]) - (first[i + last] * second[i + last]);
		output[i + last] = (first[i] * second[i + last]) + (first[i + last] * second[i]);
	}
	output[last] = first[last] * second[last];
}

ViComplexVector ViFourierTransformer::toComplex(const double input[]) const
{
	int last = mSize / 2;
	QVector<ViDoubleComplex> result(last + 1);
	result[0] = ViDoubleComplex(input[0], 0);
	for(int i = 1; i < last; ++i)
	{
		result[i] = ViDoubleComplex(input[i], input[i + last]);
	}
	result[last] = ViDoubleComplex(input[last], 0);
	return result;
}
