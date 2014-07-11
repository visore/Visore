#include <vifourierinterpolator.h>
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
	setType();
}

void ViFourierInterpolator::setType()
{
	if(mDegree < 15) mEigen = ViEigenManager::getByBits(53);
	else if(mDegree < 25) mEigen = ViEigenManager::getByBits(64);
	else if(mDegree < 30) mEigen = ViEigenManager::getByBits(72);
	else if(mDegree < 35) mEigen = ViEigenManager::getByBits(80);
	else if(mDegree < 40) mEigen = ViEigenManager::getByBits(88);
	else if(mDegree < 45) mEigen = ViEigenManager::getByBits(96);
	else if(mDegree < 50) mEigen = ViEigenManager::getByBits(104);
	else if(mDegree < 55) mEigen = ViEigenManager::getByBits(112);
	else if(mDegree < 60) mEigen = ViEigenManager::getByBits(120);
	else if(mDegree < 65) mEigen = ViEigenManager::getByBits(128);
	else mEigen = ViEigenManager::getByBits(256);
}

bool ViFourierInterpolator::validParameters()
{
	if(mEstimation == Best) return true;
	return validParameters(mMode, mWindowSize, mDegree, mDerivatives);
}

bool ViFourierInterpolator::validParameters(const Mode &mode, const int &windowSize, const int &degree, const int &derivatives)
{
	if(mode == Normal) return windowSize >= (degree * 2) + 1;
	else if(mode == Osculating) return (windowSize >= (degree * 2) + 1) && degree >= derivatives && derivatives <= (windowSize / 2.0);
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
	mEstimation = estimation;

	if(mMode == Normal)
	{
		if(mEstimation == Fixed) interpolatePointer = &ViFourierInterpolator::interpolateFixed;
		else if(mEstimation == Best) interpolatePointer = &ViFourierInterpolator::interpolateBestNormal;
		interpolateModelPointer = &ViFourierInterpolator::interpolateModelNormal;
		estimateModelPointer = &ViFourierInterpolator::estimateModelNormal;
	}
	else if(mMode == Osculating)
	{
		if(mEstimation == Fixed) interpolatePointer = &ViFourierInterpolator::interpolateFixed;
		else if(mEstimation == Best) interpolatePointer = &ViFourierInterpolator::interpolateBestOsculating;
		interpolateModelPointer = &ViFourierInterpolator::interpolateModelOsculating;
		estimateModelPointer = &ViFourierInterpolator::estimateModelOsculating;
	}
	else if(mMode == Splines)
	{
		if(mEstimation == Fixed) interpolatePointer = &ViFourierInterpolator::interpolateFixedSplines;
		else if(mEstimation == Best) interpolatePointer = &ViFourierInterpolator::interpolateBestSplines;
		interpolateModelPointer = &ViFourierInterpolator::interpolateModelSplines;
		estimateModelPointer = &ViFourierInterpolator::estimateModelSplines;
	}
}

bool ViFourierInterpolator::interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel)
{
	// Scaling by (predictionCount + size - 1) to get all the x-values in [0,1]. Otherwise working with 1, 2, 3, 4, ... will result in sin(PI*x) to always be 0.
	// Multiply by Pi, since we wok with sin and cos and we want the values to be in [0, Pi] or [0, 2Pi]
	return (this->*interpolatePointer)(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize, (M_PI / qreal(leftSize + outputSize + rightSize - 1)), error);
}

bool ViFourierInterpolator::interpolateFixed(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling, ViError *error)
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

bool ViFourierInterpolator::interpolateFixedSplines(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling, ViError *error)
{
	if(validParameters(mMode, leftSize, rightSize, mDegree, mDerivatives))
	{
		ViEigenBaseVector *coefficients = (this->*estimateModelPointer)(mDegree, mDerivatives, leftSamples, leftSize, rightSamples, rightSize, outputSize, scaling);

		// Use the middel spline for prediction
		int outputOffset[outputSize];
		splineOffsetOutput(outputOffset, outputSize, (mDegree * 2) + 1, leftSize);
		(this->*interpolateModelPointer)(mDegree, coefficients, outputSamples, outputSize, leftSize, scaling, outputOffset);

		if(error != NULL)
		{
			qreal interpolationLeft[leftSize];
			qreal interpolationRight[rightSize];
			int rightStart = leftSize + outputSize;

			int leftOffsets[leftSize];
			int rightOffsets[rightSize];
			splineOffsetLeft(leftOffsets, leftSize, (mDegree * 2) + 1);
			splineOffsetRight(rightOffsets, rightSize, (mDegree * 2) + 1, leftSize + outputSize, outputSize);
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

bool ViFourierInterpolator::interpolateBestNormal(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling, ViError *error)
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

bool ViFourierInterpolator::interpolateBestOsculating(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling, ViError *error)
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
					bestDegree = i;
					bestMse = currentMse;
					bestCoefficients = currentCoefficients;
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

bool ViFourierInterpolator::interpolateBestSplines(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling, ViError *error)
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

				splineOffsetLeft(leftOffsets, leftSize, (i * 2) + 1);
				splineOffsetRight(rightOffsets, rightSize, (i * 2) + 1, leftSize + outputSize, outputSize);
				(this->*interpolateModelPointer)(i, currentCoefficients, interpolationLeft, leftSize, 0, scaling, leftOffsets);
				(this->*interpolateModelPointer)(i, currentCoefficients, interpolationRight, rightSize, rightStart, scaling, rightOffsets);

				currentMse = ViError::calculateMse(interpolationLeft, leftSamples, leftSize, interpolationRight, rightSamples, rightSize);
				if(currentMse < bestMse)
				{
					mEigen->clear(bestCoefficients);
					bestDegree = i;
					bestMse = currentMse;
					bestCoefficients = currentCoefficients;
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
		splineOffsetLeft(leftOffsets, leftSize, (bestDegree * 2) + 1);
		splineOffsetRight(rightOffsets, rightSize, (bestDegree * 2) + 1, leftSize + outputSize, outputSize);
		(this->*interpolateModelPointer)(bestDegree, bestCoefficients, interpolationLeft, leftSize, 0, scaling, leftOffsets);
		(this->*interpolateModelPointer)(bestDegree, bestCoefficients, interpolationRight, rightSize, rightStart, scaling, rightOffsets);
		error->add(interpolationLeft, leftSamples, leftSize);
		error->add(interpolationRight, rightSamples, rightSize);
	}

	int outputOffset[outputSize];
	splineOffsetOutput(outputOffset, outputSize, (bestDegree * 2) + 1, leftSize);
	(this->*interpolateModelPointer)(bestDegree, bestCoefficients, outputSamples, outputSize, leftSize, scaling, outputOffset);
	mEigen->clear(bestCoefficients);

	return true;
}

ViEigenBaseVector* ViFourierInterpolator::estimateModelNormal(const int &degree, const int &derivative, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling)
{
	static int i, j, offset, size;
	static qreal x, value;

	size = leftSize + rightSize;

	ViEigenBaseMatrix *matrix = mEigen->createMatrix(size, (2 * degree) + 1);
	ViEigenBaseVector *vector = mEigen->createVector(size);

	for(i = 0; i < leftSize; ++i)
	{
		vector->set(i, leftSamples[i]);
		x = i * scaling;
		matrix->set(i, 0, 0.5);
		for(j = 1; j <= degree; ++j)
		{
			value = x * j;
			matrix->setCos(i, j, value);
			matrix->setSin(i, j + degree, value);
		}
	}

	for(i = 0; i < rightSize; ++i)
	{
		offset = leftSize + i;
		vector->set(offset, rightSamples[i]);
		x = (offset + outputSize) * scaling;
		matrix->set(offset, 0, 0.5);
		for(j = 1; j <= degree; ++j)
		{
			value = x * j;
			matrix->setCos(offset, j, value);
			matrix->setSin(offset, j + degree, value);
		}
	}

	ViEigenBaseVector *coefficients = mEigen->estimate(matrix, vector);
	mEigen->clear(matrix);
	mEigen->clear(vector);
	return coefficients;
}

void ViFourierInterpolator::interpolateModelNormal(const int &degree, const ViEigenBaseVector *coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling, const int *offsets)
{
	static int i, j;
	static qreal value, x;

	ViEigenBaseMatrix *matrix = mEigen->createMatrix(outputSize, (2 * degree) + 1);
	for(i = 0; i < outputSize; ++i)
	{
		x = scaling * (start + i);
		matrix->set(i, 0, 1);
		for(j = 1; j <= degree; ++j)
		{
			value = x * j ;
			matrix->setCos(i, j, value);
			matrix->setSin(i, j + degree, value);
		}
	}
	mEigen->solve(coefficients, matrix, outputSamples, outputSize);
	mEigen->clear(matrix);
}

ViEigenBaseVector* ViFourierInterpolator::estimateModelOsculating(const int &degree, const int &derivative, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling)
{
	static int i, j, offset, start, end, derivativeCount, totalDerivatives, totalSize, size;
	static qreal x, value;

	size = leftSize + rightSize;
	derivativeCount = size - 4; // Average, we don't have derivatives for the first and last point for both sides
	totalDerivatives = derivativeCount * derivative;
	totalSize = size + totalDerivatives;

	ViEigenBaseMatrix *matrix = mEigen->createMatrix(totalSize, (2 * degree) + 1);
	ViEigenBaseVector *vector = mEigen->createVector(totalSize);

	for(i = 0; i < leftSize; ++i)
	{
		vector->set(i, leftSamples[i]);
		x = i * scaling;
		matrix->set(i, 0, 0.5);
		for(j = 1; j <= degree; ++j)
		{
			value = x * j;
			matrix->setCos(i, j, value);
			matrix->setSin(i, j + degree, value);
		}
	}

	for(i = 0; i < rightSize; ++i)
	{
		offset = leftSize + i;
		vector->set(offset, rightSamples[i]);
		x = (offset + outputSize) * scaling;
		matrix->set(offset, 0, 0.5);
		for(j = 1; j <= degree; ++j)
		{
			value = x * j;
			matrix->setCos(offset, j, value);
			matrix->setSin(offset, j + degree, value);
		}
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
			calculateDerivative(degree, j * scaling, matrix, offset, i);
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
			calculateDerivative(degree, j * scaling, matrix, offset, i);
		}
	}

	ViEigenBaseVector *coefficients = mEigen->estimate(matrix, vector);
	mEigen->clear(matrix);
	mEigen->clear(vector);
	return coefficients;
}

void ViFourierInterpolator::interpolateModelOsculating(const int &degree, const ViEigenBaseVector *coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling, const int *offsets)
{
	static int i, j;
	static qreal value, x;

	ViEigenBaseMatrix *matrix = mEigen->createMatrix(outputSize, (2 * degree) + 1);
	for(i = 0; i < outputSize; ++i)
	{
		x = scaling * (start + i);
		matrix->set(i, 0, 1);
		for(j = 1; j <= degree; ++j)
		{
			value = x * j;
			matrix->setCos(i, j, value);
			matrix->setSin(i, j + degree, value);
		}
	}
	mEigen->solve(coefficients, matrix, outputSamples, outputSize);
	mEigen->clear(matrix);
}

ViEigenBaseVector* ViFourierInterpolator::estimateModelSplines(const int &degree, const int &derivative, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling)
{
	static int i, j, extraEquations, columnOffset1, columnOffset2, rowOffset1, rowOffset2, rightStart, splineCount, leftSplineCount, rightSplineCount, singleCoefficientCount, coefficientCount, derivativeCount, equationCount, size;
	static qreal x1, x2, value1, value2;

	size = leftSize + rightSize;
	if(size <= 1) return NULL;
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

	ViEigenBaseMatrix *matrix = mEigen->createMatrix(equationCount, coefficientCount);
	ViEigenBaseVector *vector = mEigen->createVector(equationCount);

	// Add left spline polynomials
	for(i = 0; i < leftSplineCount; ++i)
	{
		rowOffset1 = i * 2;
		rowOffset2 = rowOffset1 + 1;
		columnOffset1 = singleCoefficientCount * i;
		columnOffset2 = columnOffset1 + degree;

		vector->set(rowOffset1, leftSamples[i]);
		vector->set(rowOffset2, leftSamples[i + 1]);

		x1 = i * scaling;
		x2 = (i + 1) * scaling;

		matrix->set(rowOffset1, columnOffset1, 0.5);
		matrix->set(rowOffset2, columnOffset1, 0.5);

		for(j = 1; j <= degree; ++j)
		{
			value1 = x1 * j;
			value2 = x2 * j;

			matrix->setCos(rowOffset1, j + columnOffset1, value1);
			matrix->setCos(rowOffset2, j + columnOffset1, value2);

			matrix->setSin(rowOffset1, j + columnOffset2, value1);
			matrix->setSin(rowOffset2, j + columnOffset2, value2);
		}
	}

	// Add the spline polynomial that runs over the output gap, aka between the last left and first right sample
	rowOffset1 = leftSplineCount * 2;
	rowOffset2 = rowOffset1 + 1;
	columnOffset1 = singleCoefficientCount * leftSplineCount;
	columnOffset2 = columnOffset1 + degree;

	vector->set(rowOffset1, leftSamples[leftSize - 1]);
	vector->set(rowOffset2, rightSamples[0]);

	x1 = (leftSize - 1) * scaling;
	x2 = (leftSize + outputSize) * scaling;

	matrix->set(rowOffset1, columnOffset1, 0.5);
	matrix->set(rowOffset2, columnOffset1, 0.5);

	for(j = 1; j <= degree; ++j)
	{
		value1 = x1 * j;
		value2 = x2 * j;

		matrix->setCos(rowOffset1, j + columnOffset1, value1);
		matrix->setCos(rowOffset2, j + columnOffset1, value2);

		matrix->setSin(rowOffset1, j + columnOffset2, value1);
		matrix->setSin(rowOffset2, j + columnOffset2, value2);
	}

	// Add right spline polynomials
	for(i = 0; i < rightSplineCount; ++i)
	{
		rowOffset1 = (leftSplineCount + i + 1) * 2;
		rowOffset2 = rowOffset1 + 1;
		columnOffset1 = singleCoefficientCount * (i + leftSplineCount + 1);
		columnOffset2 = columnOffset1 + degree;

		vector->set(rowOffset1, rightSamples[i]);
		vector->set(rowOffset2, rightSamples[i + 1]);

		x1 = (rightStart + i) * scaling;
		x2 = (rightStart + i + 1) * scaling;

		matrix->set(rowOffset1, columnOffset1, 0.5);
		matrix->set(rowOffset2, columnOffset1, 0.5);

		for(j = 1; j <= degree; ++j)
		{
			value1 = x1 * j;
			value2 = x2 * j;

			matrix->setCos(rowOffset1, j + columnOffset1, value1);
			matrix->setCos(rowOffset2, j + columnOffset1, value2);

			matrix->setSin(rowOffset1, j + columnOffset2, value1);
			matrix->setSin(rowOffset2, j + columnOffset2, value2);
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
			calculateDerivative(degree, x1, matrix, rowOffset2, i, columnOffset1, 1);
			calculateDerivative(degree, x1, matrix, rowOffset2, i, columnOffset1 + singleCoefficientCount, -1); // -1: take the derivative on the right-hand side to the left of the equation
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
			calculateDerivative(degree, x1, matrix, rowOffset2, i, columnOffset1, 1);
			calculateDerivative(degree, x1, matrix, rowOffset2, i, columnOffset1 + singleCoefficientCount, -1); // -1: take the derivative on the right-hand side to the left of the equation
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

void ViFourierInterpolator::interpolateModelSplines(const int &degree, const ViEigenBaseVector *coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling, const int *offsets)
{
	static int i, j;
	static qreal x, value;

	ViEigenBaseMatrix *matrix = mEigen->createMatrix(outputSize, (2 * degree) + 1);
	for(i = 0; i < outputSize; ++i)
	{
		x = scaling * (start + i);
		matrix->set(i, 0, 1);
		for(j = 1; j <= degree; ++j)
		{
			value = x * j;
			matrix->setCos(i, j, value);
			matrix->setSin(i, j + degree, value);
		}
	}

	mEigen->solve(coefficients, matrix, outputSamples, outputSize, offsets);
	mEigen->clear(matrix);
}

void ViFourierInterpolator::calculateDerivative(const int &degree, const qreal &x, ViEigenBaseMatrix *matrix, const int &rowOffset, const int &derivative)
{
	static int i;
	static qreal value1, value2;

	matrix->set(rowOffset, 0, 0);
	if(derivative % 4 == 0) // 4th, 8th, 12th, ... derivatives
	{
		for(i = 1; i <= degree; ++i)
		{
			value1 = x * i;
			value2 = qPow(i, derivative);
			matrix->setCosMulti(rowOffset, i, value2, value1);
			matrix->setSinMulti(rowOffset, i + degree, value2, value1);
		}
	}
	else if(derivative % 4 == 1) // 1st, 5th, 9th, ... derivatives
	{
		for(i = 1; i <= degree; ++i)
		{
			value1 = x * i;
			value2 = qPow(i, derivative);
			matrix->setCosMulti(rowOffset, i + degree, value2, value1);
			matrix->setSinMulti(rowOffset, i, -value2, value1);
		}
	}
	else if(derivative % 4 == 2) // 2nd, 6th, 10th, ... derivatives
	{
		for(i = 1; i <= degree; ++i)
		{
			value1 = x * i;
			value2 = -qPow(i, derivative);
			matrix->setCosMulti(rowOffset, i, value2, value1);
			matrix->setSinMulti(rowOffset, i + degree, value2, value1);
		}
	}
	else if(derivative % 4 == 3) // 3rd, 7th, 1th, ... derivatives
	{
		for(i = 1; i <= degree; ++i)
		{
			value1 = x * i;
			value2 = qPow(i, derivative);
			matrix->setCosMulti(rowOffset, i + degree, -value2, value1);
			matrix->setSinMulti(rowOffset, i, value2, value1);
		}
	}
}

void ViFourierInterpolator::calculateDerivative(const int &degree, const qreal &x, ViEigenBaseMatrix *matrix, const int &rowOffset, const int &derivative, const int &offset, const int multiplier)
{
	static int i;
	static qreal value1, value2;

	matrix->set(rowOffset, offset, 0);
	if(derivative % 4 == 0) // 4th, 8th, 12th, ... derivatives
	{
		for(i = 1; i <= degree; ++i)
		{
			value1 = x * i;
			value2 = multiplier * qPow(i, derivative);
			matrix->setCosMulti(rowOffset, i + offset, value2, value1);
			matrix->setSinMulti(rowOffset, i + offset + degree, value2, value1);
		}
	}
	else if(derivative % 4 == 1) // 1st, 5th, 9th, ... derivatives
	{
		for(i = 1; i <= degree; ++i)
		{
			value1 = x * i;
			value2 = multiplier * qPow(i, derivative);
			matrix->setCosMulti(rowOffset, i + offset + degree, value2, value1);
			matrix->setSinMulti(rowOffset, i + offset, -value2, value1);
		}
	}
	else if(derivative % 4 == 2) // 2nd, 6th, 10th, ... derivatives
	{
		for(i = 1; i <= degree; ++i)
		{
			value1 = x * i;
			value2 = multiplier * (-qPow(i, derivative));
			matrix->setCosMulti(rowOffset, i + offset, value2, value1);
			matrix->setSinMulti(rowOffset, i + offset + degree, value2, value1);
		}
	}
	else if(derivative % 4 == 3) // 3rd, 7th, 1th, ... derivatives
	{
		for(i = 1; i <= degree; ++i)
		{
			value1 = x * i;
			value2 = multiplier * qPow(i, derivative);
			matrix->setCosMulti(rowOffset, i + offset + degree, -value2, value1);
			matrix->setSinMulti(rowOffset, i + offset, value2, value1);
		}
	}
}

void ViFourierInterpolator::splineOffsetLeft(int *offsets, const int &size, const int &coefficientCount)
{
	static int i;
	offsets[0] = 0;
	for(i = 1; i < size; ++i) offsets[i] = (i - 1) * coefficientCount;
}

void ViFourierInterpolator::splineOffsetOutput(int *offsets, const int &size, const int &coefficientCount, const int &start)
{
	static int i;
	for(i = 0; i < size; ++i) offsets[i] = (start - 1) * coefficientCount;
}

void ViFourierInterpolator::splineOffsetRight(int *offsets, const int &size, const int &coefficientCount, const int &start, const int &outputSize)
{
	static int i;
	offsets[0] = start - outputSize;
	for(i = 1; i < size; ++i) offsets[i] = (start + i - 1 - outputSize) * coefficientCount;
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
