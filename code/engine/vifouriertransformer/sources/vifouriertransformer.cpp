#ifdef VIFOURIERTRANSFORMER_H

template <typename T>
ViFourierTransformerThread<T>::ViFourierTransformerThread()
	: QThread()
{
	mInput = NULL;
	mOutput = NULL;
}

template <typename T>
void ViFourierTransformerThread<T>::setData(T input[], T output[])
{
	mInput = input;
	mOutput = output;
}

template <typename T>
ViFourierTransformerForwardThread<T>::ViFourierTransformerForwardThread()
	: ViFourierTransformerThread<T>()
{
}

template <typename T>
void ViFourierTransformerForwardThread<T>::run()
{
	ViFourierTransformerThread<T>::mFourierTransform.mFourierTransform.do_fft(ViFourierTransformerThread<T>::mInput, ViFourierTransformerThread<T>::mOutput);
}

template <typename T>
ViFourierTransformerInverseThread<T>::ViFourierTransformerInverseThread()
	: ViFourierTransformerThread<T>()
{
}

template <typename T>
void ViFourierTransformerInverseThread<T>::run()
{
	ViFourierTransformerThread<T>::mFourierTransform.mFourierTransform.do_ifft(ViFourierTransformerThread<T>::mInput, ViFourierTransformerThread<T>::mOutput);
}

template <typename T>
ViFourierTransformer<T>::ViFourierTransformer()
	: QObject()
{
	QObject::connect(&mForwardThread, SIGNAL(finished()), this, SIGNAL(finished()));
	QObject::connect(&mInverseThread, SIGNAL(finished()), this, SIGNAL(finished()));
}

template <typename T>
void ViFourierTransformer<T>::transform(T input[], T output[], Direction direction)
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

template <typename T>
void ViFourierTransformer<T>::forwardTransform(T input[], T output[])
{
	mForwardThread.setData(input, output);
	mForwardThread.start();
}

template <typename T>
void ViFourierTransformer<T>::inverseTransform(T input[], T output[])
{
	mInverseThread.setData(input, output);
	mInverseThread.start();
}

#endif
