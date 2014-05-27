#include <vifourierpredictor.h>
#include <visystemsolver.h>
#include <vilogger.h>
#include <vidifferentiater.h>
#include <float.h>

#define DEFAULT_DEGREE 3
#define DEFAULT_DERIVATIVE 0

ViFourierPredictor::ViFourierPredictor(const Mode &mode, const Estimation &estimation)
	: ViPredictor()
{
	setPointers(mode, estimation);
	setDegree(DEFAULT_DEGREE);
	setDerivatives(DEFAULT_DERIVATIVE);

	addParameterName("Window Size");
	addParameterName("Degree");
	if(mMode == Osculating || mMode == Splines) addParameterName("Derivatives");
}

ViFourierPredictor::ViFourierPredictor(const ViFourierPredictor &other)
	: ViPredictor(other)
{
	setPointers(other.mMode, other.mEstimation);
	setDegree(other.mDegree);
	setDerivatives(other.mDerivatives);
}

ViFourierPredictor::~ViFourierPredictor()
{
}

QString ViFourierPredictor::name(QString replace, bool spaced)
{
	QString mode, estimation;

	if(mMode == Normal) mode = "Normal Mode";
	else if(mMode == Osculating) mode = "Osculating Mode";
	else if(mMode == Splines) mode = "Splines Mode";

	if(mEstimation == Fixed) estimation = "Fixed Estimation";
	else if(mEstimation == Best) estimation = "Best Estimation";

	return ViPredictor::name(replace, spaced) + " (" + mode + ", " + estimation + ")";
}

void ViFourierPredictor::setDegree(const int &degree)
{
	mDegree = degree;

	mBestParameters.clear();
	mBestParameters.resize(mDegree + 1);
	for(int i = 0; i <= mDegree; ++i)
	{
		mBestParameters[i].clear();
		mBestParameters[i].resize(mDerivatives + 1);
		mBestParameters[i].fill(0);
	}
}

void ViFourierPredictor::setDerivatives(const int &derivatives)
{
	if(mMode == Normal) mDerivatives = 0;
	else mDerivatives = derivatives;

	mBestParameters.clear();
	mBestParameters.resize(mDegree + 1);
	for(int i = 0; i <= mDegree; ++i)
	{
		mBestParameters[i].clear();
		mBestParameters[i].resize(mDerivatives + 1);
		mBestParameters[i].fill(0);
	}
}

void ViFourierPredictor::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else if(number == 1) setDegree(value);
	else if(number == 2) setDerivatives(value);
	else
	{
		LOG("Invalid parameter for this predictor.", QtCriticalMsg);
		exit(-1);
	}
}

bool ViFourierPredictor::validParameters()
{
	return validParameters(mMode, mWindowSize, mDegree, mDerivatives);
}

bool ViFourierPredictor::validParameters(const Mode &mode, const int &windowSize, const int &degree, const int &derivatives)
{
	if(mode == Normal) return windowSize >= (degree * 2) + 1;
	else if(mode == Osculating) return (windowSize >= (degree * 2) + 1) && degree >= derivatives;
	else if(mode == Splines)
	{
		if(windowSize <= 1) return false;
		int coefficients = (windowSize - 1) * ((degree * 2) + 1); // For every spline
		int equations = (windowSize - 1) * 2; // Polynomial for each spline
		equations += (windowSize - 2) * derivatives; // Derivatives at all intermediate points
		equations += 1; // Set incoming spline (a0) equal to 0
		return equations >= coefficients && degree >= derivatives;
	}
}

void ViFourierPredictor::setPointers(const Mode &mode, const Estimation &estimation)
{
	mMode = mode;
	if(mMode == Normal)
	{
		predictModelPointer = &ViFourierPredictor::predictModelNormal;
		predictBestPointer = &ViFourierPredictor::predictBestNormal;
		estimateModelPointer = &ViFourierPredictor::estimateModelNormal;
	}
	else if(mMode == Osculating)
	{
		predictModelPointer = &ViFourierPredictor::predictModelOsculating;
		predictBestPointer = &ViFourierPredictor::predictBestDerivative;
		estimateModelPointer = &ViFourierPredictor::estimateModelOsculating;
	}
	else if(mMode == Splines)
	{
		predictModelPointer = &ViFourierPredictor::predictModelSplines;
		predictBestPointer = &ViFourierPredictor::predictBestDerivative;
		estimateModelPointer = &ViFourierPredictor::estimateModelSplines;
	}

	mEstimation = estimation;
	if(mEstimation == Fixed) predictPointer = &ViFourierPredictor::predictFixed;
	else if(mEstimation == Best) predictPointer = &ViFourierPredictor::predictBest;
}

bool ViFourierPredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount)
{
	// Scaling by (predictionCount + size - 1) to get all the x-values in [0,1]. Otherwise working with 1, 2, 3, 4, ... will result in sin(PI*x) to always be 0.
	// Multiply by Pi, since we wok with sin and cos and we want the values to be in [0, Pi] or [0, 2Pi]
	return (this->*predictPointer)(samples, size, predictedSamples, predictionCount, (M_PI / qreal(predictionCount + size - 1)));
}

bool ViFourierPredictor::predictFixed(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling)
{
	if(validParameters(mMode, size, mDegree, mDerivatives))
	{
		static int i;
		static ViVector coefficients;
		if((this->*estimateModelPointer)(mDegree, mDerivatives, coefficients, samples, size, scaling)) (this->*predictModelPointer)(mDegree, coefficients, predictedSamples, predictionCount, size, scaling);
		else for(i = 0; i < predictionCount; ++i) predictedSamples[i] = 0;
		return true;
	}
	LOG("Invalid parameter combination detected.", QtCriticalMsg);
	return false;
}

bool ViFourierPredictor::predictBest(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling)
{
	return (this->*predictBestPointer)(samples, size, predictedSamples, predictionCount, scaling);
}

bool ViFourierPredictor::predictBestNormal(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling)
{
	static int i;
	static ViVector currentCoefficients, bestCoefficients;
	static qreal currentMse, bestMse;

	qreal prediction[size];
	bestMse = DBL_MAX;

	for(i = 1; i <= mDegree; ++i)
	{
		if(validParameters(mMode, size, i, 0))
		{
			if((this->*estimateModelPointer)(i, 0, currentCoefficients, samples, size, scaling))
			{
				(this->*predictModelPointer)(i, currentCoefficients, prediction, size, 0, scaling);
				currentMse = calculateMse(samples, prediction, size);
				if(currentMse < bestMse)
				{
					bestMse = currentMse;
					bestCoefficients = currentCoefficients;
				}
				else break;
			}
		}
	}
	if(bestMse == DBL_MAX) return false;

	--i; // Important: last iteration will increase i before exiting the loop
	mBestParameters[i][0] += 1;
	(this->*predictModelPointer)(i, bestCoefficients, predictedSamples, predictionCount, size, scaling);
	return true;
}

bool ViFourierPredictor::predictBestDerivative(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling)
{
	static int i, j;
	static ViVector currentCoefficients, bestCoefficients;
	static qreal currentMse, bestMse;
	static bool wasBad, wasBadAgain;

	qreal prediction[size];
	bestMse = DBL_MAX;
	wasBad = false;
	wasBadAgain = false;

	for(i = 1; i <= mDegree; ++i)
	{
		for(j = 1; j <= mDerivatives; ++j)
		{
			if(validParameters(mMode, size, i, j))
			{
				if((this->*estimateModelPointer)(i, j, currentCoefficients, samples, size, scaling))
				{
					(this->*predictModelPointer)(i, currentCoefficients, prediction, size, 0, scaling);
					currentMse = calculateMse(samples, prediction, size);
					if(currentMse < bestMse)
					{
						bestMse = currentMse;
						bestCoefficients = currentCoefficients;
						wasBad = false;
					}
					else
					{
						if(wasBad) wasBadAgain = true;
						else wasBad = true;
						break;
					}
				}
			}
		}
		//if(wasBadAgain) break;
	}
	if(bestMse == DBL_MAX) return false;

	--i; --j; // Important: last iteration will increase i before exiting the loop
	mBestParameters[i][j] += 1;
	(this->*predictModelPointer)(i, bestCoefficients, predictedSamples, predictionCount, size, scaling);
	return true;
}

bool ViFourierPredictor::estimateModelNormal(const int &degree, const int &derivative, ViVector &coefficients, const qreal *samples, const int &size, const qreal &scaling)
{
	// http://cant.ua.ac.be/sites/cant.ua.ac.be/files/courses/cscw/ratint/fourier.fausett.pdf
	// http://en.wikipedia.org/wiki/Trigonometric_polynomial

	static int i, j;
	static qreal value1, value2;

	ViMatrix matrix(size, (2 * degree) + 1); // a0, a1 ... an and b1 ... bn
	ViVector vector(size);

	for(i = 0; i < size; ++i)
	{
		vector[i] = samples[i];
		value1 = scaling * i;
		matrix[i][0] = 0.5;
		for(j = 1; j <= degree; ++j)
		{
			value2 = value1 * j;
			matrix[i][j] = qCos(value2);
			matrix[i][j + degree] = qSin(value2);
		}
	}

	return ViSystemSolver::solve(matrix, vector, coefficients);
}

void ViFourierPredictor::predictModelNormal(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling)
{
	static int i, j;
	static qreal a0, value, value1, value2;

	a0 = coefficients[0] / 2;
	for(i = 0; i < size; ++i)
	{
		value1 = scaling * (start + i);
		value = a0;
		for(j = 1; j <= degree; ++j)
		{
			value2 = value1 * j ;
			value += (coefficients[j] * qCos(value2)) + (coefficients[j + degree] * qSin(value2));
		}
		prediction[i] = value;
	}
}

bool ViFourierPredictor::estimateModelOsculating(const int &degree, const int &derivative, ViVector &coefficients, const qreal *samples, const int &size, const qreal &scaling)
{
	static int i, j, offset, start, end, derivativeCount, totalDerivatives, totalSize;
	static qreal value1, value2;

	derivativeCount = size - 2; // Average, we don't have derivatives for the first and last point
	totalDerivatives = derivativeCount * derivative;
	totalSize = size + totalDerivatives;

	ViMatrix matrix(totalSize, (2 * degree) + 1); // a0, a1 ... an and b1 ... bn
	ViVector vector(totalSize);

	for(i = 0; i < size; ++i)
	{
		vector[i] = samples[i];
		value1 = scaling * i;
		matrix[i][0] = 0.5;
		for(j = 1; j <= degree; ++j)
		{
			value2 = value1 * j;
			matrix[i][j] = qCos(value2);
			matrix[i][j + degree] = qSin(value2);
		}
	}

	// Determine for how many samples we cannot calculate a derivitate, and skip those equations.
	// Der1 and der2 must skip 1 sample at the start and 1 at the end, der3 and der4 skip 2, etc
	start = qCeil(derivative / 2.0);

	end = size - start;
	for(i = 1; i <= derivative; ++i)
	{
		for(j = start; j < end; ++j)
		{
			offset = size + (derivativeCount * (i - 1)) + (j - 1);
			bool error;
			vector[offset] = ViDifferentiate::derivative(i, samples, size, j, error);
			calculateDerivative(degree, scaling * j, matrix[offset], i);
		}
	}

	return ViSystemSolver::solve(matrix, vector, coefficients);
}

void ViFourierPredictor::predictModelOsculating(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling)
{
	static int i, j;
	static qreal a0, value1, value2, value3;

	a0 = coefficients[0] / 2;
	for(i = 0; i < size; ++i)
	{
		value2 = scaling * (start + i);
		value1 = a0;
		for(j = 1; j <= degree; ++j)
		{
			value3 = value2 * j ;
			value1 += (coefficients[j] * qCos(value3)) + (coefficients[j + degree] * qSin(value3));
		}
		prediction[i] = value1;
	}
}

bool ViFourierPredictor::estimateModelSplines(const int &degree, const int &derivative, ViVector &coefficients, const qreal *samples, const int &size, const qreal &scaling)
{
	if(size <= 1) return false;

	static int i, j, columnOffset1, columnOffset2, rowOffset1, rowOffset2, splineCount, singleCoefficientCount, coefficientCount, derivativeCount, equationCount;
	static qreal value1, value2, value3, value4;
	static bool addFirstSpline;

	splineCount = (size - 1);
	singleCoefficientCount = ((degree * 2) + 1);
	coefficientCount = splineCount * singleCoefficientCount;
	derivativeCount = splineCount - 1;

	addFirstSpline = false;

	equationCount = 0;
	equationCount += splineCount * 2; // Spline polynomials. Times 2 since we use the spline with 2 points
	equationCount += derivativeCount * derivative; // Intermediate derivatives should be equal;
	if(equationCount < coefficientCount) // Add first spline == 0
	{
		++equationCount;
		addFirstSpline = true;
	}

	ViMatrix matrix(equationCount, coefficientCount);
	ViVector vector(equationCount);

	// Add spline polynomials
	for(i = 0; i < splineCount; ++i)
	{
		rowOffset1 = i * 2;
		rowOffset2 = rowOffset1 + 1;
		columnOffset1 = singleCoefficientCount * i;
		columnOffset2 = columnOffset1 + degree;

		vector[rowOffset1] = samples[i];
		vector[rowOffset2] = samples[i + 1];

		value1 = scaling * i;
		value2 = value1 + scaling;

		matrix[rowOffset1][columnOffset1] = 0.5;
		matrix[rowOffset2][columnOffset1] = 0.5;

		for(j = 1; j <= degree; ++j)
		{
			value3 = value1 * j;
			value4 = value2 * j;

			matrix[rowOffset1][j + columnOffset1] = qCos(value3);
			matrix[rowOffset2][j + columnOffset1] = qCos(value4);

			matrix[rowOffset1][j + columnOffset2] = qSin(value3);
			matrix[rowOffset2][j + columnOffset2] = qSin(value4);
		}
	}

	// Add the deratives between neighbouring splines
	for(i = 1; i <= derivative; ++i)
	{
		rowOffset1 = (splineCount * 2) + ((i - 1) * derivativeCount);
		for(j = 0; j < derivativeCount; ++j)
		{
			rowOffset2 = rowOffset1 + j;
			columnOffset1 = j * singleCoefficientCount;
			value1 = scaling * (j + 1);
			calculateDerivative(degree, value1, matrix[rowOffset2], i, columnOffset1, 1);
			calculateDerivative(degree, value1, matrix[rowOffset2], i, columnOffset1 + singleCoefficientCount, -1); // -1: take the derivative on the right-hand side to the left of the equation
		}
	}

	// Make the first spline 0 if needed
	if(addFirstSpline) matrix[equationCount - 1][0] = 1;

	return ViSystemSolver::solve(matrix, vector, coefficients);
}

void ViFourierPredictor::predictModelSplines(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling)
{
	static int i, j, offset;
	static qreal a0, value1, value2, value3;

	// Use the last spline for prediction
	offset = (start - 2) * ((degree * 2) + 1);

	a0 = coefficients[offset] / 2;
	for(i = 0; i < size; ++i)
	{
		value2 = scaling * (start + i);
		value1 = a0;
		for(j = 1; j <= degree; ++j)
		{
			value3 = value2 * j;
			value1 += (coefficients[offset + j] * qCos(value3)) + (coefficients[offset + j + degree] * qSin(value3));
		}
		prediction[i] = value1;
	}
}

void ViFourierPredictor::calculateDerivative(const int &degree, const qreal &x, ViVector &row, const int &derivative)
{
	static int i;
	static qreal value1, value2;

	row[0] = 0;
	if(derivative % 4 == 0) // 4th, 8th, 12th, ... derivatives
	{
		for(i = 1; i <= degree; ++i)
		{
			value1 = x * i;
			value2 = qPow(i, derivative);
			row[i] = value2 * qCos(value1);
			row[i + degree] = value2 * qSin(value1);
		}
	}
	else if(derivative % 4 == 1) // 1st, 5th, 9th, ... derivatives
	{
		for(i = 1; i <= degree; ++i)
		{
			value1 = x * i;
			value2 = qPow(i, derivative);
			row[i + degree] = value2 * qCos(value1);
			row[i] = -value2 * qSin(value1);
		}
	}
	else if(derivative % 4 == 2) // 2nd, 6th, 10th, ... derivatives
	{
		for(i = 1; i <= degree; ++i)
		{
			value1 = x * i;
			value2 = -qPow(i, derivative);
			row[i] = value2 * qCos(value1);
			row[i + degree] = value2 * qSin(value1);
		}
	}
	else if(derivative % 4 == 3) // 3rd, 7th, 1th, ... derivatives
	{
		for(i = 1; i <= degree; ++i)
		{
			value1 = x * i;
			value2 = qPow(i, derivative);
			row[i + degree] = -value2 * qCos(value1);
			row[i] = value2 * qSin(value1);
		}
	}
}

void ViFourierPredictor::calculateDerivative(const int &degree, const qreal &x, ViVector &row, const int &derivative, const int &offset, const int multiplier)
{
	static int i;
	static qreal value1, value2;

	row[offset] = 0;
	if(derivative % 4 == 0) // 4th, 8th, 12th, ... derivatives
	{
		for(i = 1; i <= degree; ++i)
		{
			value1 = x * i;
			value2 = multiplier * qPow(i, derivative);
			row[i + offset] = value2 * qCos(value1);
			row[i + offset + degree] = value2 * qSin(value1);
		}
	}
	else if(derivative % 4 == 1) // 1st, 5th, 9th, ... derivatives
	{
		for(i = 1; i <= degree; ++i)
		{
			value1 = x * i;
			value2 = multiplier * qPow(i, derivative);
			row[i + offset + degree] = value2 * qCos(value1);
			row[i + offset] = -value2 * qSin(value1);
		}
	}
	else if(derivative % 4 == 2) // 2nd, 6th, 10th, ... derivatives
	{
		for(i = 1; i <= degree; ++i)
		{
			value1 = x * i;
			value2 = multiplier * (-qPow(i, derivative));
			row[i + offset] = value2 * qCos(value1);
			row[i + offset + degree] = value2 * qSin(value1);
		}
	}
	else if(derivative % 4 == 3) // 3rd, 7th, 1th, ... derivatives
	{
		for(i = 1; i <= degree; ++i)
		{
			value1 = x * i;
			value2 = multiplier * qPow(i, derivative);
			row[i + offset + degree] = -value2 * qCos(value1);
			row[i + offset] = value2 * qSin(value1);
		}
	}
}

qreal ViFourierPredictor::calculateMse(const qreal *observed, const qreal *predicted, const int &size)
{
	static int i;
	static qreal mse;

	mse = 0;
	for(i = 0; i < size; ++i) mse += qPow(predicted[i] - observed[i], 2);

	return mse / size;
}
