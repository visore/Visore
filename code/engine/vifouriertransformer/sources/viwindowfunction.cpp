#ifdef VIWINDOWFUNCTION_H

/**********************************************************
ViWindowFunction
**********************************************************/

template <typename T>
void ViWindowFunction<T>::apply(T *data, int size)
{
	for(int i = 0; i < size; ++i)
	{
		data[i] *= mWindow[i];
	}
}

template <typename T>
void ViWindowFunction<T>::create(int size)
{
	if(size != mWindow.size())
	{
		mWindow.clear();
		mWindow.resize(size);
		fillWindow(size);
	}
}

template <typename T>
void ViWindowFunction<T>::fillWindow(int size)
{
	for(int i = 0; i < size; ++i)
	{
		mWindow[i] = calculate(i, size);
	}
}

/**********************************************************
ViWindowFunctionManager
**********************************************************/

template <typename T>
ViWindowFunction<T>* ViWindowFunctionManager<T>::createFunction(QString functionName)
{
	functionName = functionName.trimmed().toLower().replace("function", "");
	if(functionName == "hamming")
	{
		return new ViHammingFunction<T>;
	}
	else if(functionName == "hann")
	{
		return new ViHannFunction<T>;
	}
	return NULL;
}

/**********************************************************
ViRectangularFunction
**********************************************************/

template <typename T>
T ViRectangularFunction<T>::calculate(int currentSample, int totalSamples)
{
	return 1.0;
}

/**********************************************************
ViHammingFunction
**********************************************************/

template <typename T>
T ViHammingFunction<T>::calculate(int currentSample, int totalSamples)
{
	return 0.54 + (0.46 * qCos((2 * M_PI * currentSample) / (totalSamples - 1)));
}

/**********************************************************
ViHannFunction
**********************************************************/

template <typename T>
T ViHannFunction<T>::calculate(int currentSample, int totalSamples)
{
	return 0.5 * (1 - qCos((2 * M_PI * currentSample) / (totalSamples - 1)));
}

#endif
