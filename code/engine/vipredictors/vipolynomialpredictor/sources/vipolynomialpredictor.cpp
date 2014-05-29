#include <vipolynomialpredictor.h>
#include <visystemsolver.h>
#include <vilogger.h>
#include <vidifferentiater.h>
#include <float.h>

#define DEFAULT_DEGREE 3
#define DEFAULT_DERIVATIVE 0

ViPolynomialPredictor::ViPolynomialPredictor(const Mode &mode, const Estimation &estimation)
	: ViPredictor()
{
	setPointers(mode, estimation);
	setDegree(DEFAULT_DEGREE);
	setDerivatives(DEFAULT_DERIVATIVE);

	addParameterName("Window Size");
	addParameterName("Degree");
	if(mMode == Osculating || mMode == Splines) addParameterName("Derivatives");
}

ViPolynomialPredictor::ViPolynomialPredictor(const ViPolynomialPredictor &other)
	: ViPredictor(other)
{
	setPointers(other.mMode, other.mEstimation);
	setDegree(other.mDegree);
	setDerivatives(other.mDerivatives);
}

ViPolynomialPredictor::~ViPolynomialPredictor()
{
}

QString ViPolynomialPredictor::name(QString replace, bool spaced)
{
	QString mode, estimation;

	if(mMode == Normal) mode = "Normal Mode";
	else if(mMode == Osculating) mode = "Osculating Mode";
	else if(mMode == Splines) mode = "Splines Mode";

	if(mEstimation == Fixed) estimation = "Fixed Estimation";
	else if(mEstimation == Best) estimation = "Best Estimation";

	QString name = ViPredictor::name(replace, spaced) + " (" + mode + ", " + estimation + ")";
	if(spaced) return name;
	else return name.replace(" ", "").replace(",", "_");
}

void ViPolynomialPredictor::setDegree(const int &degree)
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

void ViPolynomialPredictor::setDerivatives(const int &derivatives)
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

void ViPolynomialPredictor::setParameter(const int &number, const qreal &value)
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

bool ViPolynomialPredictor::validParameters()
{
	return validParameters(mMode, mWindowSize, mDegree, mDerivatives);
}

bool ViPolynomialPredictor::validParameters(const Mode &mode, const int &windowSize, const int &degree, const int &derivatives)
{
	if(mode == Normal) return windowSize >= degree + 1;
	else if(mode == Osculating) return windowSize >= degree + 1 && degree >= derivatives;
	else if(mode == Splines)
	{
		if(windowSize <= 1) return false;
		int coefficients = (windowSize - 1) * (degree + 1); // For every spline
		int equations = (windowSize - 1) * 2; // Polynomial for each spline
		equations += (windowSize - 2) * derivatives; // Derivatives at all intermediate points
		equations += 1; // Set incoming spline (a0) equal to 0
		return equations >= coefficients && degree >= derivatives;
	}
}

void ViPolynomialPredictor::setPointers(const Mode &mode, const Estimation &estimation)
{
	mMode = mode;
	if(mMode == Normal)
	{
		predictModelPointer = &ViPolynomialPredictor::predictModelNormal;
		predictBestPointer = &ViPolynomialPredictor::predictBestNormal;
		estimateModelPointer = &ViPolynomialPredictor::estimateModelNormal;
	}
	else if(mMode == Osculating)
	{
		predictModelPointer = &ViPolynomialPredictor::predictModelOsculating;
		predictBestPointer = &ViPolynomialPredictor::predictBestDerivative;
		estimateModelPointer = &ViPolynomialPredictor::estimateModelOsculating;
	}
	else if(mMode == Splines)
	{
		predictModelPointer = &ViPolynomialPredictor::predictModelSplines;
		predictBestPointer = &ViPolynomialPredictor::predictBestDerivative;
		estimateModelPointer = &ViPolynomialPredictor::estimateModelSplines;
	}

	mEstimation = estimation;
	if(mEstimation == Fixed) predictPointer = &ViPolynomialPredictor::predictFixed;
	else if(mEstimation == Best) predictPointer = &ViPolynomialPredictor::predictBest;
}

bool ViPolynomialPredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount)
{
	// Scaling because multiplying integers (or powers) might lead to variable overflow
	return (this->*predictPointer)(samples, size, predictedSamples, predictionCount, qreal(predictionCount + size - 1));
}

bool ViPolynomialPredictor::predictFixed(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling)
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

bool ViPolynomialPredictor::predictBest(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling)
{
	return (this->*predictBestPointer)(samples, size, predictedSamples, predictionCount, scaling);
}

bool ViPolynomialPredictor::predictBestNormal(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling)
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

bool ViPolynomialPredictor::predictBestDerivative(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling)
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

bool ViPolynomialPredictor::estimateModelNormal(const int &degree, const int &derivative, ViVector &coefficients, const qreal *samples, const int &size, const qreal &scaling)
{
	static int i, j;
	static qreal x;

	ViMatrix matrix(size, degree + 1);
	ViVector vector(size);

	for(i = 0; i < size; ++i)
	{
		vector[i] = samples[i];
		x = i / scaling;
		matrix[i][0] = 1;
		matrix[i][1] = x;
		for(j = 2; j <= degree; ++j) matrix[i][j] = qPow(x, j);
	}

	return ViSystemSolver::solve(matrix, vector, coefficients);
}

void ViPolynomialPredictor::predictModelNormal(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling)
{
	static int i, j;
	static qreal x, result;

	for(i = 0; i < size; ++i)
	{
		x = (start + i) / scaling;
		result = coefficients[0] + (coefficients[1] * x);
		for(j = 2; j <= degree; ++j) result += coefficients[j] * qPow(x, j);
		prediction[i] = result;
	}
}

bool ViPolynomialPredictor::estimateModelOsculating(const int &degree, const int &derivative, ViVector &coefficients, const qreal *samples, const int &size, const qreal &scaling)
{
	static int i, j, offset, start, end, derivativeCount, totalDerivatives, totalSize;
	static qreal x;

	derivativeCount = size - 2; // Average, we don't have derivatives for the first and last point
	totalDerivatives = derivativeCount * derivative;
	totalSize = size + totalDerivatives;

	ViMatrix matrix(totalSize, degree + 1);
	ViVector vector(totalSize);

	for(i = 0; i < size; ++i)
	{
		vector[i] = samples[i];
		x = i / scaling;
		matrix[i][0] = 1;
		matrix[i][1] = x;
		for(j = 2; j <= degree; ++j) matrix[i][j] = qPow(x, j);
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
			calculateDerivative(degree, j / scaling, matrix[offset], i);
		}
	}

	return ViSystemSolver::solve(matrix, vector, coefficients);
}

void ViPolynomialPredictor::predictModelOsculating(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling)
{
	static int i, j;
	static qreal x, result;

	for(i = 0; i < size; ++i)
	{
		x = (start + i) / scaling;
		result = coefficients[0] + (coefficients[1] * x);
		for(j = 2; j <= degree; ++j) result += coefficients[j] * qPow(x, j);
		prediction[i] = result;
	}
}

bool ViPolynomialPredictor::estimateModelSplines(const int &degree, const int &derivative, ViVector &coefficients, const qreal *samples, const int &size, const qreal &scaling)
{
	if(size <= 1) return false;

	static int i, j, columnOffset, rowOffset1, rowOffset2, splineCount, singleCoefficientCount, coefficientCount, derivativeCount, equationCount;
	static qreal x1, x2;
	static bool addFirstSpline;

	splineCount = size - 1;
	singleCoefficientCount = degree + 1;
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
		columnOffset = singleCoefficientCount * i;

		vector[rowOffset1] = samples[i];
		vector[rowOffset2] = samples[i + 1];

		x1 = i / scaling;
		x2 = (i + 1) / scaling;

		matrix[rowOffset1][columnOffset] = 1;
		matrix[rowOffset2][columnOffset] = 1;

		matrix[rowOffset1][columnOffset + 1] = x1;
		matrix[rowOffset2][columnOffset + 1] = x2;

		for(j = 2; j <= degree; ++j)
		{
			matrix[rowOffset1][j + columnOffset] = qPow(x1, j);
			matrix[rowOffset2][j + columnOffset] = qPow(x2, j);
		}
	}

	// Add the deratives between neighbouring splines
	for(i = 1; i <= derivative; ++i)
	{
		rowOffset1 = (splineCount * 2) + ((i - 1) * derivative);
		for(j = 0; j < derivativeCount; ++j)
		{
			rowOffset2 = rowOffset1 + j;
			columnOffset = j * singleCoefficientCount;
			x1 = (j + 1) / scaling;
			calculateDerivative(degree, x1, matrix[rowOffset2], i, columnOffset, 1);
			calculateDerivative(degree, x1, matrix[rowOffset2], i, columnOffset + singleCoefficientCount, -1); // -1: take the derivative on the right-hand side to the left of the equation
		}
	}

	// Make the first spline 0 if needed
	if(addFirstSpline) matrix[equationCount - 1][0] = 1;

	return ViSystemSolver::solve(matrix, vector, coefficients);
}

void ViPolynomialPredictor::predictModelSplines(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling)
{
	static int i, j, offset;
	static qreal x, result;

	// Use the last spline for prediction
	offset = (start - 2) * (degree + 1);

	for(i = 0; i < size; ++i)
	{
		x = (start + i) / scaling;
		result = coefficients[offset] + (coefficients[1 + offset] * x);
		for(j = 2; j <= degree; ++j) result += coefficients[j + offset] * qPow(x, j);
		prediction[i] = result;
	}
}

void ViPolynomialPredictor::calculateDerivative(const int &degree, const qreal &x, ViVector &row, const int &derivative)
{
	static int i;
	for(i = 0; i < derivative; ++i) row[i] = 0;
	row[derivative] = 1;
	for(i = derivative + 1; i <= degree; ++i) row[i] = i * qPow(x, i - 1);
}

void ViPolynomialPredictor::calculateDerivative(const int &degree, const qreal &x, ViVector &row, const int &derivative, const int &offset, const int multiplier)
{
	static int i;
	for(i = 0; i < derivative; ++i) row[i + offset] = 0;
	row[derivative + offset] = 1;
	for(i = derivative + 1; i <= degree; ++i) row[i + offset] = multiplier * i * qPow(x, i - 1);
}

qreal ViPolynomialPredictor::calculateMse(const qreal *observed, const qreal *predicted, const int &size)
{
	static int i;
	static qreal mse;

	mse = 0;
	for(i = 0; i < size; ++i) mse += qPow(predicted[i] - observed[i], 2);

	return mse / size;
}
