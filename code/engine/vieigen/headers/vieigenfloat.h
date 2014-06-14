#ifndef VIEIGENFLOAT_H
#define VIEIGENFLOAT_H

#include <vieigenbase.h>

class ViEigenMatrixFloat : public ViEigenBaseMatrix, public EigenMatrixFloat
{

	public:

		ViEigenMatrixFloat();
		ViEigenMatrixFloat(const int &size);
		ViEigenMatrixFloat(const int &rows, const int &columns);

		void get(double &result, const int &row, const int &column);
		void get(double &result, const int &row, const int &column) const;
		void get(mpfr::mpreal &result, const int &row, const int &column);
		void get(mpfr::mpreal &result, const int &row, const int &column) const;

		void set(const int &row, const int &column, const double &value);
		void set(const int &row, const int &column, const mpfr::mpreal &value);

		void setPower(const int &row, const int &column, const double &base, const int &exponent);
		void setPowerMulti(const int &row, const int &column, const double &multi, const double &base, const int &exponent);

		void setSin(const int &row, const int &column, const double &value);
		void setCos(const int &row, const int &column, const double &value);
		void setSinMulti(const int &row, const int &column, const double &multi, const double &value);
		void setCosMulti(const int &row, const int &column, const double &multi, const double &value);

		QString toString(const int &precision = 5) const;

};

class ViEigenVectorFloat : public ViEigenBaseVector, public EigenVectorFloat
{

	public:

		ViEigenVectorFloat();
		ViEigenVectorFloat(const int &rows);
		ViEigenVectorFloat(const EigenVectorFloat &other);

		void get(double &result, const int &row);
		void get(double &result, const int &row) const;
		void get(mpfr::mpreal &result, const int &row);
		void get(mpfr::mpreal &result, const int &row) const;

		void set(const int &row, const double &value);
		void set(const int &row, const mpfr::mpreal &value);

		QString toString(const int &precision = 5) const;

};

class ViEigenFloat : public ViEigenBase
{

	public:

		void intialize(mpfr::mpreal &value);

		ViEigenBaseMatrix* createMatrix();
		ViEigenBaseMatrix* createMatrix(const int &size);
		ViEigenBaseMatrix* createMatrix(const int &rows, const int &columns);
		ViEigenBaseVector* createVector();
		ViEigenBaseVector* createVector(const int &rows);

		ViEigenBaseVector* estimate(const ViEigenBaseMatrix *matrix, const ViEigenBaseVector *vector);
		void solve(const ViEigenBaseVector *coefficients, const ViEigenBaseMatrix *values, double *output, const int &outputSize, const int &offset = 0);
		void solve(const ViEigenBaseVector *coefficients, const ViEigenBaseMatrix *values, double *output, const int &outputSize, const int *offset);

		void clear(ViEigenBaseMatrix *matrix);
		void clear(ViEigenBaseVector *vector);

	protected:

		inline const ViEigenMatrixFloat* cast(const ViEigenBaseMatrix *matrix) const;
		inline const ViEigenVectorFloat* cast(const ViEigenBaseVector *vector) const;
		inline ViEigenMatrixFloat* cast(ViEigenBaseMatrix *matrix) const;
		inline ViEigenVectorFloat* cast(ViEigenBaseVector *vector) const;
		inline ViEigenVectorFloat* cast(const EigenVectorFloat &vector) const;
};

#endif
