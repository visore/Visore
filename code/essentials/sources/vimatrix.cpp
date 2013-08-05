#include <vimatrix.h>
#include <qmath.h>

ViMatrix::ViMatrix()
{
	mRows = 0;
	mColumns = 0;
	mVectors = NULL;
}

ViMatrix::ViMatrix(const int &rows, const int &colums, const double **data)
{
	mRows = rows;
	mColumns = colums;
	mVectors = new ViVector*[mRows];
	int row, column;
	for(row = 0; row < mRows; ++row)
	{
		mVectors[row] = new ViVector(mColumns);
		for(column = 0; column < mColumns; ++column)
		{
			if (data) (*this)[row][column] = data[row][column];
			else (*this)[row][column] = 0;
		}
	}
}

ViMatrix::ViMatrix(const ViMatrix &other)
{
	mVectors = NULL;
	copy(other);
}

ViMatrix::~ViMatrix()
{
	clear();
}

void ViMatrix::clear()
{
	if(mVectors != NULL)
	{
		for(int i = 0; i < mRows; ++i)
		{
			delete mVectors[i];
		}
		delete [] mVectors;
		mVectors = NULL;
	}
	mRows = 0;
	mColumns = 0;
}

bool ViMatrix::isValid() const
{
	return mRows > 0 && mColumns > 0;
}

int ViMatrix::rows() const
{
	return mRows;
}

int ViMatrix::columns() const
{
	return mColumns;
}

double ViMatrix::sum() const
{
	int row, column;
	double sum = 0;
	for(row = 0; row < mRows; ++row)
	{
		for(column = 0; column < mColumns; ++column)
		{
			sum += mVectors[row]->at(column);
		}
	}
	return sum;
}

ViMatrix ViMatrix::invert()
{
	ViMatrix inverted;
	invert(inverted);
	return inverted;
}

ViMatrix ViMatrix::invert(bool &success)
{
	ViMatrix inverted;
	success = invert(inverted);
	return inverted;
}

bool ViMatrix::invert(ViMatrix &inverted)
{
	// Very straightforward implementation of Gauss-Jordan elimination to invert a matrix.

	if(mRows != mColumns) return false;

	int i, j, k, argumentMaximum;
	double absoluteMaximum, factor;
	int rows = mRows; // Need this when calling invert with itself

	ViMatrix temp = *this;
	inverted = ViMatrix::identityMatrix(rows);

	// Do the elimination one column at a time
	for(i = 0; i < rows; ++i)
	{
		// Pivot the row with the largest absolute value in column i, into row i
		absoluteMaximum = 0;
		argumentMaximum = 0;

		for(j = i; j < rows; ++j)
		{
			if(qAbs(temp[j][i]) > absoluteMaximum)
			{
				absoluteMaximum = qAbs(temp[j][i]);
				argumentMaximum = j;
			}
		}

		// If no row has a nonzero value in that column, the matrix is singular and we have to give up.
		if(absoluteMaximum == 0) return false;

		if(i != argumentMaximum)
		{
			temp.swapRows(i, argumentMaximum);
			inverted.swapRows(i, argumentMaximum);
		}

		// Divide this row by the value of M[i][i]
		factor = 1.0 / temp[i][i];
		temp[i] *= factor;
		inverted[i] *= factor;

		// Eliminate the rest of the column
		for(j = 0; j < rows; ++j)
		{
			if(j == i && qAbs(temp[j][i]) > 0)
			{
				// Subtract a multiple of row i from row j
				factor = temp[j][i];
				for(k = 0; k < rows; ++k)
				{
					temp[j][k] -= (temp[i][k] * factor);
					inverted[j][k] -= (inverted[i][k] * factor);
				}
			}
		}
	}
	return true;
}

ViMatrix ViMatrix::transpose() const
{
	ViMatrix matrix(mColumns, mRows);
	int row, column;
	for(row = 0; row < mRows; ++row)
	{
		for(column = 0; column < mColumns; ++column)
		{
			matrix[column][row] = mVectors[row]->at(column);
		}
	}
	return matrix;
}

void ViMatrix::swapRows(const int &first, const int &second)
{
	ViVector *temp = mVectors[first];
	mVectors[first] = mVectors[second];
	mVectors[second] = temp;
}

ViMatrix ViMatrix::matrixMultiply(const ViMatrix &matrix) const
{
	int rows = qMin(mRows, matrix.rows());
	int columns = qMin(mColumns, matrix.columns());
	int row, column;
	ViMatrix newMatrix(rows, columns);
	for(row = 0; row < rows; ++row)
	{
		for(column = 0; column < columns; ++column)
		{
			newMatrix[row][column] = mVectors[row]->at(column) * matrix[row][column];
		}
	}
	return newMatrix;
}

ViMatrix ViMatrix::scalarMultiply(const ViMatrix &matrix) const
{
	if(mColumns != matrix.rows()) return ViMatrix();
	int row, column, i;
	ViMatrix newMatrix(mRows, matrix.columns());
	for(row = 0; row < mRows; ++row)
	{
		for(column = 0; column < matrix.columns(); ++column)
		{
			newMatrix[row][column] = 0;
			for(i = 0; i < mColumns; ++i)
			{
				newMatrix[row][column] += mVectors[row]->at(i) * matrix[i][column];
			}
		}
	}
	return newMatrix;
}

ViMatrix ViMatrix::subset(const int &startRow, const int &rowCount, const int &startColumn, const int &columnCount) const
{
	ViMatrix matrix(rowCount, columnCount);
	int row, column;
	for(row = 0; row < rowCount; ++row)
	{
		for(column = 0; column < columnCount; ++column)
		{
			matrix[row][column] = mVectors[startRow + row]->at(startColumn + column);
		}
	}
	return matrix;
}

ViMatrix ViMatrix::concatenateColums(const ViMatrix &matrix) const
{
	if(mRows, matrix.rows()) return ViMatrix();
	ViMatrix newMatrix(mRows, mColumns + matrix.columns());
	int row, column;
	for(row = 0; row < mRows; ++row)
	{
		for(column = 0; column < mColumns; ++column)
		{
			newMatrix[row][column] = mVectors[row]->at(column);
		}
		for(column = 0; column < matrix.columns(); ++column)
		{
			newMatrix[row][column + mColumns] = matrix[row][column];
		}
	}
	return newMatrix;
}

ViMatrix ViMatrix::identityMatrix(const int &size)
{
	ViMatrix matrix(size, size);
	for(int i = 0; i < size; ++i)
	{
		matrix[i][i] = 1;
	}
	return matrix;
}

ViMatrix& ViMatrix::operator = (const ViMatrix& other)
{
	copy(other);
	return *this;
}

ViMatrix ViMatrix::operator + (const ViMatrix &matrix) const
{
	int row, column;
	int rows = qMin(mRows, matrix.rows());
	int columns = qMin(mColumns, matrix.columns());
	ViMatrix newMatrix(rows, columns);
	for(row = 0; row < rows; ++row)
	{
		for(column = 0; column < columns; ++column)
		{
			newMatrix[row][column] = mVectors[row]->at(column) + matrix[row][column];
		}
	}
	return newMatrix;
}

ViMatrix ViMatrix::operator * (const double &value) const
{
	ViMatrix matrix(mRows, mColumns);
	int row, column;
	for(row = 0; row < mRows; ++row)
	{
		for(column = 0; column < mColumns; ++column)
		{
			matrix[row][column] = mVectors[row]->at(column) * value;
		}
	}
	return matrix;
}

ViVector ViMatrix::operator * (const ViVector &vector) const
{
	int row, column, size = qMin(mColumns, vector.size());
	ViVector newVector(mRows);
	for(row = 0; row < mRows; ++row)
	{
		newVector[row] = 0;
		for(column = 0; column < size; ++column)
		{
			newVector[row] += mVectors[row]->at(column) * vector[column];
		}
	}
	return newVector;
}

ViVector& ViMatrix::operator [] (const int &index)
{
	return *mVectors[index];
}

ViVector& ViMatrix::operator [] (const int &index) const
{
	return *mVectors[index];
}

void ViMatrix::copy(const ViMatrix& other)
{
	clear();
	mRows = other.mRows;
	mColumns = other.mColumns;
	if(mRows > 0 && mColumns > 0)
	{
		mVectors = new ViVector*[mRows];
		for(int row = 0; row < mRows; ++row)
		{
			mVectors[row] = new ViVector(mColumns);
			*mVectors[row] = *other.mVectors[row];
		}
	}
}
