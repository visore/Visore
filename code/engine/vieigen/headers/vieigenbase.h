#ifndef VIEIGENBASE_H
#define VIEIGENBASE_H

// Must add file like this, because the Eigen "include" files have a relative path to the src directory
#include <../../../external/eigen/Dense>
#include <../../../external/eigen/MPRealSupport>
#include <QString>

typedef Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> EigenMatrixFloat;
typedef Eigen::Matrix<float, Eigen::Dynamic, 1> EigenVectorFloat;

typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> EigenMatrixDouble;
typedef Eigen::Matrix<double, Eigen::Dynamic, 1> EigenVectorDouble;

typedef Eigen::Matrix<mpfr::mpreal, Eigen::Dynamic, Eigen::Dynamic> EigenMatrixMpreal;
typedef Eigen::Matrix<mpfr::mpreal, Eigen::Dynamic, 1> EigenVectorMpreal;

QString eigenToString(const double &value, const int &precision = 5);
QString eigenToString(const mpfr::mpreal &value, const int &precision = 5);

class ViEigenBaseMatrix
{

	public:

		virtual void get(double &result, const int &row, const int &column) = 0;
		virtual void get(double &result, const int &row, const int &column) const = 0;
		virtual void get(mpfr::mpreal &result, const int &row, const int &column) = 0;
		virtual void get(mpfr::mpreal &result, const int &row, const int &column) const = 0;

		virtual void set(const int &row, const int &column, const double &value) = 0;
		virtual void set(const int &row, const int &column, const mpfr::mpreal &value) = 0;

		virtual void setPower(const int &row, const int &column, const double &base, const int &exponent) = 0;
		virtual void setPowerMulti(const int &row, const int &column, const double &multi, const double &base, const int &exponent) = 0;

		virtual QString toString(const int &precision = 5) const = 0;

};

class ViEigenBaseVector
{

	public:

		virtual void get(double &result, const int &row) = 0;
		virtual void get(double &result, const int &row) const = 0;
		virtual void get(mpfr::mpreal &result, const int &row) = 0;
		virtual void get(mpfr::mpreal &result, const int &row) const = 0;

		virtual void set(const int &row, const double &value) = 0;
		virtual void set(const int &row, const mpfr::mpreal &value) = 0;

		virtual QString toString(const int &precision = 5) const = 0;

};

class ViEigenBase
{

	public:

		virtual void intialize(float &value);
		virtual void intialize(double &value);
		virtual void intialize(mpfr::mpreal &value) = 0;

		virtual ViEigenBaseMatrix* createMatrix() = 0;
		virtual ViEigenBaseMatrix* createMatrix(const int &size) = 0;
		virtual ViEigenBaseMatrix* createMatrix(const int &rows, const int &columns) = 0;
		virtual ViEigenBaseVector* createVector() = 0;
		virtual ViEigenBaseVector* createVector(const int &rows) = 0;

		virtual ViEigenBaseVector* estimate(const ViEigenBaseMatrix *matrix, const ViEigenBaseVector *vector) = 0;
		virtual void solve(const ViEigenBaseVector *coefficients, const ViEigenBaseMatrix *values, double *output, const int &outputSize) = 0;

		virtual void clear(ViEigenBaseMatrix *matrix) = 0;
		virtual void clear(ViEigenBaseVector *vector) = 0;

};

#endif
