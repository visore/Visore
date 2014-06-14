#include <vieigendouble.h>

ViEigenMatrixDouble::ViEigenMatrixDouble()
	: EigenMatrixDouble()
{
}

ViEigenMatrixDouble::ViEigenMatrixDouble(const int &size)
	: EigenMatrixDouble(size, size)
{
	int i, j;
	for(i = 0; i < size; ++i)
	{
		for(j = 0; j < size; ++j) this->coeffRef(i, j) = 0;
	}
}

ViEigenMatrixDouble::ViEigenMatrixDouble(const int &rows, const int &columns)
	: EigenMatrixDouble(rows, columns)
{
	int i, j;
	for(i = 0; i < rows; ++i)
	{
		for(j = 0; j < columns; ++j) this->coeffRef(i, j) = 0;
	}
}

void ViEigenMatrixDouble::get(double &result, const int &row, const int &column)
{
	result = this->coeffRef(row, column);
}

void ViEigenMatrixDouble::get(double &result, const int &row, const int &column) const
{
	result = this->coeffRef(row, column);
}

void ViEigenMatrixDouble::get(mpfr::mpreal &result, const int &row, const int &column)
{
	result = this->coeffRef(row, column);
}

void ViEigenMatrixDouble::get(mpfr::mpreal &result, const int &row, const int &column) const
{
	result = this->coeffRef(row, column);
}

void ViEigenMatrixDouble::set(const int &row, const int &column, const double &value)
{
	this->coeffRef(row, column) = value;
}

void ViEigenMatrixDouble::set(const int &row, const int &column, const mpfr::mpreal &value)
{
	this->coeffRef(row, column) = value.toDouble();
}

void ViEigenMatrixDouble::setPower(const int &row, const int &column, const double &base, const int &exponent)
{
	set(row, column, std::pow(base, exponent));
}

void ViEigenMatrixDouble::setPowerMulti(const int &row, const int &column, const double &multi, const double &base, const int &exponent)
{
	set(row, column, multi * std::pow(base, exponent));
}

void ViEigenMatrixDouble::setSin(const int &row, const int &column, const double &value)
{
	set(row, column, std::sin(value));
}

void ViEigenMatrixDouble::setCos(const int &row, const int &column, const double &value)
{
	set(row, column, std::cos(value));
}

void ViEigenMatrixDouble::setSinMulti(const int &row, const int &column, const double &multi, const double &value)
{
	set(row, column, multi * std::sin(value));
}

void ViEigenMatrixDouble::setCosMulti(const int &row, const int &column, const double &multi, const double &value)
{
	set(row, column, multi * std::cos(value));
}

QString ViEigenMatrixDouble::toString(const int &precision) const
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

ViEigenVectorDouble::ViEigenVectorDouble()
	: EigenVectorDouble()
{
}

ViEigenVectorDouble::ViEigenVectorDouble(const int &rows)
	: EigenVectorDouble(rows)
{
	for(int i = 0; i < rows; ++i) this->coeffRef(i) = 0;
}

ViEigenVectorDouble::ViEigenVectorDouble(const EigenVectorDouble &other)
	: EigenVectorDouble(other)
{
}

void ViEigenVectorDouble::get(double &result, const int &row)
{
	result = this->coeffRef(row);
}

void ViEigenVectorDouble::get(double &result, const int &row) const
{
	result = this->coeffRef(row);
}

void ViEigenVectorDouble::get(mpfr::mpreal &result, const int &row)
{
	result = this->coeffRef(row);
}

void ViEigenVectorDouble::get(mpfr::mpreal &result, const int &row) const
{
	result = this->coeffRef(row);
}

void ViEigenVectorDouble::set(const int &row, const double &value)
{
	this->coeffRef(row) = value;
}

void ViEigenVectorDouble::set(const int &row, const mpfr::mpreal &value)
{
	this->coeffRef(row) = value.toDouble();
}

QString ViEigenVectorDouble::toString(const int &precision) const
{
	QString result = "";
	for(int i = 0; i < this->rows(); ++i)
	{
		result += eigenToString(this->coeff(i), precision) + " ";
	}
	result += "\n";
	return result;
}

void ViEigenDouble::intialize(mpfr::mpreal &value)
{
}

ViEigenBaseMatrix* ViEigenDouble::createMatrix()
{
	return new ViEigenMatrixDouble();
}

ViEigenBaseMatrix* ViEigenDouble::createMatrix(const int &size)
{
	return new ViEigenMatrixDouble(size);
}

ViEigenBaseMatrix* ViEigenDouble::createMatrix(const int &rows, const int &columns)
{
	return new ViEigenMatrixDouble(rows, columns);
}

ViEigenBaseVector* ViEigenDouble::createVector()
{
	return new ViEigenVectorDouble();
}

ViEigenBaseVector* ViEigenDouble::createVector(const int &rows)
{
	return new ViEigenVectorDouble(rows);
}

ViEigenBaseVector* ViEigenDouble::estimate(const ViEigenBaseMatrix *matrix, const ViEigenBaseVector *vector)
{
	return cast(cast(matrix)->fullPivHouseholderQr().solve(*cast(vector)));
}

void ViEigenDouble::solve(const ViEigenBaseVector *coefficients, const ViEigenBaseMatrix *values, double *output, const int &outputSize, const int &offset)
{
	const ViEigenVectorDouble *castCoefficients = cast(coefficients);
	const ViEigenMatrixDouble *castValues = cast(values);
	double result;
	int i, j;

	for(i = 0; i < outputSize; ++i)
	{
		result = 0;
		for(j = 0; j < castValues->cols(); ++j)
		{
			result += castCoefficients->coeffRef(j + offset) * castValues->coeffRef(i, j);
		}
		if(result < -1) result = -1;
		else if(result > 1) result = 1;
		output[i] = result;
	}
}

void ViEigenDouble::solve(const ViEigenBaseVector *coefficients, const ViEigenBaseMatrix *values, double *output, const int &outputSize, const int *offset)
{
	const ViEigenVectorDouble *castCoefficients = cast(coefficients);
	const ViEigenMatrixDouble *castValues = cast(values);
	double result;
	int i, j;

	for(i = 0; i < outputSize; ++i)
	{
		result = 0;
		for(j = 0; j < castValues->cols(); ++j)
		{
			result += castCoefficients->coeffRef(offset[i] + j) * castValues->coeffRef(i, j);
		}
		if(result < -1) result = -1;
		else if(result > 1) result = 1;
		output[i] = result;
	}
}

void ViEigenDouble::clear(ViEigenBaseMatrix *matrix)
{
	delete cast(matrix); // Cast first, otherwise there are memory leaks
	matrix = NULL;
}

void ViEigenDouble::clear(ViEigenBaseVector *vector)
{
	delete cast(vector); // Cast first, otherwise there are memory leaks
	vector = NULL;
}

inline const ViEigenMatrixDouble* ViEigenDouble::cast(const ViEigenBaseMatrix *matrix) const
{
	return (const ViEigenMatrixDouble*) matrix;
}

inline const ViEigenVectorDouble* ViEigenDouble::cast(const ViEigenBaseVector *vector) const
{
	return (const ViEigenVectorDouble*) vector;
}

inline ViEigenMatrixDouble* ViEigenDouble::cast(ViEigenBaseMatrix *matrix) const
{
	return (ViEigenMatrixDouble*) matrix;
}

inline ViEigenVectorDouble* ViEigenDouble::cast(ViEigenBaseVector *vector) const
{
	return (ViEigenVectorDouble*) vector;
}

ViEigenVectorDouble* ViEigenDouble::cast(const EigenVectorDouble &vector) const
{
	return new ViEigenVectorDouble(vector);
}
