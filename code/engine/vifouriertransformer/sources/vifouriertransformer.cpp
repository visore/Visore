#include "vifouriertransformer.h"

ViFourierTransformer::ViFourierTransformer(Execution execution)
	 : QObject()
{
	mExecution = execution;
	QObject::connect(&mFixedForwardThread, SIGNAL(finished()), this, SIGNAL(finished()));
	QObject::connect(&mFixedInverseThread, SIGNAL(finished()), this, SIGNAL(finished()));
	QObject::connect(&mVariableForwardThread, SIGNAL(finished()), this, SIGNAL(finished()));
	QObject::connect(&mVariableInverseThread, SIGNAL(finished()), this, SIGNAL(finished()));
}

qint32 ViFourierTransformer::optimalSize()
{
	return FFT_SAMPLES;
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
	if(numberOfSamples == FFT_SAMPLES)
	{
		ViFourierTransformer::fixedForwardTransform(input, output);
	}
	else
	{
		ViFourierTransformer::variableForwardTransform(input, output, numberOfSamples);
	}
}

void ViFourierTransformer::inverseTransform(float input[], float output[], qint32 numberOfSamples)
{
	if(numberOfSamples == FFT_SAMPLES)
	{
		ViFourierTransformer::fixedInverseTransform(input, output);
	}
	else
	{
		ViFourierTransformer::variableInverseTransform(input, output, numberOfSamples);
	}
}

void ViFourierTransformer::fixedForwardTransform(float *input, float *output)
{
	mFixedForwardThread.setData(input, output);
	if(mExecution == ViFourierTransformer::SameThread)
	{
		mFixedForwardThread.run();
	}
	else
	{
		mFixedForwardThread.start();
	}
}

void ViFourierTransformer::fixedInverseTransform(float input[], float output[])
{
	mFixedInverseThread.setData(input, output);
	if(mExecution == ViFourierTransformer::SameThread)
	{
		mFixedInverseThread.run();
	}
	else
	{
		mFixedInverseThread.start();
	}
}

void ViFourierTransformer::variableForwardTransform(float input[], float output[], qint32 numberOfSamples)
{
	mVariableForwardThread.setData(input, output, numberOfSamples);
	if(mExecution == ViFourierTransformer::SameThread)
	{
		mVariableForwardThread.run();
	}
	else
	{
		mVariableForwardThread.start();
	}
}

void ViFourierTransformer::variableInverseTransform(float input[], float output[], qint32 numberOfSamples)
{
	mVariableInverseThread.setData(input, output, numberOfSamples);
	if(mExecution == ViFourierTransformer::SameThread)
	{
		mVariableInverseThread.run();
	}
	else
	{
		mVariableInverseThread.start();
	}
}
