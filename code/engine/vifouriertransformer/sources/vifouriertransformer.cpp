#include "vifouriertransformer.h"

ViFourierTransformer::ViFourierTransformer(int size, QString functionName)
{
	mWindowFunctions = ViWindowFunctionManager<float>::functions();
	mWindowFunction = 0;
	mCalculator = 0;
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
}

ViFourierTransformer::Initialization ViFourierTransformer::setSize(int size)
{
	if(isValidSize(size))
	{
		mSize = size;
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

bool ViFourierTransformer::setWindowFunction(QString functionName)
{
	for(int i = 0; i < mWindowFunctions.size(); ++i)
	{
		if(functionName.trimmed().toLower().replace("function", "") == mWindowFunctions[i].trimmed().toLower().replace("function", ""))
		{
			if(mWindowFunction != 0)
			{
				delete mWindowFunction;
			}
			mWindowFunction = ViWindowFunctionManager<float>::createFunction(functionName);
			if(mWindowFunction != 0 && isValidSize(mSize))
			{
				mWindowFunction->create(mSize);
			}
			return true;
		}
	}
	return false;
}

QStringList ViFourierTransformer::windowFunctions()
{
	return mWindowFunctions;
}

void ViFourierTransformer::transform(float input[], float output[], Direction direction)
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

void ViFourierTransformer::forwardTransform(float *input, float *output)
{
	if(mWindowFunction != 0)
	{
		mWindowFunction->apply(input, mSize);
	}
	mCalculator->setData(input, output);
	mCalculator->forward();
}

void ViFourierTransformer::inverseTransform(float input[], float output[])
{
	mCalculator->setData(input, output);
	mCalculator->inverse();
}

void ViFourierTransformer::rescale(float input[])
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

int ViFourierTransformer::sizeToKey(int size)
{
	float result = log(size) / float(log(2));
	if(result == float(int(result)))
	{
		return result;
	}
	return -1;
}

bool ViFourierTransformer::isValidSize(int value)
{
	return ((value > 0) && ((value & (~value + 1)) == value));
}

void ViFourierTransformer::conjugate(float input[])
{
	for(int i = mSize / 2 + 1; i < mSize; ++i)
	{
		input[i] = -input[i];
	}
}

ViComplexVector ViFourierTransformer::toComplex(float input[])
{
	int last = mSize / 2;
	QVector<ViComplexFloat> result(last + 1);
	result[0] = ViComplexFloat(input[0], 0);
	for(int i = 1; i < last; ++i)
	{
		result[i] = ViComplexFloat(input[i], -input[last + i]);
	}
	result[last] = ViComplexFloat(input[last], 0);
	return result;
}
