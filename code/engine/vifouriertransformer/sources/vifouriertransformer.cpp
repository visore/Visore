#ifdef VIFOURIERTRANSFORMER_H

template <typename T>
ViFourierTransformer<T>::ViFourierTransformer()
	: QObject()
{
	QObject::connect(&mFixedForwardThread, SIGNAL(finished()), this, SIGNAL(finished()));
	QObject::connect(&mFixedInverseThread, SIGNAL(finished()), this, SIGNAL(finished()));
	QObject::connect(&mVariableForwardThread, SIGNAL(finished()), this, SIGNAL(finished()));
	QObject::connect(&mVariableInverseThread, SIGNAL(finished()), this, SIGNAL(finished()));
}

template <typename T>
void ViFourierTransformer<T>::transform(T input[], T output[], Direction direction, qint32 numberOfSamples)
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

template <typename T>
void ViFourierTransformer<T>::forwardTransform(T input[], T output[], qint32 numberOfSamples)
{
	if(numberOfSamples == FFT_SAMPLES)
	{
		ViFourierTransformer<T>::fixedForwardTransform(input, output);
	}
	else
	{
		ViFourierTransformer<T>::variableForwardTransform(input, output);
	}
}

template <typename T>
void ViFourierTransformer<T>::inverseTransform(T input[], T output[], qint32 numberOfSamples)
{
	if(numberOfSamples == FFT_SAMPLES)
	{
		ViFourierTransformer<T>::fixedInverseTransform(input, output);
	}
	else
	{
		ViFourierTransformer<T>::variableInverseTransform(input, output);
	}
}

template <typename T>
void ViFourierTransformer<T>::fixedForwardTransform(T input[], T output[])
{
	mFixedForwardThread.setData(input, output);
	mFixedForwardThread.start();
}

template <typename T>
void ViFourierTransformer<T>::fixedInverseTransform(T input[], T output[])
{
	mFixedInverseThread.setData(input, output);
	mFixedInverseThread.start();
}

template <typename T>
void ViFourierTransformer<T>::variableForwardTransform(T input[], T output[], qint32 numberOfSamples)
{
	mVariableForwardThread.setData(input, output, numberOfSamples);
	mVariableForwardThread.start();
}

template <typename T>
void ViFourierTransformer<T>::variableInverseTransform(T input[], T output[], qint32 numberOfSamples)
{
	mVariableInverseThread.setData(input, output, numberOfSamples);
	mVariableInverseThread.start();
}

#endif
