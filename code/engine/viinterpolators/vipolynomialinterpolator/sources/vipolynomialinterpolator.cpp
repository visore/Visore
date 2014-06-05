#include <vipolynomialinterpolator.h>
#include <visystemsolver.h>
#include <vidifferentiater.h>
#include <vilogger.h>
#include <float.h>

#define DEFAULT_DEGREE 3
#define DEFAULT_DERIVATIVE 0

ViPolynomialInterpolator::ViPolynomialInterpolator(const Mode &mode, const Estimation &estimation)
	: ViInterpolator()
{
	mEigen = new ViEigenDoubleZ();

	setPointers(mode, estimation);
	setDegree(DEFAULT_DEGREE);
	setDerivatives(DEFAULT_DERIVATIVE);

	addParameterName("Window Size");
	addParameterName("Degree");
	if(mMode == Osculating || mMode == Splines) addParameterName("Derivatives");
}

ViPolynomialInterpolator::ViPolynomialInterpolator(const ViPolynomialInterpolator &other)
	: ViInterpolator(other)
{
	setPointers(other.mMode, other.mEstimation);
	setDegree(other.mDegree);
	setDerivatives(other.mDerivatives);
}

ViPolynomialInterpolator::~ViPolynomialInterpolator()
{
}

QString ViPolynomialInterpolator::name(QString replace, bool spaced)
{
	QString mode, estimation;

	if(mMode == Normal) mode = "Normal Mode";
	else if(mMode == Osculating) mode = "Osculating Mode";
	else if(mMode == Splines) mode = "Splines Mode";

	if(mEstimation == Fixed) estimation = "Fixed Estimation";
	else if(mEstimation == Best) estimation = "Best Estimation";

	QString name = ViInterpolator::name(replace, spaced) + " (" + mode + ", " + estimation + ")";
	if(spaced) return name;
	else return name.replace(" ", "").replace(",", "_");
}

void ViPolynomialInterpolator::setDegree(const int &degree)
{
	mDegree = degree;
}

void ViPolynomialInterpolator::setDerivatives(const int &derivatives)
{
	if(mMode == Normal) mDerivatives = 0;
	else mDerivatives = derivatives;
}

void ViPolynomialInterpolator::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else if(number == 1) setDegree(value);
	else if(number == 2) setDerivatives(value);
	else
	{
		LOG("Invalid parameter for this interpolator.", QtCriticalMsg);
		exit(-1);
	}
}

bool ViPolynomialInterpolator::validParameters()
{
	if(mEstimation == Best) return true;
	return validParameters(mMode, mWindowSize, mDegree, mDerivatives);
}

bool ViPolynomialInterpolator::validParameters(const Mode &mode, const int &windowSize, const int &degree, const int &derivatives)
{
	if(mode == Normal) return windowSize >= degree + 1;
	else if(mode == Osculating) return windowSize >= degree + 1 && degree >= derivatives;
	else if(mode == Splines)
	{
		if(windowSize <= 1) return false;
		int coefficients = (windowSize - 1) * (degree + 1); // For every spline
		int equations = (windowSize - 1) * 2; // Polynomial for each spline
		equations += (windowSize - 2) * derivatives; // Derivatives at all intermediate points, except the first and last point on every side
		equations += 1 * derivatives; // Set incoming spline (a0) equal to 0
		return equations >= coefficients && degree >= derivatives;
	}
}

bool ViPolynomialInterpolator::validParameters(const Mode &mode, const int &leftSize, const int &rightSize, const int &degree, const int &derivatives)
{
	if((mode == Osculating || mode == Osculating) && (leftSize == 0 || rightSize == 0)) return false;
	return validParameters(mode, leftSize + rightSize, degree, derivatives);
}

void ViPolynomialInterpolator::setPointers(const Mode &mode, const Estimation &estimation)
{
	mMode = mode;
	if(mMode == Normal)
	{
		interpolateModelPointer = &ViPolynomialInterpolator::interpolateModelNormal;
		interpolateBestPointer = &ViPolynomialInterpolator::interpolateBestNormal;
		estimateModelPointer = &ViPolynomialInterpolator::estimateModelNormal;
	}
	/*else if(mMode == Osculating)
	{
		interpolateModelPointer = &ViPolynomialInterpolator::interpolateModelOsculating;
		interpolateBestPointer = &ViPolynomialInterpolator::interpolateBestDerivative;
		estimateModelPointer = &ViPolynomialInterpolator::estimateModelOsculating;
	}
	else if(mMode == Splines)
	{
		interpolateModelPointer = &ViPolynomialInterpolator::interpolateModelSplines;
		interpolateBestPointer = &ViPolynomialInterpolator::interpolateBestDerivative;
		estimateModelPointer = &ViPolynomialInterpolator::estimateModelSplines;
	}*/

	mEstimation = estimation;
	if(mEstimation == Fixed) interpolatePointer = &ViPolynomialInterpolator::interpolateFixed;
	else if(mEstimation == Best) interpolatePointer = &ViPolynomialInterpolator::interpolateBest;
}

bool ViPolynomialInterpolator::interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	// Scaling because multiplying integers (or powers) might lead to variable overflow
	return (this->*interpolatePointer)(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize, qreal(leftSize + outputSize + rightSize - 1));
}

bool ViPolynomialInterpolator::interpolateFixed(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling)
{
	if(validParameters(mMode, leftSize, rightSize, mDegree, mDerivatives))
	{
		//static ViEigenVector coefficients;
		ViEigenVectorZ *coefficients = (this->*estimateModelPointer)(mDegree, mDerivatives, leftSamples, leftSize, rightSamples, rightSize, outputSize, scaling);
		(this->*interpolateModelPointer)(mDegree, coefficients, outputSamples, outputSize, leftSize, scaling);
		mEigen->clear(coefficients);
		return true;
	}
	LOG("Invalid parameter combination detected.", QtCriticalMsg);
	return false;
}

bool ViPolynomialInterpolator::interpolateBest(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling)
{
	return (this->*interpolateBestPointer)(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize, scaling);
}

bool ViPolynomialInterpolator::interpolateBestNormal(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling)
{
	/*static int i, rightStart;
	static ViEigenVector currentCoefficients, bestCoefficients;
	static qreal currentMse, bestMse;

	qreal interpolationLeft[leftSize];
	qreal interpolationRight[rightSize];
	rightStart = leftSize + outputSize;
	bestMse = DBL_MAX;

	for(i = 1; i <= mDegree; ++i)
	{
		if(validParameters(mMode, leftSize, rightSize, i, 0))
		{
			if((this->*estimateModelPointer)(i, 0, currentCoefficients, leftSamples, leftSize, rightSamples, rightSize, outputSize, scaling))
			{
				(this->*interpolateModelPointer)(i, currentCoefficients, interpolationLeft, leftSize, 0, scaling);
				(this->*interpolateModelPointer)(i, currentCoefficients, interpolationRight, rightSize, rightStart, scaling);
				currentMse = calculateMse(leftSamples, interpolationLeft, leftSize) + calculateMse(rightSamples, interpolationRight, rightSize);
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

	(this->*interpolateModelPointer)(i, bestCoefficients, outputSamples, outputSize, leftSize, scaling);
	return true;*/
}

bool ViPolynomialInterpolator::interpolateBestDerivative(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling)
{
	/*static int i, j, rightStart;
	static ViEigenVector currentCoefficients, bestCoefficients;
	static qreal currentMse, bestMse;
	static bool wasBad, wasBadAgain;

	qreal interpolationLeft[leftSize];
	qreal interpolationRight[rightSize];
	rightStart = leftSize + outputSize;
	bestMse = DBL_MAX;
	wasBad = false;
	wasBadAgain = false;

	for(i = 1; i <= mDegree; ++i)
	{
		for(j = 1; j <= mDerivatives; ++j)
		{
			if(validParameters(mMode, leftSize, rightSize, i, j))
			{
				if((this->*estimateModelPointer)(i, j, currentCoefficients, leftSamples, leftSize, rightSamples, rightSize, outputSize, scaling))
				{
					(this->*interpolateModelPointer)(i, currentCoefficients, interpolationLeft, leftSize, 0, scaling);
					(this->*interpolateModelPointer)(i, currentCoefficients, interpolationRight, rightSize, rightStart, scaling);
					currentMse = calculateMse(leftSamples, interpolationLeft, leftSize) + calculateMse(rightSamples, interpolationRight, rightSize);
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
		if(wasBadAgain) break;
	}
	if(bestMse == DBL_MAX) return false;

	(this->*interpolateModelPointer)(i, bestCoefficients, outputSamples, outputSize, leftSize, scaling);
	return true;*/
}


ViEigenVectorZ* ViPolynomialInterpolator::estimateModelNormal(const int &degree, const int &derivative, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling)
{
	static int i, j, offset, size;
	static qreal x;

	size = leftSize + rightSize;

	ViEigenMatrixZ *matrix = mEigen->createMatrix(size, degree + 1);
	ViEigenVectorZ *vector = mEigen->createVector(size);

	for(i = 0; i < leftSize; ++i)
	{
		vector->set(i, leftSamples[i]);
		x = i / scaling;
		matrix->set(i, 0, 1);
		matrix->set(i, 1, x);
		for(j = 2; j <= degree; ++j) matrix->set(i, j, pow(x, j));
	}

	for(i = 0; i < rightSize; ++i)
	{
		offset = leftSize + i;
		vector->set(offset, rightSamples[i]);
		x = (offset + outputSize) / scaling;
		matrix->set(offset, 0, 1);
		matrix->set(offset, 1, x);
		for(j = 2; j <= degree; ++j) matrix->set(offset, j, pow(x, j));
	}

	ViEigenVectorZ *coefficients = mEigen->solve(matrix, vector);
	mEigen->clear(matrix);
	mEigen->clear(vector);
	return coefficients;
}

void ViPolynomialInterpolator::interpolateModelNormal(const int &degree, const ViEigenVectorZ *coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling)
{
	static int i, j;
	static qreal x, result, temp;

	for(i = 0; i < outputSize; ++i)
	{
		x = (start + i) / scaling;
		coefficients->get(temp, 0);
		result = temp;
		coefficients->get(temp, 1);
		result += temp * x;
		for(j = 2; j <= degree; ++j)
		{
			coefficients->get(temp, j);
			result += temp * std::pow(x, j);
		}
		outputSamples[i] = result;
	}
}

/*
bool ViPolynomialInterpolator::estimateModelNormal(const int &degree, const int &derivative, ViEigenVector &coefficients, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling)
{
	static int i, j, offset, size;
	static qreal x;

	size = leftSize + rightSize;

	ViEigenMatrix matrix = EigenX::createMatrix(size, degree + 1);
	ViEigenVector vector = EigenX::createVector(size);

	for(i = 0; i < leftSize; ++i)
	{
		vector(i) = leftSamples[i];
		x = i / scaling;
		matrix(i, 0) = 1;
		matrix(i, 1) = x;
		for(j = 2; j <= degree; ++j) EigenX::power(matrix(i, j), x, j);
	}

	for(i = 0; i < rightSize; ++i)
	{
		offset = leftSize + i;
		vector(offset) = rightSamples[i];
		x = (offset + outputSize) / scaling;
		matrix(offset, 0) = 1;
		matrix(offset, 1) = x;
		for(j = 2; j <= degree; ++j) EigenX::power(matrix(offset, j), x, j);
	}

	EigenX::solve(matrix, vector, coefficients);
	return true;
}

void ViPolynomialInterpolator::interpolateModelNormal(const int &degree, const ViEigenVector &coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling)
{
	static int i, j;
	static qreal x, result;

	for(i = 0; i < outputSize; ++i)
	{
		x = (start + i) / scaling;
		result = EigenX::toReal(coefficients(0) + (coefficients(1) * x));
		for(j = 2; j <= degree; ++j) result += EigenX::toReal(coefficients(j) * EigenX::power(x, j));
		outputSamples[i] = result;
	}
}

bool ViPolynomialInterpolator::estimateModelOsculating(const int &degree, const int &derivative, ViEigenVector &coefficients, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling)
{
	static int i, j, offset, start, end, derivativeCount, totalDerivatives, totalSize, size;
	static qreal x;

	size = leftSize + rightSize;
	derivativeCount = size - 4; // Average, we don't have derivatives for the first and last point for both sides
	totalDerivatives = derivativeCount * derivative;
	totalSize = size + totalDerivatives;

	ViEigenMatrix matrix = EigenX::createMatrix(totalSize, degree + 1);
	ViEigenVector vector = EigenX::createVector(totalSize);

	for(i = 0; i < leftSize; ++i)
	{
		vector(i) = leftSamples[i];
		x = i / scaling;
		matrix(i, 0) = 1;
		matrix(i, 1) = x;
		for(j = 2; j <= degree; ++j) EigenX::power(matrix(i, j), x, j);
	}

	for(i = 0; i < rightSize; ++i)
	{
		offset = leftSize + i;
		vector(offset) = rightSamples[i];
		x = (offset + outputSize) / scaling;
		matrix(offset, 0) = 1;
		matrix(offset, 1) = x;
		for(j = 2; j <= degree; ++j) EigenX::power(matrix(offset, j), x, j);
	}

	// Determine for how many samples we cannot calculate a derivitate, and skip those equations.
	// Der1 and der2 must skip 1 sample at the start and 1 at the end, der3 and der4 skip 2, etc
	start = qCeil(derivative / 2.0);

	end = leftSize - start;
	for(i = 1; i <= derivative; ++i)
	{
		for(j = start; j < end; ++j)
		{
			offset = size + (derivativeCount * (i - 1)) + (j - 1);
			vector(offset) = ViDifferentiate::derivative(i, leftSamples, j);
			calculateDerivative(degree, j / scaling, matrix, offset, i);
		}
	}

	end = rightSize - start;
	for(i = 1; i <= derivative; ++i)
	{
		for(j = start; j < end; ++j)
		{
			offset = (leftSize - 2) + size + (derivativeCount * (i - 1)) + (j - 1);
			vector(offset) = ViDifferentiate::derivative(i, rightSamples, j);
			calculateDerivative(degree, j / scaling, matrix, offset, i);
		}
	}

	EigenX::solve(matrix, vector, coefficients);
	return true;
}

void ViPolynomialInterpolator::interpolateModelOsculating(const int &degree, const ViEigenVector &coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling)
{
	static int i, j;
	static qreal x, result;

	for(i = 0; i < outputSize; ++i)
	{
		x = (start + i) / scaling;
		result = EigenX::toReal(coefficients(0) + (coefficients(1) * x));
		for(j = 2; j <= degree; ++j) result += EigenX::toReal(coefficients(j) * EigenX::power(x, j));
		outputSamples[i] = result;
	}
}

bool ViPolynomialInterpolator::estimateModelSplines(const int &degree, const int &derivative, ViEigenVector &coefficients, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling)
{
	static int i, j, extraEquations, columnOffset, rowOffset1, rowOffset2, rightStart, splineCount, leftSplineCount, rightSplineCount, singleCoefficientCount, coefficientCount, derivativeCount, equationCount, size;
	static qreal x1, x2;

	size = leftSize + rightSize;
	if(size <= 1) return false;
	rightStart = leftSize + outputSize;

	splineCount = size - 1;
	leftSplineCount = leftSize - 1;
	rightSplineCount = rightSize - 1;

	singleCoefficientCount = degree + 1;
	coefficientCount = splineCount * singleCoefficientCount;
	derivativeCount = splineCount - 1;

	extraEquations = 0;

	equationCount = 0;
	equationCount += splineCount * 2; // Spline polynomials. Times 2 since we use the spline with 2 points
	equationCount += derivativeCount * derivative; // Intermediate derivatives should be equal;
	while(equationCount < coefficientCount) // Add first spline == 0
	{
		++equationCount;
		++extraEquations;
	}

	ViEigenMatrix matrix = EigenX::createMatrix(equationCount, coefficientCount);
	ViEigenVector vector = EigenX::createVector(equationCount);

	// Add left spline polynomials
	for(i = 0; i < leftSplineCount; ++i)
	{
		rowOffset1 = i * 2;
		rowOffset2 = rowOffset1 + 1;
		columnOffset = singleCoefficientCount * i;

		vector(rowOffset1) = leftSamples[i];
		vector(rowOffset2) = leftSamples[i + 1];

		x1 = i / scaling;
		x2 = (i + 1) / scaling;

		matrix(rowOffset1, columnOffset) = 1;
		matrix(rowOffset2, columnOffset) = 1;

		matrix(rowOffset1, columnOffset + 1) = x1;
		matrix(rowOffset2, columnOffset + 1) = x2;

		for(j = 2; j <= degree; ++j)
		{
			EigenX::power(matrix(rowOffset1, j + columnOffset), x1, j);
			EigenX::power(matrix(rowOffset2, j + columnOffset), x2, j);
		}
	}

	// Add the spline polynomial that runs over the output gap, aka between the last left and first right sample
	rowOffset1 = leftSplineCount * 2;
	rowOffset2 = rowOffset1 + 1;
	columnOffset = singleCoefficientCount * leftSplineCount;

	vector(rowOffset1) = leftSamples[leftSize - 1];
	vector(rowOffset2) = rightSamples[0];

	x1 = (leftSize - 1) / scaling;
	x2 = (leftSize + outputSize) / scaling;

	matrix(rowOffset1, columnOffset) = 1;
	matrix(rowOffset2, columnOffset) = 1;

	matrix(rowOffset1, columnOffset + 1) = x1;
	matrix(rowOffset2, columnOffset + 1) = x2;

	for(j = 2; j <= degree; ++j)
	{
		EigenX::power(matrix(rowOffset1, j + columnOffset), x1, j);
		EigenX::power(matrix(rowOffset2, j + columnOffset), x2, j);
	}

	// Add right spline polynomials
	for(i = 0; i < rightSplineCount; ++i)
	{
		rowOffset1 = (leftSplineCount + i + 1) * 2;
		rowOffset2 = rowOffset1 + 1;
		columnOffset = singleCoefficientCount * (i + leftSplineCount + 1);

		vector(rowOffset1) = rightSamples[i];
		vector(rowOffset2) = rightSamples[i + 1];

		x1 = (rightStart + i) / scaling;
		x2 = (rightStart + i + 1) / scaling;

		matrix(rowOffset1, columnOffset) = 1;
		matrix(rowOffset2, columnOffset) = 1;

		matrix(rowOffset1, columnOffset + 1) = x1;
		matrix(rowOffset2, columnOffset + 1) = x2;

		for(j = 2; j <= degree; ++j)
		{
			EigenX::power(matrix(rowOffset1, j + columnOffset), x1, j);
			EigenX::power(matrix(rowOffset2, j + columnOffset), x2, j);
		}
	}

	// Add the deratives between neighbouring left splines
	for(i = 1; i <= derivative; ++i)
	{
		rowOffset1 = (splineCount * 2) + ((i - 1) * leftSplineCount);
		for(j = 0; j < leftSplineCount; ++j)
		{
			rowOffset2 = rowOffset1 + j;
			columnOffset = j * singleCoefficientCount;
			x1 = (j + 1) / scaling;
			calculateDerivative(degree, x1, matrix, rowOffset2, i, columnOffset, 1);
			calculateDerivative(degree, x1, matrix, rowOffset2, i, columnOffset + singleCoefficientCount, -1); // -1: take the derivative on the right-hand side to the left of the equation
		}
	}

	// Add the deratives between neighbouring right splines
	for(i = 1; i <= derivative; ++i)
	{
		rowOffset1 = (splineCount * 2) + ((i - 1) * rightSplineCount) + (leftSplineCount * derivative);
		for(j = 0; j < rightSplineCount; ++j)
		{
			rowOffset2 = rowOffset1 + j;
			columnOffset = (j + leftSplineCount) * singleCoefficientCount;
			x1 = (rightStart + j) / scaling;
			calculateDerivative(degree, x1, matrix, rowOffset2, i, columnOffset, 1);
			calculateDerivative(degree, x1, matrix, rowOffset2, i, columnOffset + singleCoefficientCount, -1); // -1: take the derivative on the right-hand side to the left of the equation
		}
	}

	// Set the derivitives for the first spline at point 0 to 0
	if(extraEquations > 0)
	{
		for(i = 1; i <= extraEquations; ++i) matrix(equationCount - i, 0) = 1;
	}

	EigenX::solve(matrix, vector, coefficients);
	return true;
}

void ViPolynomialInterpolator::interpolateModelSplines(const int &degree, const ViEigenVector &coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling)
{
	static int i, j, offset;
	static qreal x, result;

	// Use the middel spline for prediction
	offset = (start - 1) * (degree + 1);

	for(i = 0; i < outputSize; ++i)
	{
		x = (start + i) / scaling;
		result = (coefficients(offset) + EigenX::toReal(coefficients(1 + offset) * x));
		for(j = 2; j <= degree; ++j) result += EigenX::toReal(coefficients(j + offset) * EigenX::power(x, j));
		outputSamples[i] = result;
	}
}

void ViPolynomialInterpolator::calculateDerivative(const int &degree, const qreal &x, ViEigenMatrix &matrix, const int &rowOffset, const int &derivative)
{
	static int i;
	for(i = 0; i < derivative; ++i) matrix(rowOffset, i) = 0;
	matrix(rowOffset, derivative) = 1;
	for(i = derivative + 1; i <= degree; ++i) matrix(rowOffset, i) = i * EigenX::power(x, i - 1);
}

void ViPolynomialInterpolator::calculateDerivative(const int &degree, const qreal &x, ViEigenMatrix &matrix, const int &rowOffset, const int &derivative, const int &offset, const int multiplier)
{
	static int i;
	for(i = 0; i < derivative; ++i) matrix(rowOffset, i + offset) = 0;
	matrix(rowOffset, derivative + offset) = 1;
	for(i = derivative + 1; i <= degree; ++i) matrix(rowOffset, i + offset) = multiplier * i * EigenX::power(x, i - 1);
}

qreal ViPolynomialInterpolator::calculateMse(const qreal *observed, const qreal *predicted, const int &size)
{
	static int i;
	static qreal mse;

	mse = 0;
	for(i = 0; i < size; ++i) mse += qPow(predicted[i] - observed[i], 2);

	return mse / size;
}
*/
ViPolynomialInterpolator* ViPolynomialInterpolator::clone()
{
	return new ViPolynomialInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViPolynomialInterpolator* create()
{
	return new ViPolynomialInterpolator();
}

#ifdef __cplusplus
}
#endif
