#include "vifouriertransformer.h"

#include <iostream>
#include <QString>

using namespace std;


ViFourierTransformer::ViFourierTransformer(Execution execution, int fixedSize)
	 : QObject()
{
	mSize = 0;

	mForwardThread = NULL;
	mInverseThread = NULL;
	mRescaleThread = NULL;

	initialize();
	setFixedSize(fixedSize);
	setExecution(execution);
}

ViFourierTransformer::~ViFourierTransformer()
{
	qDeleteAll(mFixedForwardThreads.begin(), mFixedForwardThreads.end());
	mFixedForwardThreads.clear();

	qDeleteAll(mFixedInverseThreads.begin(), mFixedInverseThreads.end());
	mFixedInverseThreads.clear();

	qDeleteAll(mFixedRescaleThreads.begin(), mFixedRescaleThreads.end());
	mFixedRescaleThreads.clear();

	delete mVariableForwardThread;
	delete mVariableInverseThread;
	delete mVariableRescaleThread;
}

void ViFourierTransformer::emitFinished()
{
	emit finished();
}

bool ViFourierTransformer::setFixedSize(int size)
{
	bool wasSetForward = false;
	bool wasSetInverse = false;
	bool wasSetRescale = false;
	int index, last;
	last = mFixedForwardThreads.size();
	for(index = 0; index < last; ++index)
	{
		if(mFixedForwardThreads[index]->size() == size)
		{
			mForwardThread = mFixedForwardThreads[index];
			wasSetForward = true;
		}
	}
	last = mFixedInverseThreads.size();
	for(index = 0; index < last; ++index)
	{
		if(mFixedInverseThreads[index]->size() == size)
		{
			mInverseThread = mFixedInverseThreads[index];
			wasSetInverse = true;
		}
	}
	last = mFixedRescaleThreads.size();
	for(index = 0; index < last; ++index)
	{
		if(mFixedRescaleThreads[index]->size() == size)
		{
			mRescaleThread = mFixedRescaleThreads[index];
			wasSetRescale = true;
		}
	}
	if(wasSetForward && wasSetInverse && wasSetRescale)
	{
		mSize = size;
		return true;
	}
	else
	{
		mSize = 0;
		mForwardThread = mVariableForwardThread;
		mInverseThread = mVariableInverseThread;
		mRescaleThread = mVariableRescaleThread;
		return false;
	}
}

void ViFourierTransformer::setExecution(Execution execution)
{
	if(execution == ViFourierTransformer::SameThread)
	{
		forwardTransformtion = &ViFourierTransformer::forwardTransformSameThread;
		inverseTransformtion = &ViFourierTransformer::inverseTransformSameThread;
		rescaleTransformtion = &ViFourierTransformer::rescaleTransformSameThread;
	}
	else
	{
		forwardTransformtion = &ViFourierTransformer::forwardTransformSeperateThread;
		inverseTransformtion = &ViFourierTransformer::inverseTransformSeperateThread;
		rescaleTransformtion = &ViFourierTransformer::rescaleTransformSeperateThread;
	}
}

void ViFourierTransformer::transform(float input[], float output[], ViWindower *windower, Direction direction)
{
	if(direction == ViFourierTransformer::Forward)
	{
		forwardTransform(input, output, windower);
	}
	else
	{
		inverseTransform(input, output);
	}
}

void ViFourierTransformer::forwardTransform(float *input, float *output, ViWindower *windower)
{
	if(windower != NULL)
	{
		windower->apply(input, mSize);
	}
	fixedForwardTransform(input, output);
}

void ViFourierTransformer::inverseTransform(float input[], float output[])
{
	fixedInverseTransform(input, output);
}

void ViFourierTransformer::rescale(float input[])
{
	fixedRescale(input);
}

void ViFourierTransformer::transform(float input[], float output[], int numberOfSamples, ViWindower *windower, Direction direction)
{
	if(direction == ViFourierTransformer::Forward)
	{
		forwardTransform(input, output, numberOfSamples, windower);
	}
	else
	{
		inverseTransform(input, output, numberOfSamples);
	}
}

void ViFourierTransformer::forwardTransform(float *input, float *output, int numberOfSamples, ViWindower *windower)
{
	if(windower != NULL)
	{
		windower->apply(input, numberOfSamples);
	}
	variableForwardTransform(input, output, numberOfSamples);
}

void ViFourierTransformer::inverseTransform(float input[], float output[], int numberOfSamples)
{
	variableInverseTransform(input, output, numberOfSamples);
}

void ViFourierTransformer::rescale(float input[], int numberOfSamples)
{
	variableRescale(input, numberOfSamples);
}

void ViFourierTransformer::initialize()
{
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<3>(this));
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<4>(this));
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<5>(this));
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<6>(this));
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<7>(this));
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<8>(this));
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<9>(this));
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<10>(this));
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<11>(this));
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<12>(this));
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<13>(this));
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<14>(this));

	mFixedInverseThreads.append(new ViFourierFixedInverseThread<3>(this));
	mFixedInverseThreads.append(new ViFourierFixedInverseThread<4>(this));
	mFixedInverseThreads.append(new ViFourierFixedInverseThread<5>(this));
	mFixedInverseThreads.append(new ViFourierFixedInverseThread<6>(this));
	mFixedInverseThreads.append(new ViFourierFixedInverseThread<7>(this));
	mFixedInverseThreads.append(new ViFourierFixedInverseThread<8>(this));
	mFixedInverseThreads.append(new ViFourierFixedInverseThread<9>(this));
	mFixedInverseThreads.append(new ViFourierFixedInverseThread<10>(this));
	mFixedInverseThreads.append(new ViFourierFixedInverseThread<11>(this));
	mFixedInverseThreads.append(new ViFourierFixedInverseThread<12>(this));
	mFixedInverseThreads.append(new ViFourierFixedInverseThread<13>(this));
	mFixedInverseThreads.append(new ViFourierFixedInverseThread<14>(this));

	mFixedRescaleThreads.append(new ViFourierFixedRescaleThread<3>(this));
	mFixedRescaleThreads.append(new ViFourierFixedRescaleThread<4>(this));
	mFixedRescaleThreads.append(new ViFourierFixedRescaleThread<5>(this));
	mFixedRescaleThreads.append(new ViFourierFixedRescaleThread<6>(this));
	mFixedRescaleThreads.append(new ViFourierFixedRescaleThread<7>(this));
	mFixedRescaleThreads.append(new ViFourierFixedRescaleThread<8>(this));
	mFixedRescaleThreads.append(new ViFourierFixedRescaleThread<9>(this));
	mFixedRescaleThreads.append(new ViFourierFixedRescaleThread<10>(this));
	mFixedRescaleThreads.append(new ViFourierFixedRescaleThread<11>(this));
	mFixedRescaleThreads.append(new ViFourierFixedRescaleThread<12>(this));
	mFixedRescaleThreads.append(new ViFourierFixedRescaleThread<13>(this));
	mFixedRescaleThreads.append(new ViFourierFixedRescaleThread<14>(this));

	mVariableForwardThread = new ViFourierVariableForwardThread(this);
	mVariableInverseThread = new ViFourierVariableInverseThread(this);
	mVariableRescaleThread = new ViFourierVariableRescaleThread(this);
}

void ViFourierTransformer::fixedForwardTransform(float *input, float *output)
{
	mForwardThread->setData(input, output);
	(this->*forwardTransformtion)();
}

void ViFourierTransformer::fixedInverseTransform(float input[], float output[])
{
	mInverseThread->setData(input, output);
	(this->*inverseTransformtion)();
}

void ViFourierTransformer::fixedRescale(float input[])
{
	mRescaleThread->setData(input);
	(this->*rescaleTransformtion)();
}

void ViFourierTransformer::variableForwardTransform(float input[], float output[], int numberOfSamples)
{
	mForwardThread->setData(input, output);
	mForwardThread->setSize(numberOfSamples);
	(this->*forwardTransformtion)();
}

void ViFourierTransformer::variableInverseTransform(float input[], float output[], int numberOfSamples)
{
	mInverseThread->setData(input, output);
	mInverseThread->setSize(numberOfSamples);
	(this->*inverseTransformtion)();
}

void ViFourierTransformer::variableRescale(float input[], int numberOfSamples)
{
	mRescaleThread->setData(input);
	mRescaleThread->setSize(numberOfSamples);
	(this->*rescaleTransformtion)();
}

void ViFourierTransformer::forwardTransformSameThread()
{
	mForwardThread->run();
}

void ViFourierTransformer::forwardTransformSeperateThread()
{
	mForwardThread->start();
}

void ViFourierTransformer::inverseTransformSameThread()
{
	mInverseThread->run();
}

void ViFourierTransformer::inverseTransformSeperateThread()
{
	mInverseThread->start();
}

void ViFourierTransformer::rescaleTransformSameThread()
{
	mRescaleThread->run();
}

void ViFourierTransformer::rescaleTransformSeperateThread()
{
	mRescaleThread->start();
}

ViComplexVector toComplex(float input[], int numberOfSamples)
{
	QVector<ViComplexFloat> result;
	int last = numberOfSamples / 2;
	result.push_back(ViComplexFloat(input[0], 0));
	for(int i = 1; i < last; ++i)
	{
		result.push_back(ViComplexFloat(input[i], -input[last + i]));
	}
	result.push_back(ViComplexFloat(input[last], 0));
	return result;
}
