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
	mEigen = NULL;

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
	mEigen = other.mEigen;

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
	setType();
}

void ViPolynomialInterpolator::setType()
{
	if(mDegree <= 15) mEigen = ViEigenManager::getByBits(53);
	else if(mDegree <= 20) mEigen = ViEigenManager::getByBits(64);
	else if(mDegree <= 22) mEigen = ViEigenManager::getByBits(72);
	else if(mDegree <= 26) mEigen = ViEigenManager::getByBits(80);
	else if(mDegree <= 30) mEigen = ViEigenManager::getByBits(88);
	else if(mDegree <= 35) mEigen = ViEigenManager::getByBits(96);
	else if(mDegree <= 40) mEigen = ViEigenManager::getByBits(104);
	else if(mDegree <= 50) mEigen = ViEigenManager::getByBits(112);
	else if(mDegree <= 60) mEigen = ViEigenManager::getByBits(120);
	else if(mDegree <= 70) mEigen = ViEigenManager::getByBits(128);
	else mEigen = ViEigenManager::getByBits(256);
}

bool ViPolynomialInterpolator::validParameters()
{
	if(mEstimation == Best) return true;
	return validParameters(mMode, mWindowSize, mDegree, mDerivatives);
}

bool ViPolynomialInterpolator::validParameters(const Mode &mode, const int &windowSize, const int &degree, const int &derivatives)
{
//	if(!ViEigenManager::isSupported(decimalPrecision(windowSize, degree))) return false;
	if(mode == Normal) return windowSize >= degree + 1;
	else if(mode == Osculating) return windowSize >= degree + 1 && degree >= derivatives && derivatives <= (windowSize / 2.0);
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
	mEstimation = estimation;

	if(mMode == Normal)
	{
		if(mEstimation == Fixed) interpolatePointer = &ViPolynomialInterpolator::interpolateFixed;
		else if(mEstimation == Best) interpolatePointer = &ViPolynomialInterpolator::interpolateBestNormal;
		interpolateModelPointer = &ViPolynomialInterpolator::interpolateModelNormal;
		estimateModelPointer = &ViPolynomialInterpolator::estimateModelNormal;
	}
	else if(mMode == Osculating)
	{
		if(mEstimation == Fixed) interpolatePointer = &ViPolynomialInterpolator::interpolateFixed;
		else if(mEstimation == Best) interpolatePointer = &ViPolynomialInterpolator::interpolateBestOsculating;
		interpolateModelPointer = &ViPolynomialInterpolator::interpolateModelOsculating;
		estimateModelPointer = &ViPolynomialInterpolator::estimateModelOsculating;
	}
	else if(mMode == Splines)
	{
		if(mEstimation == Fixed) interpolatePointer = &ViPolynomialInterpolator::interpolateFixedSplines;
		else if(mEstimation == Best) interpolatePointer = &ViPolynomialInterpolator::interpolateBestSplines;
		interpolateModelPointer = &ViPolynomialInterpolator::interpolateModelSplines;
		estimateModelPointer = &ViPolynomialInterpolator::estimateModelSplines;
	}
}

bool ViPolynomialInterpolator::interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel)
{
	// Scaling because multiplying integers (or powers) might lead to variable overflow
	return (this->*interpolatePointer)(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize, qreal(leftSize + outputSize + rightSize - 1), error);
}

bool ViPolynomialInterpolator::interpolateFixed(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling, ViError *error)
{
	if(validParameters(mMode, leftSize, rightSize, mDegree, mDerivatives))
	{
		ViEigenBaseVector *coefficients = (this->*estimateModelPointer)(mDegree, mDerivatives, leftSamples, leftSize, rightSamples, rightSize, outputSize, scaling);
		(this->*interpolateModelPointer)(mDegree, coefficients, outputSamples, outputSize, leftSize, scaling, NULL);
		if(error != NULL)
		{
			qreal interpolationLeft[leftSize];
			qreal interpolationRight[rightSize];
			int rightStart = leftSize + outputSize;
			(this->*interpolateModelPointer)(mDegree, coefficients, interpolationLeft, leftSize, 0, scaling, NULL);
			(this->*interpolateModelPointer)(mDegree, coefficients, interpolationRight, rightSize, rightStart, scaling, NULL);
			error->add(interpolationLeft, leftSamples, leftSize);
			error->add(interpolationRight, rightSamples, rightSize);
		}
		mEigen->clear(coefficients);
		return true;
	}
	LOG("Invalid parameter combination detected.", QtCriticalMsg);
	return false;
}

bool ViPolynomialInterpolator::interpolateFixedSplines(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling, ViError *error)
{
	if(validParameters(mMode, leftSize, rightSize, mDegree, mDerivatives))
	{
		ViEigenBaseVector *coefficients = (this->*estimateModelPointer)(mDegree, mDerivatives, leftSamples, leftSize, rightSamples, rightSize, outputSize, scaling);

		// Use the middel spline for prediction
		int outputOffset[outputSize];
		splineOffsetOutput(outputOffset, outputSize, mDegree + 1, leftSize);
		(this->*interpolateModelPointer)(mDegree, coefficients, outputSamples, outputSize, leftSize, scaling, outputOffset);

		if(error != NULL)
		{
			qreal interpolationLeft[leftSize];
			qreal interpolationRight[rightSize];
			int rightStart = leftSize + outputSize;

			int leftOffsets[leftSize];
			int rightOffsets[rightSize];
			splineOffsetLeft(leftOffsets, leftSize, mDegree + 1);
			splineOffsetRight(rightOffsets, rightSize, mDegree + 1, leftSize + outputSize, outputSize);
			(this->*interpolateModelPointer)(mDegree, coefficients, interpolationLeft, leftSize, 0, scaling, leftOffsets);
			(this->*interpolateModelPointer)(mDegree, coefficients, interpolationRight, rightSize, rightStart, scaling, rightOffsets);

			error->add(interpolationLeft, leftSamples, leftSize);
			error->add(interpolationRight, rightSamples, rightSize);
		}
		mEigen->clear(coefficients);
		return true;
	}
	LOG("Invalid parameter combination detected.", QtCriticalMsg);
	return false;
}

bool ViPolynomialInterpolator::interpolateBestNormal(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling, ViError *error)
{
	static int i, rightStart, bestDegree;
	static qreal currentMse, bestMse;

	ViEigenBaseVector *bestCoefficients = NULL;

	qreal interpolationLeft[leftSize];
	qreal interpolationRight[rightSize];
	rightStart = leftSize + outputSize;
	bestMse = DBL_MAX;
	bestDegree = 1;

	for(i = 1; i <= mDegree; ++i)
	{
		if(validParameters(mMode, leftSize, rightSize, i, 0))
		{
			ViEigenBaseVector *currentCoefficients = (this->*estimateModelPointer)(i, 0, leftSamples, leftSize, rightSamples, rightSize, outputSize, scaling);
			(this->*interpolateModelPointer)(i, currentCoefficients, interpolationLeft, leftSize, 0, scaling, NULL);
			(this->*interpolateModelPointer)(i, currentCoefficients, interpolationRight, rightSize, rightStart, scaling, NULL);
			currentMse = ViError::calculateMse(interpolationLeft, leftSamples, leftSize, interpolationRight, rightSamples, rightSize);
			if(currentMse < bestMse)
			{
				mEigen->clear(bestCoefficients);
				bestDegree = i;
				bestMse = currentMse;
				bestCoefficients = currentCoefficients;
			}
			else
			{
				mEigen->clear(currentCoefficients);
				break;
			}
		}
	}
	if(bestMse == DBL_MAX) return false;

	if(error != NULL)
	{
		(this->*interpolateModelPointer)(bestDegree, bestCoefficients, interpolationLeft, leftSize, 0, scaling, NULL);
		(this->*interpolateModelPointer)(bestDegree, bestCoefficients, interpolationRight, rightSize, rightStart, scaling, NULL);
		error->add(interpolationLeft, leftSamples, leftSize);
		error->add(interpolationRight, rightSamples, rightSize);
	}

	(this->*interpolateModelPointer)(bestDegree, bestCoefficients, outputSamples, outputSize, leftSize, scaling, NULL);
	mEigen->clear(bestCoefficients);

	return true;
}

bool ViPolynomialInterpolator::interpolateBestOsculating(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling, ViError *error)
{
	static int i, j, rightStart, bestDegree;
	static qreal currentMse, bestMse;
	static bool wasBad, wasBadAgain;

	ViEigenBaseVector *bestCoefficients = NULL;

	qreal interpolationLeft[leftSize];
	qreal interpolationRight[rightSize];

	rightStart = leftSize + outputSize;
	bestMse = DBL_MAX;
	bestDegree = 1;
	wasBad = false;
	wasBadAgain = false;

	for(i = 1; i <= mDegree; ++i)
	{
		for(j = 1; j <= mDerivatives; ++j)
		{
			if(validParameters(mMode, leftSize, rightSize, i, j))
			{
				ViEigenBaseVector *currentCoefficients = (this->*estimateModelPointer)(i, j, leftSamples, leftSize, rightSamples, rightSize, outputSize, scaling);
				(this->*interpolateModelPointer)(i, currentCoefficients, interpolationLeft, leftSize, 0, scaling, NULL);
				(this->*interpolateModelPointer)(i, currentCoefficients, interpolationRight, rightSize, rightStart, scaling, NULL);
				currentMse = ViError::calculateMse(interpolationLeft, leftSamples, leftSize, interpolationRight, rightSamples, rightSize);
				if(currentMse < bestMse)
				{
					mEigen->clear(bestCoefficients);
					bestMse = currentMse;
					bestCoefficients = currentCoefficients;
					bestDegree = i;
					wasBad = false;
				}
				else
				{
					mEigen->clear(currentCoefficients);
					if(wasBad) wasBadAgain = true;
					else wasBad = true;
					break;
				}
			}
		}
		if(wasBadAgain) break;
	}
	if(bestMse == DBL_MAX) return false;

	if(error != NULL)
	{
		(this->*interpolateModelPointer)(bestDegree, bestCoefficients, interpolationLeft, leftSize, 0, scaling, NULL);
		(this->*interpolateModelPointer)(bestDegree, bestCoefficients, interpolationRight, rightSize, rightStart, scaling, NULL);
		error->add(interpolationLeft, leftSamples, leftSize);
		error->add(interpolationRight, rightSamples, rightSize);
	}

	(this->*interpolateModelPointer)(bestDegree, bestCoefficients, outputSamples, outputSize, leftSize, scaling, NULL);
	mEigen->clear(bestCoefficients);

	return true;
}

bool ViPolynomialInterpolator::interpolateBestSplines(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling, ViError *error)
{
	static int i, j, rightStart, bestDegree;
	static qreal currentMse, bestMse;
	static bool wasBad, wasBadAgain;

	ViEigenBaseVector *bestCoefficients = NULL;

	qreal interpolationLeft[leftSize];
	qreal interpolationRight[rightSize];

	rightStart = leftSize + outputSize;
	bestMse = DBL_MAX;
	bestDegree = 1;
	wasBad = false;
	wasBadAgain = false;

	int leftOffsets[leftSize];
	int rightOffsets[rightSize];

	for(i = 1; i <= mDegree; ++i)
	{
		for(j = 1; j <= mDerivatives; ++j)
		{
			if(validParameters(mMode, leftSize, rightSize, i, j))
			{
				ViEigenBaseVector *currentCoefficients = (this->*estimateModelPointer)(i, j, leftSamples, leftSize, rightSamples, rightSize, outputSize, scaling);

				splineOffsetLeft(leftOffsets, leftSize, i + 1);
				splineOffsetRight(rightOffsets, rightSize, i + 1, leftSize + outputSize, outputSize);
				(this->*interpolateModelPointer)(i, currentCoefficients, interpolationLeft, leftSize, 0, scaling, leftOffsets);
				(this->*interpolateModelPointer)(i, currentCoefficients, interpolationRight, rightSize, rightStart, scaling, rightOffsets);

				currentMse = ViError::calculateMse(interpolationLeft, leftSamples, leftSize, interpolationRight, rightSamples, rightSize);
				if(currentMse < bestMse)
				{
					mEigen->clear(bestCoefficients);
					bestMse = currentMse;
					bestCoefficients = currentCoefficients;
					bestDegree = i;
					wasBad = false;
				}
				else
				{
					mEigen->clear(currentCoefficients);
					if(wasBad) wasBadAgain = true;
					else wasBad = true;
					break;
				}
			}
		}
		if(wasBadAgain) break;
	}
	if(bestMse == DBL_MAX) return false;

	if(error != NULL)
	{
		splineOffsetLeft(leftOffsets, leftSize, bestDegree + 1);
		splineOffsetRight(rightOffsets, rightSize, bestDegree + 1, leftSize + outputSize, outputSize);
		(this->*interpolateModelPointer)(bestDegree, bestCoefficients, interpolationLeft, leftSize, 0, scaling, leftOffsets);
		(this->*interpolateModelPointer)(bestDegree, bestCoefficients, interpolationRight, rightSize, rightStart, scaling, rightOffsets);
		error->add(interpolationLeft, leftSamples, leftSize);
		error->add(interpolationRight, rightSamples, rightSize);
	}

	int outputOffset[outputSize];
	splineOffsetOutput(outputOffset, outputSize, bestDegree + 1, leftSize);
	(this->*interpolateModelPointer)(bestDegree, bestCoefficients, outputSamples, outputSize, leftSize, scaling, outputOffset);

	mEigen->clear(bestCoefficients);

	return true;
}

ViEigenBaseVector* ViPolynomialInterpolator::estimateModelNormal(const int &degree, const int &derivative, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling)
{
	static int i, j, offset, size;
	static qreal x;

	size = leftSize + rightSize;

	ViEigenBaseMatrix *matrix = mEigen->createMatrix(size, degree + 1);
	ViEigenBaseVector *vector = mEigen->createVector(size);

	for(i = 0; i < leftSize; ++i)
	{
		vector->set(i, leftSamples[i]);
		x = i / scaling;
		matrix->set(i, 0, 1);
		matrix->set(i, 1, x);
		for(j = 2; j <= degree; ++j) matrix->setPower(i, j, x, j);
	}

	for(i = 0; i < rightSize; ++i)
	{
		offset = leftSize + i;
		vector->set(offset, rightSamples[i]);
		x = (offset + outputSize) / scaling;
		matrix->set(offset, 0, 1);
		matrix->set(offset, 1, x);
		for(j = 2; j <= degree; ++j) matrix->setPower(offset, j, x, j);
	}

	ViEigenBaseVector *coefficients = mEigen->estimate(matrix, vector);
	mEigen->clear(matrix);
	mEigen->clear(vector);
	return coefficients;
}

void ViPolynomialInterpolator::interpolateModelNormal(const int &degree, const ViEigenBaseVector *coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling, const int *offsets)
{
	static int i, j;
	static qreal x;

	ViEigenBaseMatrix *matrix = mEigen->createMatrix(outputSize, degree + 1);
	for(i = 0; i < outputSize; ++i)
	{
		x = (start + i) / scaling;
		matrix->set(i, 0, 1);
		matrix->set(i, 1, x);
		for(j = 2; j <= degree; ++j) matrix->setPower(i, j, x, j);
	}
	mEigen->solve(coefficients, matrix, outputSamples, outputSize);
	mEigen->clear(matrix);
}


ViEigenBaseVector* ViPolynomialInterpolator::estimateModelOsculating(const int &degree, const int &derivative, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling)
{
	static int i, j, offset, start, end, derivativeCount, totalDerivatives, totalSize, size;
	static qreal x;

	size = leftSize + rightSize;
	derivativeCount = size - 4; // Average, we don't have derivatives for the first and last point for both sides
	totalDerivatives = derivativeCount * derivative;
	totalSize = size + totalDerivatives;

	ViEigenBaseMatrix *matrix = mEigen->createMatrix(totalSize, degree + 1);
	ViEigenBaseVector *vector = mEigen->createVector(totalSize);

	for(i = 0; i < leftSize; ++i)
	{
		vector->set(i, leftSamples[i]);
		x = i / scaling;
		matrix->set(i, 0, 1);
		matrix->set(i, 1, x);
		for(j = 2; j <= degree; ++j) matrix->setPower(i, j, x, j);
	}

	for(i = 0; i < rightSize; ++i)
	{
		offset = leftSize + i;
		vector->set(offset, rightSamples[i]);
		x = (offset + outputSize) / scaling;
		matrix->set(offset, 0, 1);
		matrix->set(offset, 1, x);
		for(j = 2; j <= degree; ++j) matrix->setPower(offset, j, x, j);
	}

	// Determine for how many samples we cannot calculate a derivitate, and skip those equations.
	// Der1 and der2 must skip 1 sample at the start and 1 at the end, der3 and der4 skip 2, etc
	start = ceil(derivative / 2.0);

	qreal derivativesLeft[leftSize];
	qreal derivativesRight[rightSize];

	end = leftSize - start;
	for(i = 1; i <= derivative; ++i)
	{
		ViDifferentiater::derivative(i, leftSamples, leftSize, derivativesLeft);
		for(j = start; j < end; ++j)
		{
			offset = size + (derivativeCount * (i - 1)) + (j - 1);
			vector->set(offset, derivativesLeft[j]);
			calculateDerivative(degree, j / scaling, matrix, offset, i);
		}
	}

	end = rightSize - start;
	for(i = 1; i <= derivative; ++i)
	{
		ViDifferentiater::derivative(i, rightSamples, rightSize, derivativesRight);
		for(j = start; j < end; ++j)
		{
			offset = (leftSize - 2) + size + (derivativeCount * (i - 1)) + (j - 1);
			vector->set(offset, derivativesRight[j]);
			calculateDerivative(degree, j / scaling, matrix, offset, i);
		}
	}

	ViEigenBaseVector *coefficients = mEigen->estimate(matrix, vector);
	mEigen->clear(matrix);
	mEigen->clear(vector);
	return coefficients;
}

void ViPolynomialInterpolator::interpolateModelOsculating(const int &degree, const ViEigenBaseVector *coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling, const int *offsets)
{	
	static int i, j;
	static qreal x;

	ViEigenBaseMatrix *matrix = mEigen->createMatrix(outputSize, degree + 1);
	for(i = 0; i < outputSize; ++i)
	{
		x = (start + i) / scaling;
		matrix->set(i, 0, 1);
		matrix->set(i, 1, x);
		for(j = 2; j <= degree; ++j) matrix->setPower(i, j, x, j);
	}
	mEigen->solve(coefficients, matrix, outputSamples, outputSize);
	mEigen->clear(matrix);
}

ViEigenBaseVector* ViPolynomialInterpolator::estimateModelSplines(const int &degree, const int &derivative, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling)
{
	static int i, j, extraEquations, columnOffset, rowOffset1, rowOffset2, rightStart, splineCount, leftSplineCount, rightSplineCount, singleCoefficientCount, coefficientCount, derivativeCount, equationCount, size;
	static qreal x1, x2;

	size = leftSize + rightSize;
	if(size <= 1) return NULL;
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

	ViEigenBaseMatrix *matrix = mEigen->createMatrix(equationCount, coefficientCount);
	ViEigenBaseVector *vector = mEigen->createVector(equationCount);

	// Add left spline polynomials
	for(i = 0; i < leftSplineCount; ++i)
	{
		rowOffset1 = i * 2;
		rowOffset2 = rowOffset1 + 1;
		columnOffset = singleCoefficientCount * i;

		vector->set(rowOffset1, leftSamples[i]);
		vector->set(rowOffset2, leftSamples[i + 1]);

		x1 = i / scaling;
		x2 = (i + 1) / scaling;

		matrix->set(rowOffset1, columnOffset, 1);
		matrix->set(rowOffset2, columnOffset, 1);

		matrix->set(rowOffset1, columnOffset + 1, x1);
		matrix->set(rowOffset2, columnOffset + 1, x2);

		for(j = 2; j <= degree; ++j)
		{
			matrix->setPower(rowOffset1, j + columnOffset, x1, j);
			matrix->setPower(rowOffset2, j + columnOffset, x2, j);
		}
	}

	// Add the spline polynomial that runs over the output gap, aka between the last left and first right sample
	rowOffset1 = leftSplineCount * 2;
	rowOffset2 = rowOffset1 + 1;
	columnOffset = singleCoefficientCount * leftSplineCount;

	vector->set(rowOffset1, leftSamples[leftSize - 1]);
	vector->set(rowOffset2, rightSamples[0]);

	x1 = (leftSize - 1) / scaling;
	x2 = (leftSize + outputSize) / scaling;

	matrix->set(rowOffset1, columnOffset, 1);
	matrix->set(rowOffset2, columnOffset, 1);

	matrix->set(rowOffset1, columnOffset + 1, x1);
	matrix->set(rowOffset2, columnOffset + 1, x2);

	for(j = 2; j <= degree; ++j)
	{
		matrix->setPower(rowOffset1, j + columnOffset, x1, j);
		matrix->setPower(rowOffset2, j + columnOffset, x2, j);
	}

	// Add right spline polynomials
	for(i = 0; i < rightSplineCount; ++i)
	{
		rowOffset1 = (leftSplineCount + i + 1) * 2;
		rowOffset2 = rowOffset1 + 1;
		columnOffset = singleCoefficientCount * (i + leftSplineCount + 1);

		vector->set(rowOffset1, rightSamples[i]);
		vector->set(rowOffset2, rightSamples[i + 1]);

		x1 = (rightStart + i) / scaling;
		x2 = (rightStart + i + 1) / scaling;

		matrix->set(rowOffset1, columnOffset, 1);
		matrix->set(rowOffset2, columnOffset, 1);

		matrix->set(rowOffset1, columnOffset + 1, x1);
		matrix->set(rowOffset2, columnOffset + 1, x2);

		for(j = 2; j <= degree; ++j)
		{
			matrix->setPower(rowOffset1, j + columnOffset, x1, j);
			matrix->setPower(rowOffset2, j + columnOffset, x2, j);
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
		for(i = 1; i <= extraEquations; ++i) matrix->set(equationCount - i, 0, 1);
	}

	ViEigenBaseVector *coefficients = mEigen->estimate(matrix, vector);

	mEigen->clear(matrix);
	mEigen->clear(vector);
	return coefficients;
}

void ViPolynomialInterpolator::interpolateModelSplines(const int &degree, const ViEigenBaseVector *coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling, const int *offsets)
{
	static int i, j;
	static qreal x;

	ViEigenBaseMatrix *matrix = mEigen->createMatrix(outputSize, degree + 1);
	for(i = 0; i < outputSize; ++i)
	{
		x = (start + i) / scaling;
		matrix->set(i, 0, 1);
		matrix->set(i, 1, x);
		for(j = 2; j <= degree; ++j) matrix->setPower(i, j, x, j);
	}
	mEigen->solve(coefficients, matrix, outputSamples, outputSize, offsets);
	mEigen->clear(matrix);
}

void ViPolynomialInterpolator::calculateDerivative(const int &degree, const qreal &x, ViEigenBaseMatrix *matrix, const int &rowOffset, const int &derivative)
{
	static int i;
	for(i = 0; i < derivative; ++i) matrix->set(rowOffset, i, 0);
	matrix->set(rowOffset, derivative, 1);
	for(i = derivative + 1; i <= degree; ++i) matrix->setPowerMulti(rowOffset, i, i, x, i - 1);
}

void ViPolynomialInterpolator::calculateDerivative(const int &degree, const qreal &x, ViEigenBaseMatrix *matrix, const int &rowOffset, const int &derivative, const int &offset, const int multiplier)
{
	static int i;
	for(i = 0; i < derivative; ++i) matrix->set(rowOffset, i + offset, 0);
	matrix->set(rowOffset, derivative + offset, multiplier);
	for(i = derivative + 1; i <= degree; ++i) matrix->setPowerMulti(rowOffset, i + offset, multiplier * i, x, i - 1);
}

int ViPolynomialInterpolator::decimalPrecision(const int &windowSize, const int &degree)
{
	mpfr::mpreal value(1, 1024);
	value = mpfr::pow(value / windowSize, degree);
	QString valueString = QString::fromStdString(value.toString());
	int start = valueString.indexOf("e-");
	if(start < 0)
	{
		start = valueString.indexOf(",");
		if(start < 0) start = valueString.indexOf(".");
		if(start < 0) return 0;
		while(valueString.at(start + 1) == '0') ++start;
		return start/2;
	}
	else
	{
		return valueString.mid(start + 2).toInt()/2;
	}
}

void ViPolynomialInterpolator::splineOffsetLeft(int *offsets, const int &size, const int &coefficientCount)
{
	static int i;
	offsets[0] = 0;
	for(i = 1; i < size; ++i) offsets[i] = (i - 1) * coefficientCount;
}

void ViPolynomialInterpolator::splineOffsetOutput(int *offsets, const int &size, const int &coefficientCount, const int &start)
{
	static int i;
	for(i = 0; i < size; ++i) offsets[i] = (start - 1) * coefficientCount;
}

void ViPolynomialInterpolator::splineOffsetRight(int *offsets, const int &size, const int &coefficientCount, const int &start, const int &outputSize)
{
	static int i;
	offsets[0] = start - outputSize;
	for(i = 1; i < size; ++i) offsets[i] = (start + i - 1 - outputSize) * coefficientCount;
}

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
