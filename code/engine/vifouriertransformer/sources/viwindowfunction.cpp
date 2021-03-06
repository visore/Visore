#include "viwindowfunction.h"

/**********************************************************
ViWindowFunction
**********************************************************/

template <typename T>
void ViWindowFunction<T>::apply(const T input[], T output[], const int size)
{
	for(int i = 0; i < size; ++i)
    {
        output[i] = input[i] * mWindow[i];
	}
}

template <typename T>
void ViWindowFunction<T>::create(const int size)
{
	if(size != mWindow.size())
	{
		mWindow.clear();
		mWindow.resize(size);
		fillWindow(size);
	}
}

template <typename T>
void ViWindowFunction<T>::fillWindow(const int size)
{
	for(int i = 0; i < size; ++i)
	{
		mWindow[i] = calculate(i, size);
	}
}

template class ViWindowFunction<short>;
template class ViWindowFunction<int>;
template class ViWindowFunction<long>;
template class ViWindowFunction<float>;
template class ViWindowFunction<double>;

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
		return new ViHanningFunction<T>;
	}
	return NULL;
}

template <typename T>
QStringList ViWindowFunctionManager<T>::functions()
{
	QStringList result;
	result << "Rectangular" << "Hamming" << "Hanning";
	return result;
}

template class ViWindowFunctionManager<short>;
template class ViWindowFunctionManager<int>;
template class ViWindowFunctionManager<long>;
template class ViWindowFunctionManager<float>;
template class ViWindowFunctionManager<double>;

/**********************************************************
ViRectangularFunction
**********************************************************/

template <typename T>
T ViRectangularFunction<T>::calculate(const int currentSample, const int totalSamples)
{
	return 1.0;
}

template class ViRectangularFunction<short>;
template class ViRectangularFunction<int>;
template class ViRectangularFunction<long>;
template class ViRectangularFunction<float>;
template class ViRectangularFunction<double>;

/**********************************************************
ViHammingFunction
**********************************************************/

template <typename T>
T ViHammingFunction<T>::calculate(const int currentSample, const int totalSamples)
{
	return 0.54 + (0.46 * qCos((2 * M_PI * currentSample) / (totalSamples - 1)));
}

template class ViHammingFunction<short>;
template class ViHammingFunction<int>;
template class ViHammingFunction<long>;
template class ViHammingFunction<float>;
template class ViHammingFunction<double>;

/**********************************************************
ViHanningFunction
**********************************************************/

template <typename T>
T ViHanningFunction<T>::calculate(const int currentSample, const int totalSamples)
{
	return 0.5 * (1 - qCos((2 * M_PI * currentSample) / (totalSamples - 1)));
}

template class ViHanningFunction<short>;
template class ViHanningFunction<int>;
template class ViHanningFunction<long>;
template class ViHanningFunction<float>;
template class ViHanningFunction<double>;
