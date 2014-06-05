#ifndef VIEIGEN_H
#define VIEIGEN_H

// Must add file like this, because the Eigen "include" files have a relative path to the src directory
#include <../../../external/eigen/Dense>
#include <../../../external/eigen/MPRealSupport>
#include <gmpxx.h>

using namespace mpfr;

typedef mpreal bigreal;

typedef Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> EigenFloatMatrix;
typedef Eigen::Matrix<float, Eigen::Dynamic, 1> EigenFloatVector;
typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> EigenDoubleMatrix;
typedef Eigen::Matrix<double, Eigen::Dynamic, 1> EigenDoubleVector;
typedef Eigen::Matrix<mpreal, Eigen::Dynamic, Eigen::Dynamic> EigenMprealMatrix;
typedef Eigen::Matrix<mpreal, Eigen::Dynamic, 1> EigenMprealVector;


class ViEigenMatrixZ
{

	public:

		virtual void get(double &result, const int &row, const int &column) {}
		virtual void get(double &result, const int &row, const int &column) const {}
		virtual void get(mpreal &result, const int &row, const int &column) {}
		virtual void get(mpreal &result, const int &row, const int &column) const {}

		virtual void set(const int &row, const int &column, const double &value) {}
		virtual void set(const int &row, const int &column, const mpreal &value) {}

};

class ViEigenVectorZ
{

	public:

		virtual void get(double &result, const int &row) {}
		virtual void get(double &result, const int &row) const {}
		virtual void get(mpreal &result, const int &row) {}
		virtual void get(mpreal &result, const int &row) const {}

		virtual void set(const int &row, const double &value) {}
		virtual void set(const int &row, const mpreal &value) {}

};
/*
class ViEigenZ
{

	public:

		virtual ViEigenMatrixZ* createMatrix() = 0;
		virtual ViEigenMatrixZ* createMatrix(const int &size) = 0;
		virtual ViEigenMatrixZ* createMatrix(const int &rows, const int &columns) = 0;
		virtual ViEigenVectorZ* createVector() = 0;
		virtual ViEigenVectorZ* createVector(const int &rows) = 0;
		virtual ViEigenVectorZ* solve(const ViEigenMatrixZ *matrix, const ViEigenVectorZ *vector) = 0;

		static void clear(ViEigenMatrixZ *matrix)
		{
			delete matrix;
			matrix = NULL;
		}

		static void clear(ViEigenVectorZ *vector)
		{
			delete vector;
			vector = NULL;
		}

};*/

// DOUBLE

class ViEigenDoubleMatrixZ : public ViEigenMatrixZ, public EigenDoubleMatrix
{

	public:

		ViEigenDoubleMatrixZ()
			: EigenDoubleMatrix()
		{}

		ViEigenDoubleMatrixZ(const int &size)
			: EigenDoubleMatrix(size, size)
		{}

		ViEigenDoubleMatrixZ(const int &rows, const int &columns)
			: EigenDoubleMatrix(rows, columns)
		{}

		void get(double &result, const int &row, const int &column)
		{
			result = coeffRef(row, column);
		}

		void get(double &result, const int &row, const int &column) const
		{
			result = coeffRef(row, column);
		}

		void set(const int &row, const int &column, const double &value)
		{
			coeffRef(row, column) = value;
		}

};

class ViEigenDoubleVectorZ : public ViEigenVectorZ, public EigenDoubleVector
{

	public:

		ViEigenDoubleVectorZ()
			: EigenDoubleVector()
		{}

		ViEigenDoubleVectorZ(const int &rows)
			: EigenDoubleVector(rows)
		{}

		ViEigenDoubleVectorZ(const EigenDoubleVector &other)
			: EigenDoubleVector(other)
		{}

		void get(double &result, const int &row)
		{
			result = coeffRef(row);
		}

		void get(double &result, const int &row) const
		{
			result = coeffRef(row);
		}

		void set(const int &row, const double &value)
		{
			coeffRef(row) = value;
		}

};

class ViEigenBaseZ
{

	public:

		virtual ViEigenMatrixZ* createMatrix() = 0;
		virtual ViEigenMatrixZ* createMatrix(const int &size) = 0;
		virtual ViEigenMatrixZ* createMatrix(const int &rows, const int &columns) = 0;
		virtual ViEigenVectorZ* createVector() = 0;
		virtual ViEigenVectorZ* createVector(const int &rows) = 0;
		virtual ViEigenVectorZ* solve(const ViEigenMatrixZ *matrix, const ViEigenVectorZ *vector) = 0;

		static void clear(ViEigenMatrixZ *matrix)
		{
			delete matrix;
			matrix = NULL;
		}

		static void clear(ViEigenVectorZ *vector)
		{
			delete vector;
			vector = NULL;
		}

};


template <class T1, class T2, class T3>
class ViEigenZ : public ViEigenBaseZ
{

	public:

		const double& value(const ViEigenMatrixZ *matrix, const int &row, const int &column) const
		{
			return ((T1*) matrix)->coeff(row, column);
		}

		double& value(ViEigenMatrixZ *matrix, const int &row, const int &column) const
		{
			return ((T1*) matrix)->coeffRef(row, column);
		}

		const double& value(const ViEigenVectorZ *vector, const int &row) const
		{
			return ((T2*) vector)->coeff(row);
		}

		double& value(ViEigenVectorZ *vector, const int &row) const
		{
			return ((T2*) vector)->coeffRef(row);
		}

		ViEigenMatrixZ* createMatrix()
		{
			return new T1();
		}

		ViEigenMatrixZ* createMatrix(const int &size)
		{
			return new T1(size);
		}

		ViEigenMatrixZ* createMatrix(const int &rows, const int &columns)
		{
			return new T1(rows, columns);
		}

		ViEigenVectorZ* createVector()
		{
			return new T2();
		}

		ViEigenVectorZ* createVector(const int &rows)
		{
			return new T2(rows);
		}

		ViEigenVectorZ* solve(const ViEigenMatrixZ *matrix, const ViEigenVectorZ *vector)
		{
			return cast(cast(matrix)->fullPivHouseholderQr().solve(*cast(vector)));
		}

	protected:

		inline const T1* cast(const ViEigenMatrixZ *matrix) const
		{
			return (const T1*) matrix;
		}

		inline const T2* cast(const ViEigenVectorZ *vector) const
		{
			return (const T2*) vector;
		}

		inline T2* cast(const T3 &vector) const
		{
			return new T2(vector);
		}

};

typedef ViEigenZ<ViEigenDoubleMatrixZ, ViEigenDoubleVectorZ, EigenDoubleVector> ViEigenDoubleZ;

/*
class ViEigenDoubleZ : public ViEigenZ
{

	public:

		const double& value(const ViEigenMatrixZ *matrix, const int &row, const int &column) const
		{
			return ((ViEigenDoubleMatrixZ*) matrix)->coeff(row, column);
		}

		double& value(ViEigenMatrixZ *matrix, const int &row, const int &column) const
		{
			return ((ViEigenDoubleMatrixZ*) matrix)->coeffRef(row, column);
		}

		const double& value(const ViEigenVectorZ *vector, const int &row) const
		{
			return ((ViEigenDoubleVectorZ*) vector)->coeff(row);
		}

		double& value(ViEigenVectorZ *vector, const int &row) const
		{
			return ((ViEigenDoubleVectorZ*) vector)->coeffRef(row);
		}

		ViEigenMatrixZ* createMatrix()
		{
			return new ViEigenDoubleMatrixZ();
		}

		ViEigenMatrixZ* createMatrix(const int &size)
		{
			return new ViEigenDoubleMatrixZ(size);
		}

		ViEigenMatrixZ* createMatrix(const int &rows, const int &columns)
		{
			return new ViEigenDoubleMatrixZ(rows, columns);
		}

		ViEigenVectorZ* createVector()
		{
			return new ViEigenDoubleVectorZ();
		}

		ViEigenVectorZ* createVector(const int &rows)
		{
			return new ViEigenDoubleVectorZ(rows);
		}

		ViEigenVectorZ* solve(const ViEigenMatrixZ *matrix, const ViEigenVectorZ *vector)
		{
			return cast(cast(matrix)->fullPivHouseholderQr().solve(*cast(vector)));
		}

	protected:

		inline const ViEigenDoubleMatrixZ* cast(const ViEigenMatrixZ *matrix) const
		{
			return (const ViEigenDoubleMatrixZ*) matrix;
		}

		inline const ViEigenDoubleVectorZ* cast(const ViEigenVectorZ *vector) const
		{
			return (const ViEigenDoubleVectorZ*) vector;
		}

		inline ViEigenDoubleVectorZ* cast(const EigenDoubleVector &vector) const
		{
			return new ViEigenDoubleVectorZ(vector);
		}

};*/












// Template is the decimal bit precision
// 0 - 24: float
// 25 - 53: double
// 54 and above: mpreal
template <int T>
class ViEigen
{

	public:

		typedef typename std::conditional<(T <= 24), float, typename std::conditional<(T <= 53), double, mpreal>::type>::type TYPE;

		typedef Eigen::Matrix<TYPE, Eigen::Dynamic, Eigen::Dynamic> Matrix;
		typedef Eigen::Matrix<TYPE, Eigen::Dynamic, 1> Vector;

		/*typedef mpreal TYPE;

		typedef Eigen::Matrix<TYPE, Eigen::Dynamic, Eigen::Dynamic> Matrix;
		typedef Eigen::Matrix<TYPE, Eigen::Dynamic, 1> Vector;*/

	public:

		static Matrix createMatrix()
		{
			/*if(T > 53)*/ mpreal::set_default_prec(T);
			return Matrix();
		}

		static Matrix createMatrix(const int &size)
		{
			if(T > 53) mpreal::set_default_prec(T);
			return Matrix(size, size);
		}

		static Matrix createMatrix(const int &rows, const int &columns)
		{
			/*if(T > 53)*/ mpreal::set_default_prec(T);
			return Matrix(rows, columns);
		}

		static Matrix createVector()
		{
			/*if(T > 53)*/ mpreal::set_default_prec(T);
			return Vector();
		}

		static Matrix createVector(const int &size)
		{
			/*if(T > 53) */mpreal::set_default_prec(T);
			return Vector(size);
		}

		static Vector solve(const Matrix &matrix, const Vector &vector)
		{
			return matrix.fullPivHouseholderQr().solve(vector);
		}

		static void solve(const Matrix &matrix, const Vector &vector, Vector &coefficients)
		{
			coefficients = matrix.fullPivHouseholderQr().solve(vector);
		}

		static TYPE power(const double &base, const int &exponent)
		{
			TYPE temp = base;
			return std::pow(temp, exponent);
		}

		static void power(TYPE &result, const double &base, const int &exponent)
		{
			TYPE temp = base;
			result = std::pow(temp, exponent);
		}

		static double toReal(const mpreal &real)
		{
			return real.toLDouble();
		}

		static double toReal(const double &real)
		{
			return real;
		}

};

typedef ViEigen<24> ViEigenFloat;
typedef ViEigen<53> ViEigenDouble;
typedef ViEigen<128> ViEigen128;
typedef ViEigen<256> ViEigen256;
typedef ViEigen<512> ViEigen512;

#endif
