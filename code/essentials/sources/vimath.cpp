#ifdef VIMATH_H

// Curly-brackets needed, so that SWAP can be used on a signle-line if-statement
#define SWAP(a,b) { temp = a; a = b; b = temp; }

template<typename T>
T ViMath<T>::mean(const T *values, const int &size)
{
	static int i;
	static T result;
	result = 0;
	for(i = 0; i < size; ++i) result += values[i];
	return result / size;
}

template<typename T>
T ViMath<T>::mean(const T *valuesLeft, const int &sizeLeft, const T *valuesRight, const int &sizeRight)
{
	static int i;
	static T result;
	result = 0;
	for(i = 0; i < sizeLeft; ++i) result += valuesLeft[i];
	for(i = 0; i < sizeRight; ++i) result += valuesRight[i];
	return result / (sizeLeft + sizeRight);
}

template<typename T>
T ViMath<T>::median(T *values, const int &size)
{
	static int position;
	position = size / 2;
	return median(values, size, position);
}

template<typename T>
T ViMath<T>::median(T *values, const int &size, const int &position)
{
	// The quickselect algorithm
	// http://www.stat.cmu.edu/~ryantibs/median/

	static quint64 i, ir, j, l, m, mid;
	static T a, temp; // temp is used by SWAP

	l = 0;
	ir = size - 1;
	while(true)
	{
		m = l + 1;
		if(ir <= m)
		{
			if(ir == m && values[ir] < values[l]) SWAP(values[l], values[ir]);
			return values[position];
		}
		else
		{
			mid = (l + ir) >> 1;
			SWAP(values[mid], values[m]);
			if(values[l] > values[ir]) SWAP(values[l], values[ir]);
			if(values[m] > values[ir]) SWAP(values[m], values[ir]);
			if(values[l] > values[m]) SWAP(values[l], values[m]);
			i = l + 1;
			j = ir;
			a = values[m];
			while(true)
			{
				do ++i; while(values[i] < a);
				do --j; while(values[j] > a);
				if(j < i) break;
				SWAP(values[i], values[j]);
			}
			values[m] = values[j];
			values[j] = a;
			if(j >= position) ir = j - 1;
			if(j <= position) l = i;
		}
	}
}

template<typename T>
T ViMath<T>::variance(const T *values, const int &size, const bool &unbiased)
{
	static int i;
	static T result, theMean;

	result = 0;
	theMean = mean(values, size);

	for(i = 0; i < size; ++i) result += pow(values[i] - theMean, 2);

	if(unbiased) return result / (size - 1);
	return result / size;
}

template<typename T>
T ViMath<T>::variance(const T *valuesLeft, const int &sizeLeft, const T *valuesRight, const int &sizeRight, const bool &unbiased)
{
	static int i;
	static T result, theMean;

	result = 0;
	theMean = mean(valuesLeft, sizeLeft, valuesRight, sizeRight);

	for(i = 0; i < sizeLeft; ++i) result += pow(valuesLeft[i] - theMean, 2);
	for(i = 0; i < sizeRight; ++i) result += pow(valuesRight[i] - theMean, 2);

	if(unbiased) return result / (sizeLeft + sizeRight - 1);
	return result / (sizeLeft + sizeRight);
}

template<typename T>
T ViMath<T>::noise(const T &variance)
{
	static bool hasSpare = false;
	static T spare;

	if(hasSpare)
	{
		hasSpare = false;
		return spare * variance;
	}

	hasSpare = true;
	static qreal u, v, s;
	do
	{
		u = (rand() / ((double) RAND_MAX)) * 2 - 1;
		v = (rand() / ((double) RAND_MAX)) * 2 - 1;
		s = u * u + v * v;
	}
	while(s >= 1 || s == 0);

	s = sqrt(-2.0 * log(s) / s);
	spare = v * s;
	return variance * u * s;
}

template<typename T>
void ViMath<T>::noise(const T *values, const int &size, T *noiseOutput, const int &noiseSize)
{
	static int i;
	static T theVariance;
	theVariance = variance(values, size);
	for(i = 0; i < noiseSize; ++i) noiseOutput[i] = noise(theVariance);
}

template<typename T>
void ViMath<T>::noise(const T *valuesLeft, const int &sizeLeft, const T *valuesRight, const int &sizeRight, T *noiseOutput, const int &noiseSize)
{
	static int i;
	static T theVariance;
	theVariance = variance(valuesLeft, sizeLeft, valuesRight, sizeRight);
	for(i = 0; i < noiseSize; ++i) noiseOutput[i] = noise(theVariance);
}

#endif
