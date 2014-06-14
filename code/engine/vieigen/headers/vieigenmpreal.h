#ifndef VIEIGENMPREAL_H
#define VIEIGENMPREAL_H

#include <vieigenbase.h>

class ViEigenMatrixMpreal : public ViEigenBaseMatrix, public EigenMatrixMpreal
{

	public:

		ViEigenMatrixMpreal();
		ViEigenMatrixMpreal(const int &size);
		ViEigenMatrixMpreal(const int &rows, const int &columns);

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

	private:

		mpfr::mpreal mTemp;

};

class ViEigenVectorMpreal : public ViEigenBaseVector, public EigenVectorMpreal
{

	public:

		ViEigenVectorMpreal();
		ViEigenVectorMpreal(const int &rows);
		ViEigenVectorMpreal(const EigenVectorMpreal &other);

		void get(double &result, const int &row);
		void get(double &result, const int &row) const;
		void get(mpfr::mpreal &result, const int &row);
		void get(mpfr::mpreal &result, const int &row) const;

		void set(const int &row, const double &value);
		void set(const int &row, const mpfr::mpreal &value);

		QString toString(const int &precision = 5) const;

};

template <int P>
class ViEigenMpreal : public ViEigenBase
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

		inline const ViEigenMatrixMpreal* cast(const ViEigenBaseMatrix *matrix) const;
		inline const ViEigenVectorMpreal* cast(const ViEigenBaseVector *vector) const;
		inline ViEigenMatrixMpreal* cast(ViEigenBaseMatrix *matrix) const;
		inline ViEigenVectorMpreal* cast(ViEigenBaseVector *vector) const;
		inline ViEigenVectorMpreal* cast(const EigenVectorMpreal &vector) const;
		inline double cast(const mpfr::mpreal &value);

};

#endif
