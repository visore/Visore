#include "viwindowfunction.h"

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
		return new ViHannFunction<T>;
	}
	return NULL;
}

template <typename T>
QStringList ViWindowFunctionManager<T>::functions()
{
	QStringList result;
	result << "Rectangular" << "Hamming" << "Hann";
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
T ViRectangularFunction<T>::calculate(int currentSample, int totalSamples)
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
T ViHammingFunction<T>::calculate(int currentSample, int totalSamples)
{
	return 0.54 + (0.46 * qCos((2 * M_PI * currentSample) / (totalSamples - 1)));
}

template class ViHammingFunction<short>;
template class ViHammingFunction<int>;
template class ViHammingFunction<long>;
template class ViHammingFunction<float>;
template class ViHammingFunction<double>;

/**********************************************************
ViHannFunction
**********************************************************/

template <typename T>
T ViHannFunction<T>::calculate(int currentSample, int totalSamples)
{
	return 0.5 * (1 - qCos((2 * M_PI * currentSample) / (totalSamples - 1)));
}

template class ViHannFunction<short>;
template class ViHannFunction<int>;
template class ViHannFunction<long>;
template class ViHannFunction<float>;
template class ViHannFunction<double>;
