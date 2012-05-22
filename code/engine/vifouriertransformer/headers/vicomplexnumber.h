#ifndef VICOMPLEXNUMBER_H
#define VICOMPLEXNUMBER_H

template <typename T>
class ViComplexNumber
{

	public:

		ViComplexNumber();
		ViComplexNumber(T real, T imaginary);

		void setReal(T real);
		void setImaginary(T imaginary);
		void set(T real, T imaginary);
		void set(ViComplexNumber complex);

		T real();
		T imaginary();

	private:

		T mReal;
		T mImaginary;

};

typedef ViComplexNumber<float> ViComplexFloat;
typedef ViComplexNumber<double> ViComplexDouble;
typedef ViComplexNumber<int> ViComplexInteger;

#include "../sources/vicomplexnumber.cpp"

#endif
