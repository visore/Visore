#include <vivector.h>
#include <vimatrix.h>
#include <QtMath>
#include <QtNumeric>

ViVector::ViVector()
{
	mCopy = false;
	mSize = 0;
	mData = NULL;
}

ViVector::ViVector(const int &size, double *data, const bool &copy)
{
	mSize = size;
	mCopy = copy;
	if(mCopy || !data)
	{
		mCopy = true;
		mData = new double[mSize];
		if(data) for(int i = 0; i < mSize; ++i) mData[i] = data[i];
		else for(int i = 0; i < mSize; ++i) mData[i] = 0.0;
	}
	else
	{
		mCopy = false;
		mData = data;
	}
}

ViVector::ViVector(const int &size, const double *data)
{
	mCopy = true;
	mSize = size;
	mData = new double[mSize];
	for(int i = 0; i < mSize; ++i) mData[i] = data[i];
}

ViVector::ViVector(const int &size1, const double *data1, const int &size2, const double *data2)
{
	mCopy = true;
	mSize = size1 + size2;
	mData = new double[mSize];
	int i;
	for(i = 0; i < size1; ++i) mData[i] = data1[i];
	for(i = 0; i < size2; ++i) mData[i + size1] = data2[i];
}

ViVector::ViVector(const ViVector &other)
{
	mData = NULL;
	copy(other);
}

ViVector::~ViVector()
{
	clear();
}

void ViVector::clear()
{
	if(mCopy && mData != NULL)
	{
		delete [] mData;
		mData = NULL;
	}
	mSize = 0;
	mCopy = false;
}

void ViVector::resize(const int &size)
{
	clear();
	mSize = size;
	mData = new double[mSize];
}

void ViVector::fill(const double &value)
{
	for(int i = 0; i < mSize; ++i)
	{
		mData[i] = value;
	}
}

bool ViVector::isValid() const
{
	if(mSize == 0) return false;
	for(int i = 0; i < mSize; ++i)
	{
		if(qIsInf(mData[i]) || qIsNaN(mData[i])) return false;
	}
	return true;
}

int ViVector::size() const
{
	return mSize;
}

double ViVector::sum() const
{
	double sum = 0;
	for(int i = 0; i < mSize; ++i)
	{
		sum += mData[i];
	}
	return sum;
}

ViVector ViVector::concatenate(const ViVector &left, const ViVector &right)
{
	int leftSize = left.size();
	int rightSize = right.size();
	ViVector vector(leftSize + rightSize);
	int i;
	for(i = 0; i < leftSize; ++i)
	{
		vector[i] = left[i];
	}
	for(i = 0; i < rightSize; ++i)
	{
		vector[i + leftSize] = right[i];
	}
	return vector;
}


ViVector& ViVector::concatenate(const ViVector &other)
{
	*this = ViVector::concatenate(*this, other);
	return *this;
}

ViVector ViVector::subset(const int &start, const int &size) const
{
	ViVector vector(size);
	for(int i = 0; i < size; i++)
	{
		vector[i] = mData[start + i];
	}
	return vector;
}

ViMatrix ViVector::transpose()
{
	ViMatrix matrix(1, mSize);
	for(int i = 0; i < mSize; ++i)
	{
		matrix[0][i] = mData[i];
	}
	return matrix;
}

double& ViVector::at(const int &index)
{
	return mData[index];
}

double ViVector::at(const int &index) const
{
	return mData[index];
}

double& ViVector::operator [] (const int &index)
{
	return mData[index];
}

double ViVector::operator [] (const int &index) const
{
	return mData[index];
}

ViVector& ViVector::operator = (const ViVector &other)
{
	copy(other);
	return *this;
}

ViVector& ViVector::operator += (const ViVector &other)
{
	int size = qMin(mSize, other.mSize);
	for(int i = 0; i < size; ++i)
	{
		mData[i] += other[i];
	}
	return *this;
}

ViVector& ViVector::operator -= (const ViVector &other)
{
	int size = qMin(mSize, other.mSize);
	for(int i = 0; i < size; ++i)
	{
		mData[i] -= other[i];
	}
	return *this;
}

ViVector& ViVector::operator *= (const ViVector &other)
{
	int size = qMin(mSize, other.mSize);
	for(int i = 0; i < size; ++i)
	{
		mData[i] *= other[i];
	}
	return *this;
}

ViVector& ViVector::operator *= (const double &value)
{
	for(int i = 0; i < mSize; ++i)
	{
		mData[i] *= value;
	}
	return *this;
}

ViVector& ViVector::operator *= (const ViMatrix &matrix)
{
	ViVector vector = *this * matrix;
	*this = vector;
	return *this;
}

ViVector ViVector::operator + (const ViVector &other) const
{
	int size = qMin(mSize, other.mSize);
	ViVector vector(size);
	for(int i = 0; i < size; ++i)
	{
		vector[i] = mData[i] + other[i];
	}
	return vector;
}

ViVector ViVector::operator - (const ViVector &other) const
{
	int size = qMin(mSize, other.mSize);
	ViVector vector(size);
	for(int i = 0; i < size; ++i)
	{
		vector[i] = mData[i] - other[i];
	}
	return vector;
}

ViVector ViVector::operator * (const ViVector &other) const
{
	int size = qMin(mSize, other.mSize);
	ViVector vector(size);
	for(int i = 0; i < size; ++i)
	{
		vector[i] = mData[i] * other[i];
	}
	return vector;
}

ViVector ViVector::operator * (const double &value) const
{
	ViVector vector(mSize);
	for(int i = 0; i < mSize; ++i)
	{
		vector[i] = mData[i] * value;
	}
	return vector;
}

ViVector ViVector::operator * (const ViMatrix &matrix) const
{
	int j, size = qMin(mSize, matrix.rows());
	ViVector vector(matrix.columns());
	for(int i = 0; i < matrix.columns(); ++i)
	{
		vector[i] = 0;
		for( j= 0; j < size; ++j)
		{
			vector[i] += mData[j] * matrix[j][i];
		}
	}
	return vector;
}

void ViVector::copy(const ViVector &other)
{
	clear();
	mSize = other.mSize;
	mCopy = true;
	mData = new double[mSize];
	for(int i = 0; i < mSize; ++i)
	{
		mData[i] = other.mData[i];
	}
}

QString ViVector::toString(const int &decimalPlaces)
{
	QString result = "";
	for(int i = 0; i < mSize; ++i)
	{
		result += QString::number(mData[i], 'f', decimalPlaces) + " ";
	}
	return result.trimmed();
}
