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
/*
template <int P>
class ViEigenMpreal : public ViEigenBase
{

	public:

		void intialize(double &value) {}

		void intialize(mpfr::mpreal &value)
		{
			mpfr::mpreal::set_default_prec(P);
			value = mpfr::mpreal();
		}

		ViEigenBaseMatrix* createMatrix()
		{
			mpfr::mpreal::set_default_prec(P);
			return new ViEigenMatrixMpreal();
		}

		ViEigenBaseMatrix* createMatrix(const int &size)
		{
			mpfr::mpreal::set_default_prec(P);
			return new ViEigenMatrixMpreal(size);
		}

		ViEigenBaseMatrix* createMatrix(const int &rows, const int &columns)
		{
			mpfr::mpreal::set_default_prec(P);
			return new ViEigenMatrixMpreal(rows, columns);
		}

		ViEigenBaseVector* createVector()
		{
			mpfr::mpreal::set_default_prec(P);
			return new ViEigenVectorMpreal();
		}

		ViEigenBaseVector* createVector(const int &rows)
		{
			mpfr::mpreal::set_default_prec(P);
			return new ViEigenVectorMpreal(rows);
		}

		ViEigenBaseVector* estimate(const ViEigenBaseMatrix *matrix, const ViEigenBaseVector *vector)
		{
			return cast(cast(matrix)->fullPivHouseholderQr().solve(*cast(vector)));
		}

		void solve(const ViEigenBaseVector *coefficients, const ViEigenBaseMatrix *values, double *output, const int &outputSize)
		{
			const ViEigenVectorMpreal *castCoefficients = cast(coefficients);
			const ViEigenMatrixMpreal *castValues = cast(values);
			mpfr::mpreal result;
			int i, j;

			for(i = 0; i < outputSize; ++i)
			{
				result = 0;
				for(j = 0; j < castCoefficients->rows(); ++j)
				{
					result += castCoefficients->coeffRef(j) * castValues->coeffRef(i, j);
				}
				output[i] = cast(result);
			}
		}

		void clear(ViEigenBaseMatrix *matrix)
		{
			delete cast(matrix); // Cast first, otherwise there are memory leaks
			matrix = NULL;
		}

		void clear(ViEigenBaseVector *vector)
		{
			delete cast(vector); // Cast first, otherwise there are memory leaks
			vector = NULL;
		}

	protected:

		inline const ViEigenMatrixMpreal* cast(const ViEigenBaseMatrix *matrix) const
		{
			return (const ViEigenMatrixMpreal*) matrix;
		}

		inline const ViEigenVectorMpreal* cast(const ViEigenBaseVector *vector) const
		{
			return (const ViEigenVectorMpreal*) vector;
		}

		inline ViEigenMatrixMpreal* cast(ViEigenBaseMatrix *matrix) const
		{
			return (ViEigenMatrixMpreal*) matrix;
		}

		inline ViEigenVectorMpreal* cast(ViEigenBaseVector *vector) const
		{
			return (ViEigenVectorMpreal*) vector;
		}

		inline ViEigenVectorMpreal* cast(const EigenVectorMpreal &vector) const
		{
			return new ViEigenVectorMpreal(vector);
		}

		double cast(const mpfr::mpreal &value)
		{
			return value.toDouble();
		}

};*/

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
		void solve(const ViEigenBaseVector *coefficients, const ViEigenBaseMatrix *values, double *output, const int &outputSize);

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
