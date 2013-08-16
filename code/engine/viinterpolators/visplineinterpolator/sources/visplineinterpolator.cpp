#include <visplineinterpolator.h>
#include <vimodelsolver.h>

#include<vilogger.h>

#define DEFAULT_ORDER 3

ViSplineInterpolator::ViSplineInterpolator()
	: ViInterpolator()
{
	mOrder = DEFAULT_ORDER;
	mLeftSize = 0;
	mRightSize = 0;
	mOutputSize = 0;
	mLeftSamples = NULL;
	mRightSamples = NULL;
}

void ViSplineInterpolator::setOrder(const int &order)
{
	mOrder = order;
}

int ViSplineInterpolator::order()
{
	return mOrder;
}

bool ViSplineInterpolator::interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	if(mOrder < 1) return false;
	mLeftSize = leftSize;
	mRightSize = rightSize;
	mOutputSize = outputSize;
	mLeftSamples = (qreal*) leftSamples;
	mRightSamples = (qreal*) rightSamples;

	int end = leftSize + rightSize;
	int indexes = end - 1;
	int parameterCount = mOrder + 1;
	int size = parameterCount * indexes;
	int i, j, index, x1, x2, end2;

	ViVector vector(size);
	ViMatrix matrix(size, size);

	vector[0] = yValue(0);
	for(i = 1; i < indexes; ++i)
	{
		index = i * 2;
		vector[index - 1] = yValue(i);
		vector[index] = yValue(i);
	}
	vector[(indexes * 2) - 1] = yValue(indexes);
	// The rest of the vector is 0

	// Add the functions for each spline between two points
	for(i = 0; i < indexes; ++i)
	{
		index = i * 2;
		ViVector &row1 = matrix[index];
		ViVector &row2 = matrix[index + 1];
		x1 = xValue(i);
		x2 = xValue(i + 1);

		for(j = 0; j < parameterCount; ++j)
		{
			index = (i * parameterCount) + j;
			row1[index] = qPow(x1, mOrder - j);
			row2[index] = qPow(x2, mOrder - j);
		}
	}

	// Add the slope (first derivatives) of all points with a spline at each side
	/*end = indexes - 1; // (n - 2) where n is the number of data points
	int advance, advance2, row, front, exponent;
	for(i = 0; i < end; ++i)
	{
		row = (indexes * 2) + i;
		advance = i * parameterCount;
		advance2 = advance + parameterCount;
		x1 = xValue(i + 1);
		for(j = 0; j < parameterCount; ++j)
		{
			front = mOrder - j;
			exponent = front - 1;
			matrix[row][advance + j] = front * qPow(x1, exponent);
			matrix[row][advance2 + j] = -front * qPow(x1, exponent);
		}
	}*/

	int advance, advance2, row, front, exponent;

	for(int derivative = 0; derivative < mOrder - 1; ++derivative)
	{
		end = indexes - 1; // (n - 2) where n is the number of data points
		for(i = 0; i < end; ++i)
		{
			row = (indexes * 2) + i + (derivative * end);
			advance = i * parameterCount;
			advance2 = advance + parameterCount;
			x1 = xValue(i + 1);
			end2 = parameterCount - 1 - derivative;
			for(j = 0; j < end2; ++j)
			{
				front = calculateMultiplier(derivative + 1, j);
				exponent = mOrder - derivative - j - 1;
				matrix[row][advance + j] = front * qPow(x1, exponent);
				matrix[row][advance2 + j] = -front * qPow(x1, exponent);
			}
		}
	}

	// Add the second derivatives. Second derivatives should vanish at the end points (aka towards 0)
	/*end = mOrder - 1;
	for(i = 0; i < end; ++i)
	{
		row = size + i - 1;
		advance = i * parameterCount;
		x1 = xValue(i + 1);
		end2 = parameterCount - 2;
		for(j = 0; j < end2; ++j)
		{
			front = mOrder - j;
			exponent = front - 2;
			matrix[row][advance + j] = front * qPow(x1, exponent);
		}
	}*/

	end = mOrder - 1;
	for(i = 0; i < end-1; ++i)
	{
		row = size - end + i;

		cout<<row<<endl;

		advance = i * parameterCount;
		advance2 = size - parameterCount;
		x1 = xValue(0);
		x2 = xValue(indexes);
		end2 = parameterCount - 2;
		for(j = 0; j < end2; ++j)
		{
			front = calculateMultiplier(2, j);
			exponent = mOrder - 2 - j;
			cout<<x1<<" "<<front<<" "<<exponent<<endl;
			matrix[row][advance + j] = front * qPow(x1, exponent);
			matrix[row+1][advance2 + j] = front * qPow(x2, exponent);
		}
		cout<<"-************************-"<<endl;
	}

	ViModelSolver solver;
	ViVector parameters(size);
	if(!solver.estimate(mOrder, matrix, vector, parameters))
	{
		return false;
	}

	cout<<parameters.toString().toLatin1().data()<<endl;
	cout<<"-------------------------------"<<endl;
	cout<<vector.toString().toLatin1().data()<<endl;
	cout<<"-------------------------------"<<endl;
	cout<<matrix.toString().toLatin1().data()<<endl;


	qreal value;
	index = (leftSize - 1) * parameterCount;
	for(i = 0; i < outputSize; ++i)
	{
		x1 = leftSize + i;
		value = 0;
		for(j = 0; j < parameterCount; ++j)
		{
			value += parameters[index + j] * qPow(x1, mOrder - j);
		}
		outputSamples[i] = value;
	}

	return true;
}

int ViSplineInterpolator::xValue(const int &number)
{
	if(number < mLeftSize)
	{
		return number;
	}
	return number + mOutputSize;
	/*if(number==0)return 1;
	if(number==1)return 2;
	if(number==2)return 3;*/
}

qreal ViSplineInterpolator::yValue(const int &number)
{
	if(number < mLeftSize)
	{
		return mLeftSamples[number];
	}
	return mRightSamples[number - mRightSize - 1];
	/*if(number==0)return 1;
	if(number==1)return 5;
	if(number==2)return 4;*/
}

qreal ViSplineInterpolator::calculateMultiplier(const int &derivative, const int &parameterNumber)
{
	qreal result = 1;
	int multiply = mOrder - parameterNumber;
	for(int i = 0; i < derivative; ++i)
	{
		result *= multiply - i;
	}
	return result;
}

ViSplineInterpolator* ViSplineInterpolator::clone()
{
	return new ViSplineInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViSplineInterpolator* create()
{
	return new ViSplineInterpolator();
}

#ifdef __cplusplus
}
#endif
