#include "vifouriertransformer.h"

ViFourierTransformer::ViFourierTransformer()
	 : QObject()
{
	QObject::connect(&mFixedForwardThread, SIGNAL(finished()), this, SIGNAL(finished()));
	QObject::connect(&mFixedInverseThread, SIGNAL(finished()), this, SIGNAL(finished()));
	QObject::connect(&mVariableForwardThread, SIGNAL(finished()), this, SIGNAL(finished()));
	QObject::connect(&mVariableInverseThread, SIGNAL(finished()), this, SIGNAL(finished()));
}

void ViFourierTransformer::transform(float input[], float output[], Direction direction, qint32 numberOfSamples)
{
	if(direction == ViFourierTransformer::Forward)
	{
		forwardTransform(input, output, numberOfSamples);
	}
	else
	{
		inverseTransform(input, output, numberOfSamples);
	}
}

void ViFourierTransformer::forwardTransform(float input[], float output[], qint32 numberOfSamples)
{
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

void ViFourierTransformer::fixedForwardTransform(float input[], float output[])
{
	mFixedForwardThread.setData(input, output);
	mFixedForwardThread.start();
}

void ViFourierTransformer::fixedInverseTransform(float input[], float output[])
{
	mFixedInverseThread.setData(input, output);
	mFixedInverseThread.start();
}

void ViFourierTransformer::variableForwardTransform(float input[], float output[], qint32 numberOfSamples)
{
	mVariableForwardThread.setData(input, output, numberOfSamples);
	mVariableForwardThread.start();
}

void ViFourierTransformer::variableInverseTransform(float input[], float output[], qint32 numberOfSamples)
{
	mVariableInverseThread.setData(input, output, numberOfSamples);
	mVariableInverseThread.start();
}
