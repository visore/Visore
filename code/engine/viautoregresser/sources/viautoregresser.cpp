#include <viautoregresser.h>
#include <vimatrix.h>
#include <vilogger.h>

ViAutoRegresser::ViAutoRegresser()
{
	mModelOrder = NULL;
}

ViAutoRegresser::ViAutoRegresser(const ViAutoRegresser &other)
{
	if(other.mModelOrder != NULL)
	{
		mModelOrder = other.mModelOrder->clone();
	}
}

ViAutoRegresser::~ViAutoRegresser()
{
	clear();
}

void ViAutoRegresser::clear()
{
	if(mModelOrder != NULL)
	{
		delete mModelOrder;
		mModelOrder = NULL;
	}
	mCoefficients.clear();
}

void ViAutoRegresser::setModelOrder(ViModelOrder *modelOrder)
{
	clear();
	mModelOrder = modelOrder;
}

ViModelOrder* ViAutoRegresser::modelOrder()
{
	return mModelOrder;
}

ViVector& ViAutoRegresser::coefficients()
{
	return mCoefficients;
}

const ViVector& ViAutoRegresser::coefficients() const
{
	return mCoefficients;
}

bool ViAutoRegresser::calculate(const ViSampleChunk &samples)
{
	int sampleCount = samples.size();
	mModelOrder->setSampleCount(sampleCount);

	if(mModelOrder == NULL) return false;
	int order = mModelOrder->order();

	// Solve for the best autoregression coefficients using a least-squares fit
	/*ViMatrix matrix(order, order);
	ViVector vector(order);

	int row, column, end = sampleCount - order;
	for(int i = 0; i < end; ++i)
	{
		for(row = 0; row < order; ++row)
		{
			for(column = 0; column < order; ++column)
			{
				matrix[row][column] += (samples[i + row] * samples[i + column]);
			}
			vector[row] += (samples[i + order] * samples[i + row]);
		}
	}*/

	// Calculate the mean
	/*qreal mean = 0;
	for(int i = 0; i < sampleCount; ++i)
	{
		mean += samples[i];
	}
	mean /= sampleCount;

	// Apply (subtract) the mean to the samples
	ViSampleChunk newSamples(sampleCount);
	for(int i = 0; i < sampleCount; ++i)
	{
		newSamples[i] = samples[i] - mean;
	}

	int row, column, end = sampleCount - 1;
	for(int i = order - 1; i < end; ++i)
	{
		for(row = 0; row < order; ++row)
		{
			for(column = row; column < order; ++column)
			{
				matrix[row][column] += (newSamples[i - row] * newSamples[i - column]);
				cout<<" *: "<< newSamples[i - row]<<" "<< newSamples[i - column]<<" "<< newSamples[i - row] * newSamples[i - column]<<endl;
			}
			vector[row] += (newSamples[i + order] * newSamples[i + row]);
		}
	}*/

	ViMatrix matrix(sampleCount, order+1);
	ViVector vector(sampleCount);

	for(int i = 0; i < sampleCount; ++i)
	{
		vector[i] = i;
		matrix[i][0] = 1;
		for(int j = 1; j <= order; ++j)
		{
			matrix[i][j] = qPow(samples[i], j);
		}
	}

	ViMatrix transpose = matrix.transpose();

	ViVector B = transpose * vector;
	ViMatrix A = transpose.scalarMultiply(matrix);

	ViVector C = A.invert() * vector;


	ViMatrix x(2,2);
	x[0][0] = 1;
	x[0][1] = 2;
	x[1][0] = 3;
	x[1][1] = 4;

	ViMatrix x2(2,2);
	x2[0][0] = 5;
	x2[0][1] = 6;
	x2[1][0] = 7;
	x2[1][1] = 8;

	ViVector v(2);
	v[0] = 1;
	v[1] = 2;

	cout<<"****************************************"<<sampleCount<<endl;
	cout<<matrix.transpose().toString().toLatin1().data()<<endl;
	cout<<"****************************************"<<endl;

	cout<<"****************************************"<<sampleCount<<endl;
	cout<<matrix.transpose().scalarMultiply(matrix).toString().toLatin1().data()<<endl;
	cout<<"****************************************"<<endl;

	cout<<"****************************************"<<sampleCount<<endl;
	cout<<matrix.transpose().scalarMultiply(matrix).invert().toString().toLatin1().data()<<endl;
	cout<<"****************************************"<<endl;

	cout<<"****************************************"<<sampleCount<<endl;
	cout<<matrix.transpose().scalarMultiply(matrix).invert().scalarMultiply(matrix.transpose()).toString().toLatin1().data()<<endl;
	cout<<"****************************************"<<endl;

	cout<<"****************************************"<<sampleCount<<endl;
	cout<<(matrix.transpose().scalarMultiply(matrix).invert().scalarMultiply(matrix.transpose()) * vector).toString().toLatin1().data()<<endl;
	cout<<"****************************************"<<endl;

	/*ViMatrix inverted;
	if(!matrix.invert(inverted))
	{
		// The matrix is singular
		return false;
	}

	mCoefficients = inverted * vector;*/
	return true;
}
