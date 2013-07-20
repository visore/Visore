#ifndef VICOMPLEXNUMBER_H
#define VICOMPLEXNUMBER_H

template <typename T>
class ViComplexNumber
{

	public:

		ViComplexNumber();
		ViComplexNumber(const ViComplexNumber<T> &other);
		ViComplexNumber(const T &real, const T &imaginary);

		void setReal(const T &real);
		void setImaginary(const T &imaginary);
		void set(const T &real, const T &imaginary);
		void set(const ViComplexNumber &complex);

		const T& real() const;
		const T& imaginary() const;

		bool operator == (const ViComplexNumber<T> &complex) const;
		bool operator != (const ViComplexNumber<T> &complex) const;

		const ViComplexNumber<T>& operator += (const ViComplexNumber<T> &complex);
		const ViComplexNumber<T>& operator -= (const ViComplexNumber<T> &complex);

		const ViComplexNumber<T>& operator /= (const T &value);
		const ViComplexNumber<T>& operator *= (const T &value);

		ViComplexNumber<T> operator / (const T &value) const;
		ViComplexNumber<T> operator * (const T &value) const;

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
