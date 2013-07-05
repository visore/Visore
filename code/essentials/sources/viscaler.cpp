#ifdef VISCALER_H

template <typename T>
T ViScaler<T>::scale(const T &value, const T &fromMinimum, const T &fromMaximum, const T &toMinimum, const T &toMaximum)
{
	return (value - fromMinimum) * (toMaximum - toMinimum) / (fromMaximum - fromMinimum) + toMinimum;
}

template <typename T>
void ViScaler<T>::scale(const ViChunk<T> &input, ViChunk<T> &output, const T &fromMinimum, const T &fromMaximum, const T &toMinimum, const T &toMaximum)
{
	int size = input.size();
	output.resize(size);
	for(int i = 0; i < size; ++i)
	{
		output[i] = (input[i] - fromMinimum) * (toMaximum - toMinimum) / (fromMaximum - fromMinimum) + toMinimum;
	}
}

template <typename T>
void ViScaler<T>::scale(ViChunk<T> &values, const T &fromMinimum, const T &fromMaximum, const T &toMinimum, const T &toMaximum)
{
	int size = values.size();
	for(int i = 0; i < size; ++i)
	{
		values[i] = (values[i] - fromMinimum) * (toMaximum - toMinimum) / (fromMaximum - fromMinimum) + toMinimum;
	}
}

#endif
