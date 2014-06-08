#ifndef VIMATH_H
#define VIMATH_H

template<typename T>
class ViMath
{

	public:

		static T mean(const T *values, const int &size);
		static T mean(const T *valuesLeft, const int &sizeLeft, const T *valuesRight, const int &sizeRight);

		static T median(T *values, const int &size); // modifies input value array
		static T median(T *values, const int &size, const int &position); // modifies input value array

};

//Template decleration and implementation should actually be in the same file
#include "../sources/vimath.cpp"

#endif
