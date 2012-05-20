#include "vifouriertransformer.h"

ViFourierTransformer::ViFourierTransformer(Execution execution, int fixedSize)
	 : QObject()
{
	mSize = 0;

	mForwardThread = NULL;
	mInverseThread = NULL;

	initialize();
	setFixedSize(fixedSize);
	setExecution(execution);

	QObject::connect(mForwardThread, SIGNAL(finished()), this, SIGNAL(finished()));
	QObject::connect(mInverseThread, SIGNAL(finished()), this, SIGNAL(finished()));
}

ViFourierTransformer::~ViFourierTransformer()
{
	qDeleteAll(mFixedForwardThreads.begin(), mFixedForwardThreads.end());
	mFixedForwardThreads.clear();

	qDeleteAll(mFixedInverseThreads.begin(), mFixedInverseThreads.end());
	mFixedInverseThreads.clear();

	delete mVariableForwardThread;
	delete mVariableInverseThread;
}

bool ViFourierTransformer::setFixedSize(int size)
{
	bool wasSetForward = false;
	bool wasSetInverse = false;
	int index;
	for(index = 0; index < mFixedForwardThreads.size(); ++index)
	{
		if(mFixedForwardThreads[index]->size() == size)
		{
			mForwardThread = mFixedForwardThreads[index];
			wasSetForward = true;
		}
	}
	for(index = 0; index < mFixedInverseThreads.size(); ++index)
	{
		if(mFixedInverseThreads[index]->size() == size)
		{
			mInverseThread = mFixedInverseThreads[index];
			wasSetInverse = true;
		}
	}
	if(wasSetForward && wasSetInverse)
	{
		mSize = size;
		return true;
	}
	
	mSize = 0;
	mForwardThread = mVariableForwardThread;
	mInverseThread = mVariableInverseThread;
	return false;
}

void ViFourierTransformer::setExecution(Execution execution)
{
	if(execution == ViFourierTransformer::SameThread)
	{
		forwardTransformtion = &ViFourierTransformer::forwardTransformSameThread;
		inverseTransformtion = &ViFourierTransformer::inverseTransformSameThread;
	}
	else
	{
		forwardTransformtion = &ViFourierTransformer::forwardTransformSeperateThread;
		inverseTransformtion = &ViFourierTransformer::inverseTransformSeperateThread;
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
	ViFourierTransformer::fixedForwardTransform(input, output);
}

void ViFourierTransformer::inverseTransform(float input[], float output[])
{
	ViFourierTransformer::fixedInverseTransform(input, output);
}

void ViFourierTransformer::transform(float input[], float output[], qint32 numberOfSamples, ViWindower *windower, Direction direction)
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

void ViFourierTransformer::forwardTransform(float *input, float *output, qint32 numberOfSamples, ViWindower *windower)
{
	if(windower != NULL)
	{
		windower->apply(input, numberOfSamples);
	}
	variableForwardTransform(input, output, numberOfSamples);
}

void ViFourierTransformer::inverseTransform(float input[], float output[], qint32 numberOfSamples)
{
	variableInverseTransform(input, output, numberOfSamples);
}

void ViFourierTransformer::initialize()
{
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<3>());
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<4>());
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<5>());
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<6>());
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<7>());
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<8>());
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<9>());
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<10>());
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<11>());
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<12>());
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<13>());
	mFixedForwardThreads.append(new ViFourierFixedForwardThread<14>());

	mFixedInverseThreads.append(new ViFourierFixedInverseThread<3>());
	mFixedInverseThreads.append(new ViFourierFixedInverseThread<4>());
	mFixedInverseThreads.append(new ViFourierFixedInverseThread<5>());
	mFixedInverseThreads.append(new ViFourierFixedInverseThread<6>());
	mFixedInverseThreads.append(new ViFourierFixedInverseThread<7>());
	mFixedInverseThreads.append(new ViFourierFixedInverseThread<8>());
	mFixedInverseThreads.append(new ViFourierFixedInverseThread<9>());
	mFixedInverseThreads.append(new ViFourierFixedInverseThread<10>());
	mFixedInverseThreads.append(new ViFourierFixedInverseThread<11>());
	mFixedInverseThreads.append(new ViFourierFixedInverseThread<12>());
	mFixedInverseThreads.append(new ViFourierFixedInverseThread<13>());
	mFixedInverseThreads.append(new ViFourierFixedInverseThread<14>());

	mVariableForwardThread = new ViFourierVariableForwardThread();
	mVariableInverseThread = new ViFourierVariableInverseThread();
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

void ViFourierTransformer::variableForwardTransform(float input[], float output[], qint32 numberOfSamples)
{
	mForwardThread->setData(input, output);
	mForwardThread->setSize(numberOfSamples);
	(this->*forwardTransformtion)();
}

void ViFourierTransformer::variableInverseTransform(float input[], float output[], qint32 numberOfSamples)
{
	mInverseThread->setData(input, output);
	mInverseThread->setSize(numberOfSamples);
	(this->*inverseTransformtion)();
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
