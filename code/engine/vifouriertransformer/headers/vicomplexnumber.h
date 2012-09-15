#ifndef VICOMPLEXNUMBER_H
#define VICOMPLEXNUMBER_H

template <typename T>
class ViComplexNumber
{

	public:

		ViComplexNumber();
		ViComplexNumber(const ViComplexNumber<T> &other);
		ViComplexNumber(T real, T imaginary);

		void setReal(T real);
		void setImaginary(T imaginary);
		void set(T real, T imaginary);
		void set(ViComplexNumber complex);

		T real();
		T imaginary();

		bool operator==(const ViComplexNumber<T> &complex) const;
		bool operator!=(const ViComplexNumber<T> &complex) const;

		ViComplexNumber<T>& operator+=(const ViComplexNumber<T> &complex);
		ViComplexNumber<T>& operator-=(const ViComplexNumber<T> &complex);

		ViComplexNumber<T>& operator/=(const T &value);
		ViComplexNumber<T>& operator*=(const T &value);

		ViComplexNumber<T> operator/(const T &value) const;
		ViComplexNumber<T> operator*(const T &value) const;

	private:

		T mReal;
		T mImaginary;

};

typedef ViComplexNumber<float> ViFloatComplex;
typedef ViComplexNumber<double> ViDoubleComplex;
typedef ViDoubleComplex ViRealComplex;
typedef ViComplexNumber<int> ViIntegerComplex;
typedef ViComplexNumber<short> ViShortComplex;

#endif
