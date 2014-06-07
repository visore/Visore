#ifndef VIEIGENDOUBLE_H
#define VIEIGENDOUBLE_H

#include <vieigenbase.h>

class ViEigenMatrixDouble : public ViEigenBaseMatrix, public EigenMatrixDouble
{

	public:

		ViEigenMatrixDouble();
		ViEigenMatrixDouble(const int &size);
		ViEigenMatrixDouble(const int &rows, const int &columns);

		void get(double &result, const int &row, const int &column);
		void get(double &result, const int &row, const int &column) const;
		void get(mpfr::mpreal &result, const int &row, const int &column);
		void get(mpfr::mpreal &result, const int &row, const int &column) const;

		void set(const int &row, const int &column, const double &value);
		void set(const int &row, const int &column, const mpfr::mpreal &value);

		void setPower(const int &row, const int &column, const double &base, const int &exponent);
		void setPowerMulti(const int &row, const int &column, const double &multi, const double &base, const int &exponent);

		QString toString(const int &precision = 5) const;

};

class ViEigenVectorDouble : public ViEigenBaseVector, public EigenVectorDouble
{

	public:

		ViEigenVectorDouble();
		ViEigenVectorDouble(const int &rows);
		ViEigenVectorDouble(const EigenVectorDouble &other);

		void get(double &result, const int &row);
		void get(double &result, const int &row) const;
		void get(mpfr::mpreal &result, const int &row);
		void get(mpfr::mpreal &result, const int &row) const;

		void set(const int &row, const double &value);
		void set(const int &row, const mpfr::mpreal &value);

		QString toString(const int &precision = 5) const;

};

class ViEigenDouble : public ViEigenBase
{

	public:

		void intialize(mpfr::mpreal &value);

		ViEigenBaseMatrix* createMatrix();
		ViEigenBaseMatrix* createMatrix(const int &size);
		ViEigenBaseMatrix* createMatrix(const int &rows, const int &columns);
		ViEigenBaseVector* createVector();
		ViEigenBaseVector* createVector(const int &rows);

		ViEigenBaseVector* estimate(const ViEigenBaseMatrix *matrix, const ViEigenBaseVector *vector);
		void solve(const ViEigenBaseVector *coefficients, const ViEigenBaseMatrix *values, double *output, const int &outputSize);

		void clear(ViEigenBaseMatrix *matrix);
		void clear(ViEigenBaseVector *vector);

	protected:

		inline const ViEigenMatrixDouble* cast(const ViEigenBaseMatrix *matrix) const;
		inline const ViEigenVectorDouble* cast(const ViEigenBaseVector *vector) const;
		inline ViEigenMatrixDouble* cast(ViEigenBaseMatrix *matrix) const;
		inline ViEigenVectorDouble* cast(ViEigenBaseVector *vector) const;
		inline ViEigenVectorDouble* cast(const EigenVectorDouble &vector) const;

};

#endif
