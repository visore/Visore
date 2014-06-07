#include <vieigenmpreal.h>

ViEigenMatrixMpreal::ViEigenMatrixMpreal()
	: EigenMatrixMpreal()
{
}

ViEigenMatrixMpreal::ViEigenMatrixMpreal(const int &size)
	: EigenMatrixMpreal(size, size)
{
}

ViEigenMatrixMpreal::ViEigenMatrixMpreal(const int &rows, const int &columns)
	: EigenMatrixMpreal(rows, columns)
{
}

void ViEigenMatrixMpreal::get(double &result, const int &row, const int &column)
{
	result = this->coeffRef(row, column).toDouble();
}

void ViEigenMatrixMpreal::get(double &result, const int &row, const int &column) const
{
	result = this->coeffRef(row, column).toDouble();
}

void ViEigenMatrixMpreal::get(mpfr::mpreal &result, const int &row, const int &column)
{
	result = this->coeffRef(row, column);
}

void ViEigenMatrixMpreal::get(mpfr::mpreal &result, const int &row, const int &column) const
{
	result = this->coeffRef(row, column);
}

void ViEigenMatrixMpreal::set(const int &row, const int &column, const double &value)
{
	this->coeffRef(row, column) = value;
}

void ViEigenMatrixMpreal::set(const int &row, const int &column, const mpfr::mpreal &value)
{
	this->coeffRef(row, column) = value;
}

void ViEigenMatrixMpreal::setPower(const int &row, const int &column, const double &base, const int &exponent)
{
	mTemp = base;
	set(row, column, mpfr::pow(mTemp, exponent));
}

void ViEigenMatrixMpreal::setPowerMulti(const int &row, const int &column, const double &multi, const double &base, const int &exponent)
{
	mTemp = base;
	set(row, column, multi * mpfr::pow(mTemp, exponent));
}

QString ViEigenMatrixMpreal::toString(const int &precision) const
{
	QString result = "";
	for(int i = 0; i < this->rows(); ++i)
	{
		for(int j = 0; j < this->cols(); ++j)
		{
			result += eigenToString(this->coeff(i, j), precision) + " ";
		}
		result += "\n";
	}
	return result;
}

ViEigenVectorMpreal::ViEigenVectorMpreal()
	: EigenVectorMpreal()
{
}

ViEigenVectorMpreal::ViEigenVectorMpreal(const int &rows)
	: EigenVectorMpreal(rows)
{
}

ViEigenVectorMpreal::ViEigenVectorMpreal(const EigenVectorMpreal &other)
	: EigenVectorMpreal(other)
{
}

void ViEigenVectorMpreal::get(double &result, const int &row)
{
	result = this->coeffRef(row).toDouble();
}

void ViEigenVectorMpreal::get(double &result, const int &row) const
{
	result = this->coeffRef(row).toDouble();
}

void ViEigenVectorMpreal::get(mpfr::mpreal &result, const int &row)
{
	result = this->coeffRef(row);
}

void ViEigenVectorMpreal::get(mpfr::mpreal &result, const int &row) const
{
	result = this->coeffRef(row);
}

void ViEigenVectorMpreal::set(const int &row, const double &value)
{
	this->coeffRef(row) = value;
}

void ViEigenVectorMpreal::set(const int &row, const mpfr::mpreal &value)
{
	this->coeffRef(row) = value;
}

QString ViEigenVectorMpreal::toString(const int &precision) const
{
	QString result = "";
	for(int i = 0; i < this->rows(); ++i)
	{
		result += eigenToString(this->coeff(i), precision) + " ";
	}
	result += "\n";
	return result;
}

template <int P>
void ViEigenMpreal<P>::intialize(mpfr::mpreal &value)
{
	mpfr::mpreal::set_default_prec(P);
	value = mpfr::mpreal();
}

template <int P>
ViEigenBaseMatrix* ViEigenMpreal<P>::createMatrix()
{
	mpfr::mpreal::set_default_prec(P);
	return new ViEigenMatrixMpreal();
}

template <int P>
ViEigenBaseMatrix* ViEigenMpreal<P>::createMatrix(const int &size)
{
	mpfr::mpreal::set_default_prec(P);
	return new ViEigenMatrixMpreal(size);
}

template <int P>
ViEigenBaseMatrix* ViEigenMpreal<P>::createMatrix(const int &rows, const int &columns)
{
	mpfr::mpreal::set_default_prec(P);
	return new ViEigenMatrixMpreal(rows, columns);
}

template <int P>
ViEigenBaseVector* ViEigenMpreal<P>::createVector()
{
	mpfr::mpreal::set_default_prec(P);
	return new ViEigenVectorMpreal();
}

template <int P>
ViEigenBaseVector* ViEigenMpreal<P>::createVector(const int &rows)
{
	mpfr::mpreal::set_default_prec(P);
	return new ViEigenVectorMpreal(rows);
}

template <int P>
ViEigenBaseVector* ViEigenMpreal<P>::estimate(const ViEigenBaseMatrix *matrix, const ViEigenBaseVector *vector)
{
	return cast(cast(matrix)->fullPivHouseholderQr().solve(*cast(vector)));
}

template <int P>
void ViEigenMpreal<P>::solve(const ViEigenBaseVector *coefficients, const ViEigenBaseMatrix *values, double *output, const int &outputSize)
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

template <int P>
void ViEigenMpreal<P>::clear(ViEigenBaseMatrix *matrix)
{
	delete cast(matrix); // Cast first, otherwise there are memory leaks
	matrix = NULL;
}

template <int P>
void ViEigenMpreal<P>::clear(ViEigenBaseVector *vector)
{
	delete cast(vector); // Cast first, otherwise there are memory leaks
	vector = NULL;
}

template <int P>
inline const ViEigenMatrixMpreal* ViEigenMpreal<P>::cast(const ViEigenBaseMatrix *matrix) const
{
	return (const ViEigenMatrixMpreal*) matrix;
}

template <int P>
inline const ViEigenVectorMpreal* ViEigenMpreal<P>::cast(const ViEigenBaseVector *vector) const
{
	return (const ViEigenVectorMpreal*) vector;
}

template <int P>
inline ViEigenMatrixMpreal* ViEigenMpreal<P>::cast(ViEigenBaseMatrix *matrix) const
{
	return (ViEigenMatrixMpreal*) matrix;
}

template <int P>
inline ViEigenVectorMpreal* ViEigenMpreal<P>::cast(ViEigenBaseVector *vector) const
{
	return (ViEigenVectorMpreal*) vector;
}

template <int P>
inline ViEigenVectorMpreal* ViEigenMpreal<P>::cast(const EigenVectorMpreal &vector) const
{
	return new ViEigenVectorMpreal(vector);
}

template <int P>
inline double ViEigenMpreal<P>::cast(const mpfr::mpreal &value)
{
	return value.toDouble();
}

template class ViEigenMpreal<64>;
template class ViEigenMpreal<72>;
template class ViEigenMpreal<80>;
template class ViEigenMpreal<88>;
template class ViEigenMpreal<96>;
template class ViEigenMpreal<104>;
template class ViEigenMpreal<112>;
template class ViEigenMpreal<120>;
template class ViEigenMpreal<128>;
template class ViEigenMpreal<144>;
template class ViEigenMpreal<160>;
template class ViEigenMpreal<176>;
template class ViEigenMpreal<192>;
template class ViEigenMpreal<208>;
template class ViEigenMpreal<224>;
template class ViEigenMpreal<240>;
template class ViEigenMpreal<256>;
template class ViEigenMpreal<288>;
template class ViEigenMpreal<320>;
template class ViEigenMpreal<352>;
template class ViEigenMpreal<384>;
template class ViEigenMpreal<416>;
template class ViEigenMpreal<448>;
template class ViEigenMpreal<480>;
template class ViEigenMpreal<512>;
template class ViEigenMpreal<640>;
template class ViEigenMpreal<768>;
template class ViEigenMpreal<896>;
template class ViEigenMpreal<1024>;
