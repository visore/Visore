#ifndef VIMATH_H
#define VIMATH_H

template<typename T>
class ViMath
{

	public:

		// MEAN
		static T mean(const T *values, const int &size);
		static T mean(const T *valuesLeft, const int &sizeLeft, const T *valuesRight, const int &sizeRight);

		// MEDIAN
		static T median(T *values, const int &size); // modifies input value array
		static T median(T *values, const int &size, const int &position); // modifies input value array

		// VARIANCE
		static T variance(const T *values, const int &size, const bool &unbiased = true);
		static T variance(const T *valuesLeft, const int &sizeLeft, const T *valuesRight, const int &sizeRight, const bool &unbiased = true);
		static T variance(const T *values, const int &size, const T &mean, const bool &unbiased = true);
		static T variance(const T *valuesLeft, const int &sizeLeft, const T *valuesRight, const int &sizeRight, const T &mean, const bool &unbiased = true);

		static T varianceBiased(const T *values, const int &size);
		static T varianceBiased(const T *valuesLeft, const int &sizeLeft, const T *valuesRight, const int &sizeRight);
		static T varianceBiased(const T *values, const int &size, const T &mean);
		static T varianceBiased(const T *valuesLeft, const int &sizeLeft, const T *valuesRight, const int &sizeRight, const T &mean);

		static T varianceUnbiased(const T *values, const int &size);
		static T varianceUnbiased(const T *valuesLeft, const int &sizeLeft, const T *valuesRight, const int &sizeRight);
		static T varianceUnbiased(const T *values, const int &size, const T &mean);
		static T varianceUnbiased(const T *valuesLeft, const int &sizeLeft, const T *valuesRight, const int &sizeRight, const T &mean);

		// STANDARD DEVIATION
		static T standardDeviation(const T &variance, const bool &unbiased = true);
		static T standardDeviation(const T *values, const int &size, const bool &unbiased = true);
		static T standardDeviation(const T *valuesLeft, const int &sizeLeft, const T *valuesRight, const int &sizeRight, const bool &unbiased = true);
		static T standardDeviation(const T *values, const int &size, const T &mean, const bool &unbiased = true);
		static T standardDeviation(const T *valuesLeft, const int &sizeLeft, const T *valuesRight, const int &sizeRight, const T &mean, const bool &unbiased = true);

		static T standardDeviationBiased(const T &variance);
		static T standardDeviationBiased(const T *values, const int &size);
		static T standardDeviationBiased(const T *valuesLeft, const int &sizeLeft, const T *valuesRight, const int &sizeRight);
		static T standardDeviationBiased(const T *values, const int &size, const T &mean);
		static T standardDeviationBiased(const T *valuesLeft, const int &sizeLeft, const T *valuesRight, const int &sizeRight, const T &mean);

		static T standardDeviationUnbiased(const T &variance);
		static T standardDeviationUnbiased(const T *values, const int &size);
		static T standardDeviationUnbiased(const T *valuesLeft, const int &sizeLeft, const T *valuesRight, const int &sizeRight);
		static T standardDeviationUnbiased(const T *values, const int &size, const T &mean);
		static T standardDeviationUnbiased(const T *valuesLeft, const int &sizeLeft, const T *valuesRight, const int &sizeRight, const T &mean);

		// NOISE
		static T noise(const T &variance); // Generate Gaussina white noise
		static void noise(const T *values, const int &size, T *noiseOutput, const int &noiseSize);
		static void noise(const T *valuesLeft, const int &sizeLeft, const T *valuesRight, const int &sizeRight, T *noiseOutput, const int &noiseSize);

};

//Template decleration and implementation should actually be in the same file
#include "../sources/vimath.cpp"

#endif
