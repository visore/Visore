#include <viwarpedburginterpolator.h>
#include <visystemsolver.h>

#include<vilogger.h>

#define DEFAULT_DEGREE 3
#define MAXIMUM_SAMPLES -1

ViWarpedBurgInterpolator::ViWarpedBurgInterpolator()
	: ViDegreeInterpolator(MAXIMUM_SAMPLES, DEFAULT_DEGREE)
{
}

bool ViWarpedBurgInterpolator::interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	//mDegree=4;

	ViVector samples(leftSize, (qreal*) leftSamples, false);
	/*ViVector samples(leftSize);
	for(int i = 0; i < leftSize; ++i)
	{
		samples[i] = leftSamples[i];
	}*/

	qreal warpingFactor = 0;

	// Foward exterpolation
	ViVector coefficients;
	estimateBurg(warpingFactor, samples, coefficients);





	cout<<coefficients.toString().toLatin1().data()<<endl;

	static int count;
	count = coefficients.size();


	qreal value = 0;
	for(int j = 1; j < count; ++j)
	{
		value -= coefficients[j] * leftSamples[count-j-1];
	}
	cout<<"***** "<<value<<"\t"<<endl;


	for(int i = 0; i < outputSize; ++i)
	{
		int x = leftSize + i;
		qreal value = 0;
		for(int j = 1; j < count; ++j)
		{
			value -= coefficients[j] * leftSamples[x - 1 - j];
		}
		outputSamples[i] = value;
	}

	return true;
}

void ViWarpedBurgInterpolator::estimateBurg(const qreal &warpingFactor, const ViVector &samples, ViVector &coefficients)
{
	// http://emptyloops.com/technotes/A%20tutorial%20on%20Burg%27s%20method,%20algorithm%20and%20recursion.pdf

	// GET SIZE FROM INPUT VECTORS
	int N = samples.size() - 1;
	int m = mDegree;

	// INITIALIZE Ak
	coefficients.resize( m + 1);
	coefficients.fill(0);
	coefficients[ 0 ] = 1.0;
	// INITIALIZE f and b
	ViVector f(samples);
	ViVector b(samples);

	// INITIALIZE Dk
	double Dk = 0.0;
	for(int j = 0; j <= N; ++j)
	{
		Dk += 2.0 * f[ j ] * f[ j ];
	}
	Dk -= f[ 0 ] * f[ 0 ] + b[ N ] * b[ N ];

	// BURG RECURSION
	for(int k = 0; k < m; ++k)
	{
		// COMPUTE MU
		double mu = 0.0;
		for(int n = 0; n <= N - k - 1; ++n)
		{
			mu += f[ n + k + 1 ] * b[ n ];
		}
		mu *= -2.0 / Dk;

		// UPDATE coefficients
		for(int n = 0; n <= ( k + 1 ) / 2; ++n)
		{
			double t1 = coefficients[ n ] + mu * coefficients[ k + 1 - n ];
			double t2 = coefficients[ k + 1 - n ] + mu * coefficients[ n ];
			coefficients[ n ] = t1;
			coefficients[ k + 1 - n ] = t2;
		}

		// UPDATE f and b
		for ( size_t n = 0; n <= N - k - 1; n++ )
		{
			double t1 = f[ n + k + 1 ] + mu * b[ n ];
			double t2 = b[ n ] + mu * f[ n + k + 1 ];
			f[ n + k + 1 ] = t1;
			b[ n ] = t2;
		}

		// UPDATE Dk
		Dk = ( 1.0 - mu * mu ) * Dk - f[ k + 1 ] * f[ k + 1 ] - b[ N - k - 1 ] * b[ N - k - 1 ];
	}
}

ViWarpedBurgInterpolator* ViWarpedBurgInterpolator::clone()
{
	return new ViWarpedBurgInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViWarpedBurgInterpolator* create()
{
	return new ViWarpedBurgInterpolator();
}

#ifdef __cplusplus
}
#endif
