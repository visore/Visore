#include <viarmanoisedetector.h>

#define TWO_PI 6.2831853071795864769252866

#define DEFAULT_MA_DEGREE 6
#define DEFAULT_AR_DEGREE 15
#define WINDOW_SIZE 64
#define AMPLIFIER 3

ViArmaNoiseDetector::ViArmaNoiseDetector(const Type &type)
	: ViNoiseDetector()
{
	srand((unsigned)time(NULL));

	mWindowData = NULL;
	mMaMatrix = NULL;
	mArMatrix = NULL;
	mMaCoefficients = NULL;
	mArCoefficients = NULL;

	if(type == MA || type == ARMA) setDegree(MA, DEFAULT_MA_DEGREE);
	else setDegree(MA, 0);
	if(type == AR || type == ARMA) setDegree(AR, DEFAULT_AR_DEGREE);
	else setDegree(AR, 0);

	setType(type);
	setWindowSize(WINDOW_SIZE);
	setOffset(mWindowSize);
}

ViArmaNoiseDetector::ViArmaNoiseDetector(const ViArmaNoiseDetector &other)
	: ViNoiseDetector(other)
{
	srand((unsigned)time(NULL));

	mWindowData = NULL;
	mMaMatrix = NULL;
	mArMatrix = NULL;
	mMaCoefficients = NULL;
	mArCoefficients = NULL;

	if(other.mType == MA || other.mType == ARMA) setDegree(MA, other.mMaDegree);
	else setDegree(MA, 0);
	if(other.mType == AR || other.mType == ARMA) setDegree(AR, other.mArDegree);
	else setDegree(AR, 0);

	setType(other.mType);
	setWindowSize(other.mWindowSize);
	setOffset(mWindowSize);
}

ViArmaNoiseDetector::~ViArmaNoiseDetector()
{
    if(mWindowData != NULL)
    {
        delete [] mWindowData;
        mWindowData = NULL;
    }
	clear(ARMA);
}

QString ViArmaNoiseDetector::name(QString replace, bool spaced)
{
	QString n = ViNoiseDetector::name(replace, spaced);
	if(spaced) n += " ";
	n += QString::number(mMaDegree);
	if(spaced) n += " ";
	else n += "_";
	n += QString::number(mArDegree);
	return n;
}

void ViArmaNoiseDetector::setParameters(qreal param1, qreal param2)
{
	setDegree(param1, param2);
}

void ViArmaNoiseDetector::setParameters(qreal param1, qreal param2, qreal param3)
{
	setWindowSize(param3);
	setDegree(param1, param2);
}

void ViArmaNoiseDetector::setType(const Type &type)
{
	mType = type;
}

void ViArmaNoiseDetector::setWindowSize(int size)
{
    mWindowSize = size;
    if(mWindowData != NULL)
    {
        delete [] mWindowData;
    }
	mWindowData = new qreal[mWindowSize];
}

void ViArmaNoiseDetector::setDegree(const Type &type, const int &degree)
{
	if(type == MA || type == ARMA)
	{
		clear(MA);
		mMaDegree = degree;

		if(mMaDegree > 0)
		{
			mMaMatrix = new qreal*[mMaDegree];
			mMaCoefficients = new qreal[mMaDegree];
			for(int i = 0; i < mMaDegree; ++i)
			{
				mMaMatrix[i] = new qreal[mMaDegree];
			}
		}
	}
	if(type == AR || type == ARMA)
	{
		clear(AR);
		mArDegree = degree;

		if(mArDegree > 0)
		{
			mArMatrix = new qreal*[mArDegree];
			mArCoefficients = new qreal[mArDegree];
			for(int i = 0; i < mArDegree; ++i)
			{
				mArMatrix[i] = new qreal[mArDegree];
			}
		}
	}
}

void ViArmaNoiseDetector::setDegree(const int &maDegree, const int &arDegree)
{
	setDegree(MA, maDegree);
	setDegree(AR, arDegree);
}

qreal ViArmaNoiseDetector::generateNoise(const qreal &variance) const
{
	static qreal rand1, rand2;
	rand1 = rand() / ((double) RAND_MAX);
	if(rand1 < 1e-100) rand1 = 1e-100;
	rand2 = rand() / ((double) RAND_MAX);
	return qSqrt(-2 * variance * qLn(rand1)) * qCos(TWO_PI * rand2);
}

void ViArmaNoiseDetector::clear(const Type &type)
{
	if(type == MA || type == ARMA)
	{
		if(mMaCoefficients != NULL)
		{
			delete [] mMaCoefficients;
			mMaCoefficients = NULL;
		}
		if(mMaMatrix != NULL)
		{
			for(int i = 0; i < mMaDegree; ++i)
			{
				delete [] mMaMatrix[i];
			}
			delete [] mMaMatrix;
			mMaMatrix = NULL;
		}
	}

	if(type == AR || type == ARMA)
	{
		if(mArCoefficients != NULL)
		{
			delete [] mArCoefficients;
			mArCoefficients = NULL;
		}
		if(mArMatrix != NULL)
		{
			for(int i = 0; i < mArDegree; ++i)
			{
				delete [] mArMatrix[i];
			}
			delete [] mArMatrix;
			mArMatrix = NULL;
		}
	}
}

void ViArmaNoiseDetector::calculateNoise(QQueue<qreal> &samples)
{
	static int i;
	static qreal mean, variance, prediction;

	while(samples.size() > mWindowSize)
	{
		prediction = 0;

		if(mType == MA || mType == ARMA)
		{
			mean = 0;
			for(i = 0; i < mWindowSize; ++i)
			{
				mean += samples[i];
			}
			mean /= mWindowSize;

			variance = 0;
			for(i = 0; i < mWindowSize; ++i)
			{
				variance += qPow(samples[i] - mean, 2);
			}
			variance /= mWindowSize;

			for(i = 0; i < mWindowSize; ++i)
			{
				mWindowData[i] = generateNoise(variance);
			}

			if(leastSquareFit(mWindowData, mMaDegree, mMaCoefficients, mMaMatrix))
			{
				//prediction += generateNoise(variance);
				for(i = 0; i < mMaDegree; ++i)
				{
					prediction += mMaCoefficients[i] * mWindowData[mWindowSize - 1 - i];
				}
			}
		}

		if(mType == AR || mType == ARMA)
		{
			for(i = 0; i < mWindowSize; ++i)
			{
				mWindowData[i] = samples[i];
			}

			/*qreal mina = 10000;
			int minj = 2;
			for(int j = 10; j < 30; ++j)
			{
				setDegree(AR, j);

				prediction = 0;
				if(leastSquareFit(mWindowData, mArDegree, mArCoefficients, mArMatrix))
				{
					for(i = 0; i < mArDegree; ++i)
					{
						prediction += (mArCoefficients[i] * mWindowData[mWindowSize - 1 - i]);
					}
					qreal r = rss(samples[mWindowSize], prediction);
					//qreal a = aicc(r, mWindowSize, mArDegree);
					if(r < mina)
					{
						mina = r;
						minj = j;
					}
				}
			}
			//cout<<"\t\t"<<minj<<endl;
			setDegree(AR, minj);*/

			prediction = 0;
			if(leastSquareFit(mWindowData, mArDegree, mArCoefficients, mArMatrix))
			{
				for(i = 0; i < mArDegree; ++i)
				{
					prediction += (mArCoefficients[i] * mWindowData[mWindowSize - 1 - i]);
				}
			}

		}

		setNoise(qAbs(samples[mWindowSize] - prediction)/* / AMPLIFIER*/);
		//setNoise(prediction);
		samples.removeFirst();
	}
}

bool ViArmaNoiseDetector::leastSquareFit(const qreal *input, const int &degree, qreal *coefficients)
{
	static int i;

	qreal **matrix = new qreal*[degree];
	for(i = 0; i < degree; ++i)
	{
		matrix[i] = new qreal[degree];
	}

	bool result = leastSquareFit(input, degree, coefficients, matrix);

	for(i = 0; i < degree; ++i)
	{
		delete [] matrix[i];
	}
	delete [] matrix;

	return result;
}

bool ViArmaNoiseDetector::leastSquareFit(const qreal *input, const int &degree, qreal *coefficients, qreal **matrix)
{
	static int i, j, k, hj, hi, end;

	for(i = 0; i < degree; ++i)
	{
		coefficients[i] = 0.0;
		for(j = 0; j < degree; ++j)
		{
			matrix[i][j] = 0.0;
		}
	}

	end = mWindowSize - 1;
	for(i = degree - 1; i < end; ++i)
	{
		hi = i + 1;
		for(j = 0; j < degree; ++j)
		{
			hj = i - j;
			coefficients[j] += (input[hi] * input[hj]);
			for(k = j; k < degree; ++k)
			{
				matrix[j][k] += (input[hj] * input[i - k]);
			}
		}
	}

	for(i = 0; i < degree; ++i)
	{
		coefficients[i] /= (mWindowSize - degree);
		for(j = i; j < degree; ++j)
		{
			matrix[i][j] /= (mWindowSize - degree);
			matrix[j][i] = matrix[i][j];
		}
	}

	return solveEquations(matrix, coefficients, degree);
}

bool ViArmaNoiseDetector::solveEquations(double **matrix, double *coefficients, const int &degree)
{
	static int i, j, k, maxi, end;
	static qreal vswap, max, h, pivot, q;
	static qreal *mswap, *hvec;

	end = degree - 1;
	for(i = 0; i < end; ++i)
	{
		max = fabs(matrix[i][i]);
		maxi = i;
		for(j = i + 1; j < degree; ++j)
		{
			if((h = fabs(matrix[j][i])) > max)
			{
				max = h;
				maxi = j;
			}
		}
		if(maxi != i)
		{
			mswap = matrix[i];
			matrix[i] = matrix[maxi];
			matrix[maxi] = mswap;
			vswap = coefficients[i];
			coefficients[i] = coefficients[maxi];
			coefficients[maxi] = vswap;
		}

		hvec = matrix[i];
		pivot = hvec[i];
		if(fabs(pivot) == 0.0) return false;

		for(j = i + 1; j < degree; ++j)
		{
			q = - matrix[j][i] / pivot;
			matrix[j][i] = 0.0;
			for(k = i + 1; k < degree; ++k)
			{
				matrix[j][k] += q * hvec[k];
			}
			coefficients[j] += (q * coefficients[i]);
		}
	}

	coefficients[end] /= matrix[end][end];
	for(i = degree - 2; i >= 0; --i)
	{
		hvec = matrix[i];
		for(j = end; j > i; --j)
		{
			coefficients[i] -= (hvec[j] * coefficients[j]);
		}
		coefficients[i] /= hvec[i];
	}
	return true;
}

ViArmaNoiseDetector* ViArmaNoiseDetector::clone()
{
    return new ViArmaNoiseDetector(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViArmaNoiseDetector* create()
{
    return new ViArmaNoiseDetector();
}

#ifdef __cplusplus
}
#endif
