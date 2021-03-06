#include <vieigenfloat.h>

ViEigenMatrixFloat::ViEigenMatrixFloat()
	: EigenMatrixFloat()
{
}

ViEigenMatrixFloat::ViEigenMatrixFloat(const int &size)
	: EigenMatrixFloat(size, size)
{
	int i, j;
	for(i = 0; i < size; ++i)
	{
		for(j = 0; j < size; ++j) this->coeffRef(i, j) = 0;
	}
}

ViEigenMatrixFloat::ViEigenMatrixFloat(const int &rows, const int &columns)
	: EigenMatrixFloat(rows, columns)
{
	int i, j;
	for(i = 0; i < rows; ++i)
	{
		for(j = 0; j < columns; ++j) this->coeffRef(i, j) = 0;
	}
}

void ViEigenMatrixFloat::get(double &result, const int &row, const int &column)
{
	result = this->coeffRef(row, column);
}

void ViEigenMatrixFloat::get(double &result, const int &row, const int &column) const
{
	result = this->coeffRef(row, column);
}

void ViEigenMatrixFloat::get(mpfr::mpreal &result, const int &row, const int &column)
{
	result = this->coeffRef(row, column);
}

void ViEigenMatrixFloat::get(mpfr::mpreal &result, const int &row, const int &column) const
{
	result = this->coeffRef(row, column);
}

void ViEigenMatrixFloat::set(const int &row, const int &column, const double &value)
{
	this->coeffRef(row, column) = value;
}

void ViEigenMatrixFloat::set(const int &row, const int &column, const mpfr::mpreal &value)
{
	this->coeffRef(row, column) = value.toDouble();
}

void ViEigenMatrixFloat::setPower(const int &row, const int &column, const double &base, const int &exponent)
{
	set(row, column, std::pow(base, exponent));
}

void ViEigenMatrixFloat::setPowerMulti(const int &row, const int &column, const double &multi, const double &base, const int &exponent)
{
	set(row, column, multi * std::pow(base, exponent));
}

void ViEigenMatrixFloat::setSin(const int &row, const int &column, const double &value)
{
	set(row, column, std::sin(value));
}

void ViEigenMatrixFloat::setCos(const int &row, const int &column, const double &value)
{
	set(row, column, std::cos(value));
}

void ViEigenMatrixFloat::setSinMulti(const int &row, const int &column, const double &multi, const double &value)
{
	set(row, column, multi * std::sin(value));
}

void ViEigenMatrixFloat::setCosMulti(const int &row, const int &column, const double &multi, const double &value)
{
	set(row, column, multi * std::cos(value));
}

QString ViEigenMatrixFloat::toString(const int &precision) const
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

ViEigenVectorFloat::ViEigenVectorFloat()
	: EigenVectorFloat()
{
}

ViEigenVectorFloat::ViEigenVectorFloat(const int &rows)
	: EigenVectorFloat(rows)
{
	for(int i = 0; i < rows; ++i) this->coeffRef(i) = 0;
}

ViEigenVectorFloat::ViEigenVectorFloat(const EigenVectorFloat &other)
	: EigenVectorFloat(other)
{
}

void ViEigenVectorFloat::get(double &result, const int &row)
{
	result = this->coeffRef(row);
}

void ViEigenVectorFloat::get(double &result, const int &row) const
{
	result = this->coeffRef(row);
}

void ViEigenVectorFloat::get(mpfr::mpreal &result, const int &row)
{
	result = this->coeffRef(row);
}

void ViEigenVectorFloat::get(mpfr::mpreal &result, const int &row) const
{
	result = this->coeffRef(row);
}

void ViEigenVectorFloat::set(const int &row, const double &value)
{
	this->coeffRef(row) = value;
}

void ViEigenVectorFloat::set(const int &row, const mpfr::mpreal &value)
{
	this->coeffRef(row) = value.toDouble();
}

QString ViEigenVectorFloat::toString(const int &precision) const
{
	QString result = "";
	for(int i = 0; i < this->rows(); ++i)
	{
		result += eigenToString(this->coeff(i), precision) + " ";
	}
	result += "\n";
	return result;
}

void ViEigenFloat::intialize(mpfr::mpreal &value)
{
}

ViEigenBaseMatrix* ViEigenFloat::createMatrix()
{
	return new ViEigenMatrixFloat();
}

ViEigenBaseMatrix* ViEigenFloat::createMatrix(const int &size)
{
	return new ViEigenMatrixFloat(size);
}

ViEigenBaseMatrix* ViEigenFloat::createMatrix(const int &rows, const int &columns)
{
	return new ViEigenMatrixFloat(rows, columns);
}

ViEigenBaseVector* ViEigenFloat::createVector()
{
	return new ViEigenVectorFloat();
}

ViEigenBaseVector* ViEigenFloat::createVector(const int &rows)
{
	return new ViEigenVectorFloat(rows);
}

ViEigenBaseVector* ViEigenFloat::estimate(const ViEigenBaseMatrix *matrix, const ViEigenBaseVector *vector)
{
	return cast(cast(matrix)->fullPivHouseholderQr().solve(*cast(vector)));
}

void ViEigenFloat::solve(const ViEigenBaseVector *coefficients, const ViEigenBaseMatrix *values, double *output, const int &outputSize, const int &offset)
{
	const ViEigenVectorFloat *castCoefficients = cast(coefficients);
	const ViEigenMatrixFloat *castValues = cast(values);
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

void ViEigenFloat::solve(const ViEigenBaseVector *coefficients, const ViEigenBaseMatrix *values, double *output, const int &outputSize, const int *offset)
{
	const ViEigenVectorFloat *castCoefficients = cast(coefficients);
	const ViEigenMatrixFloat *castValues = cast(values);
	double result;
	int i, j;

	for(i = 0; i < outputSize; ++i)
	{
		result = 0;
		for(j = 0; j < castValues->cols(); ++j)
		{
			result += castCoefficients->coeffRef(j + offset[i]) * castValues->coeffRef(i, j);
		}
		if(result < -1) result = -1;
		else if(result > 1) result = 1;
		output[i] = result;
	}
}

void ViEigenFloat::clear(ViEigenBaseMatrix *matrix)
{
	delete cast(matrix); // Cast first, otherwise there are memory leaks
	matrix = NULL;
}

void ViEigenFloat::clear(ViEigenBaseVector *vector)
{
	delete cast(vector); // Cast first, otherwise there are memory leaks
	vector = NULL;
}

inline const ViEigenMatrixFloat* ViEigenFloat::cast(const ViEigenBaseMatrix *matrix) const
{
	return (const ViEigenMatrixFloat*) matrix;
}

inline const ViEigenVectorFloat* ViEigenFloat::cast(const ViEigenBaseVector *vector) const
{
	return (const ViEigenVectorFloat*) vector;
}

inline ViEigenMatrixFloat* ViEigenFloat::cast(ViEigenBaseMatrix *matrix) const
{
	return (ViEigenMatrixFloat*) matrix;
}

inline ViEigenVectorFloat* ViEigenFloat::cast(ViEigenBaseVector *vector) const
{
	return (ViEigenVectorFloat*) vector;
}

inline ViEigenVectorFloat* ViEigenFloat::cast(const EigenVectorFloat &vector) const
{
	return new ViEigenVectorFloat(vector);
}
