#include <vifourierinterpolator.h>
#include <visystemsolver.h>
#include <vidifferentiater.h>
#include <vilogger.h>
#include <float.h>

#define DEFAULT_DEGREE 3
#define DEFAULT_DERIVATIVE 0

ViFourierInterpolator::ViFourierInterpolator(const Mode &mode, const Estimation &estimation)
	: ViInterpolator()
{
	setPointers(mode, estimation);
	setDegree(DEFAULT_DEGREE);
	setDerivatives(DEFAULT_DERIVATIVE);

	addParameterName("Window Size");
	addParameterName("Degree");
	if(mMode == Osculating || mMode == Splines) addParameterName("Derivatives");
}

ViFourierInterpolator::ViFourierInterpolator(const ViFourierInterpolator &other)
	: ViInterpolator(other)
{
	setPointers(other.mMode, other.mEstimation);
	setDegree(other.mDegree);
	setDerivatives(other.mDerivatives);
}

ViFourierInterpolator::~ViFourierInterpolator()
{
}

QString ViFourierInterpolator::name(QString replace, bool spaced)
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

void ViFourierInterpolator::setDegree(const int &degree)
{
	mDegree = degree;
}

void ViFourierInterpolator::setDerivatives(const int &derivatives)
{
	if(mMode == Normal) mDerivatives = 0;
	else mDerivatives = derivatives;
}

void ViFourierInterpolator::setParameter(const int &number, const qreal &value)
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

bool ViFourierInterpolator::validParameters()
{
	if(mEstimation == Best) return true;
	return validParameters(mMode, mWindowSize, mDegree, mDerivatives);
}

bool ViFourierInterpolator::validParameters(const Mode &mode, const int &windowSize, const int &degree, const int &derivatives)
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

bool ViFourierInterpolator::validParameters(const Mode &mode, const int &leftSize, const int &rightSize, const int &degree, const int &derivatives)
{
	if((mode == Osculating || mode == Osculating) && (leftSize == 0 || rightSize == 0)) return false;
	return validParameters(mode, leftSize + rightSize, degree, derivatives);
}

void ViFourierInterpolator::setPointers(const Mode &mode, const Estimation &estimation)
{
	mMode = mode;
	if(mMode == Normal)
	{
		interpolateModelPointer = &ViFourierInterpolator::interpolateModelNormal;
		interpolateBestPointer = &ViFourierInterpolator::interpolateBestNormal;
		estimateModelPointer = &ViFourierInterpolator::estimateModelNormal;
	}
	else if(mMode == Osculating)
	{
		interpolateModelPointer = &ViFourierInterpolator::interpolateModelOsculating;
		interpolateBestPointer = &ViFourierInterpolator::interpolateBestDerivative;
		estimateModelPointer = &ViFourierInterpolator::estimateModelOsculating;
	}
	else if(mMode == Splines)
	{
		interpolateModelPointer = &ViFourierInterpolator::interpolateModelSplines;
		interpolateBestPointer = &ViFourierInterpolator::interpolateBestDerivative;
		estimateModelPointer = &ViFourierInterpolator::estimateModelSplines;
	}

	mEstimation = estimation;
	if(mEstimation == Fixed) interpolatePointer = &ViFourierInterpolator::interpolateFixed;
	else if(mEstimation == Best) interpolatePointer = &ViFourierInterpolator::interpolateBest;
}

bool ViFourierInterpolator::interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	// Scaling by (predictionCount + size - 1) to get all the x-values in [0,1]. Otherwise working with 1, 2, 3, 4, ... will result in sin(PI*x) to always be 0.
	// Multiply by Pi, since we wok with sin and cos and we want the values to be in [0, Pi] or [0, 2Pi]
	return (this->*interpolatePointer)(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize, (M_PI / qreal(leftSize + outputSize + rightSize - 1)));
}

bool ViFourierInterpolator::interpolateFixed(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling)
{
	if(validParameters(mMode, leftSize, rightSize, mDegree, mDerivatives))
	{
		static ViVector coefficients;
		if((this->*estimateModelPointer)(mDegree, mDerivatives, coefficients, leftSamples, leftSize, rightSamples, rightSize, outputSize, scaling))
		{
			(this->*interpolateModelPointer)(mDegree, coefficients, outputSamples, outputSize, leftSize, scaling);
		}
		return true;
	}
	LOG("Invalid parameter combination detected.", QtCriticalMsg);
	return false;
}

bool ViFourierInterpolator::interpolateBest(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling)
{
	return (this->*interpolateBestPointer)(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize, scaling);
}

bool ViFourierInterpolator::interpolateBestNormal(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling)
{
	static int i, rightStart;
	static ViVector currentCoefficients, bestCoefficients;
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
	return true;
}

bool ViFourierInterpolator::interpolateBestDerivative(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling)
{
	static int i, j, rightStart;
	static ViVector currentCoefficients, bestCoefficients;
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
	return true;
}

bool ViFourierInterpolator::estimateModelNormal(const int &degree, const int &derivative, ViVector &coefficients, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling)
{
	static int i, j, offset, size;
	static qreal x, value;

	size = leftSize + rightSize;
	ViMatrix matrix(size, (2 * degree) + 1);
	ViVector vector(size);

	for(i = 0; i < leftSize; ++i)
	{
		vector[i] = leftSamples[i];
		x = i * scaling;
		matrix[i][0] = 0.5;
		for(j = 1; j <= degree; ++j)
		{
			value = x * j;
			matrix[i][j] = qCos(value);
			matrix[i][j + degree] = qSin(value);
		}
	}

	for(i = 0; i < rightSize; ++i)
	{
		offset = leftSize + i;
		vector[offset] = rightSamples[i];
		x = (offset + outputSize) * scaling;
		matrix[offset][0] = 0.5;
		for(j = 1; j <= degree; ++j)
		{
			value = x * j;
			matrix[offset][j] = qCos(value);
			matrix[offset][j + degree] = qSin(value);
		}
	}

	return ViSystemSolver::solve(matrix, vector, coefficients);
}

void ViFourierInterpolator::interpolateModelNormal(const int &degree, const ViVector &coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling)
{
	static int i, j;
	static qreal a0, value, value1, value2;

	a0 = coefficients[0] / 2;
	for(i = 0; i < outputSize; ++i)
	{
		value1 = scaling * (start + i);
		value = a0;
		for(j = 1; j <= degree; ++j)
		{
			value2 = value1 * j ;
			value += (coefficients[j] * qCos(value2)) + (coefficients[j + degree] * qSin(value2));
		}
		outputSamples[i] = value;
	}
}

bool ViFourierInterpolator::estimateModelOsculating(const int &degree, const int &derivative, ViVector &coefficients, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling)
{
	static int i, j, offset, start, end, derivativeCount, totalDerivatives, totalSize, size;
	static qreal x, value;

	size = leftSize + rightSize;
	derivativeCount = size - 4; // Average, we don't have derivatives for the first and last point for both sides
	totalDerivatives = derivativeCount * derivative;
	totalSize = size + totalDerivatives;

	ViMatrix matrix(totalSize, (2 * degree) + 1);
	ViVector vector(totalSize);

	for(i = 0; i < leftSize; ++i)
	{
		vector[i] = leftSamples[i];
		x = i * scaling;
		matrix[i][0] = 0.5;
		for(j = 1; j <= degree; ++j)
		{
			value = x * j;
			matrix[i][j] = qCos(value);
			matrix[i][j + degree] = qSin(value);
		}
	}

	for(i = 0; i < rightSize; ++i)
	{
		offset = leftSize + i;
		vector[offset] = rightSamples[i];
		x = (offset + outputSize) * scaling;
		matrix[offset][0] = 0.5;
		for(j = 1; j <= degree; ++j)
		{
			value = x * j;
			matrix[offset][j] = qCos(value);
			matrix[offset][j + degree] = qSin(value);
		}
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
			bool error;
			vector[offset] = ViDifferentiate::derivative(i, leftSamples, leftSize, j, error);
			calculateDerivative(degree, j * scaling, matrix[offset], i);
		}
	}

	end = rightSize - start;
	for(i = 1; i <= derivative; ++i)
	{
		for(j = start; j < end; ++j)
		{
			offset = (leftSize - 2) + size + (derivativeCount * (i - 1)) + (j - 1);
			bool error;
			vector[offset] = ViDifferentiate::derivative(i, rightSamples, rightSize, j, error);
			calculateDerivative(degree, j * scaling, matrix[offset], i);
		}
	}

	return ViSystemSolver::solve(matrix, vector, coefficients);
}

void ViFourierInterpolator::interpolateModelOsculating(const int &degree, const ViVector &coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling)
{
	static int i, j;
	static qreal a0, value, value1, value2;

	a0 = coefficients[0] / 2;
	for(i = 0; i < outputSize; ++i)
	{
		value1 = scaling * (start + i);
		value = a0;
		for(j = 1; j <= degree; ++j)
		{
			value2 = value1 * j ;
			value += (coefficients[j] * qCos(value2)) + (coefficients[j + degree] * qSin(value2));
		}
		outputSamples[i] = value;
	}
}

bool ViFourierInterpolator::estimateModelSplines(const int &degree, const int &derivative, ViVector &coefficients, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling)
{
	static int i, j, extraEquations, columnOffset1, columnOffset2, rowOffset1, rowOffset2, rightStart, splineCount, leftSplineCount, rightSplineCount, singleCoefficientCount, coefficientCount, derivativeCount, equationCount, size;
	static qreal x1, x2, value1, value2;

	size = leftSize + rightSize;
	if(size <= 1) return false;
	rightStart = leftSize + outputSize;

	splineCount = size - 1;
	leftSplineCount = leftSize - 1;
	rightSplineCount = rightSize - 1;

	singleCoefficientCount = (degree * 2) + 1;
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

	ViMatrix matrix(equationCount, coefficientCount);
	ViVector vector(equationCount);

	// Add left spline polynomials
	for(i = 0; i < leftSplineCount; ++i)
	{
		rowOffset1 = i * 2;
		rowOffset2 = rowOffset1 + 1;
		columnOffset1 = singleCoefficientCount * i;
		columnOffset2 = columnOffset1 + degree;

		vector[rowOffset1] = leftSamples[i];
		vector[rowOffset2] = leftSamples[i + 1];

		x1 = i * scaling;
		x2 = (i + 1) * scaling;

		matrix[rowOffset1][columnOffset1] = 0.5;
		matrix[rowOffset2][columnOffset1] = 0.5;

		for(j = 1; j <= degree; ++j)
		{
			value1 = x1 * j;
			value2 = x2 * j;

			matrix[rowOffset1][j + columnOffset1] = qCos(value1);
			matrix[rowOffset2][j + columnOffset1] = qCos(value2);

			matrix[rowOffset1][j + columnOffset2] = qSin(value1);
			matrix[rowOffset2][j + columnOffset2] = qSin(value2);
		}
	}

	// Add the spline polynomial that runs over the output gap, aka between the last left and first right sample
	rowOffset1 = leftSplineCount * 2;
	rowOffset2 = rowOffset1 + 1;
	columnOffset1 = singleCoefficientCount * leftSplineCount;
	columnOffset2 = columnOffset1 + degree;

	vector[rowOffset1] = leftSamples[leftSize - 1];
	vector[rowOffset2] = rightSamples[0];

	x1 = (leftSize - 1) * scaling;
	x2 = (leftSize + outputSize) * scaling;

	matrix[rowOffset1][columnOffset1] = 0.5;
	matrix[rowOffset2][columnOffset1] = 0.5;

	for(j = 1; j <= degree; ++j)
	{
		value1 = x1 * j;
		value2 = x2 * j;

		matrix[rowOffset1][j + columnOffset1] = qCos(value1);
		matrix[rowOffset2][j + columnOffset1] = qCos(value2);

		matrix[rowOffset1][j + columnOffset2] = qSin(value1);
		matrix[rowOffset2][j + columnOffset2] = qSin(value2);
	}

	// Add right spline polynomials
	for(i = 0; i < rightSplineCount; ++i)
	{
		rowOffset1 = (leftSplineCount + i + 1) * 2;
		rowOffset2 = rowOffset1 + 1;
		columnOffset1 = singleCoefficientCount * (i + leftSplineCount + 1);
		columnOffset2 = columnOffset1 + degree;

		vector[rowOffset1] = rightSamples[i];
		vector[rowOffset2] = rightSamples[i + 1];

		x1 = (rightStart + i) * scaling;
		x2 = (rightStart + i + 1) * scaling;

		matrix[rowOffset1][columnOffset1] = 0.5;
		matrix[rowOffset2][columnOffset1] = 0.5;

		for(j = 1; j <= degree; ++j)
		{
			value1 = x1 * j;
			value2 = x2 * j;

			matrix[rowOffset1][j + columnOffset1] = qCos(value1);
			matrix[rowOffset2][j + columnOffset1] = qCos(value2);

			matrix[rowOffset1][j + columnOffset2] = qSin(value1);
			matrix[rowOffset2][j + columnOffset2] = qSin(value2);
		}
	}

	// Add the deratives between neighbouring left splines
	for(i = 1; i <= derivative; ++i)
	{
		rowOffset1 = (splineCount * 2) + ((i - 1) * leftSplineCount);
		for(j = 0; j < leftSplineCount; ++j)
		{
			rowOffset2 = rowOffset1 + j;
			columnOffset1 = j * singleCoefficientCount;
			x1 = (j + 1) * scaling;
			calculateDerivative(degree, x1, matrix[rowOffset2], i, columnOffset1, 1);
			calculateDerivative(degree, x1, matrix[rowOffset2], i, columnOffset1 + singleCoefficientCount, -1); // -1: take the derivative on the right-hand side to the left of the equation
		}
	}

	// Add the deratives between neighbouring right splines
	for(i = 1; i <= derivative; ++i)
	{
		rowOffset1 = (splineCount * 2) + ((i - 1) * rightSplineCount) + (leftSplineCount * derivative);
		for(j = 0; j < rightSplineCount; ++j)
		{
			rowOffset2 = rowOffset1 + j;
			columnOffset1 = (j + leftSplineCount) * singleCoefficientCount;
			x1 = (rightStart + j) * scaling;
			calculateDerivative(degree, x1, matrix[rowOffset2], i, columnOffset1, 1);
			calculateDerivative(degree, x1, matrix[rowOffset2], i, columnOffset1 + singleCoefficientCount, -1); // -1: take the derivative on the right-hand side to the left of the equation
		}
	}

	// Set the derivitives for the first spline at point 0 to 0
	if(extraEquations > 0)
	{
		for(i = 1; i <= extraEquations; ++i) matrix[equationCount - i][0] = 1;
	}

	return ViSystemSolver::solve(matrix, vector, coefficients);
}

void ViFourierInterpolator::interpolateModelSplines(const int &degree, const ViVector &coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling)
{
	static int i, j, offset;
	static qreal x, value1, value2, a0;

	// Use the middel spline for prediction
	offset = (start - 1) * (degree + 1);

	a0 = coefficients[offset] / 2;
	for(i = 0; i < outputSize; ++i)
	{
		x = scaling * (start + i);
		value1 = a0;
		for(j = 1; j <= degree; ++j)
		{
			value2 = x * j;
			value1 += (coefficients[offset + j] * qCos(value2)) + (coefficients[offset + j + degree] * qSin(value2));
		}
		outputSamples[i] = value1;
	}
}

void ViFourierInterpolator::calculateDerivative(const int &degree, const qreal &x, ViVector &row, const int &derivative)
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

void ViFourierInterpolator::calculateDerivative(const int &degree, const qreal &x, ViVector &row, const int &derivative, const int &offset, const int multiplier)
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

qreal ViFourierInterpolator::calculateMse(const qreal *observed, const qreal *predicted, const int &size)
{
	static int i;
	static qreal mse;

	mse = 0;
	for(i = 0; i < size; ++i) mse += qPow(predicted[i] - observed[i], 2);

	return mse / size;
}

ViFourierInterpolator* ViFourierInterpolator::clone()
{
	return new ViFourierInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViFourierInterpolator* create()
{
	return new ViFourierInterpolator();
}

#ifdef __cplusplus
}
#endif
