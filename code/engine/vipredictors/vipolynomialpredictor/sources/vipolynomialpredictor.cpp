#include <vipolynomialpredictor.h>
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
	if(mMode == Osculating) addParameterName("Derivatives");
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
}

void ViPolynomialPredictor::setDerivatives(const int &derivatives)
{
	if(mMode == Normal) mDerivatives = 0;
	else mDerivatives = derivatives;
}

void ViPolynomialPredictor::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else if(number == 1) setDegree(value);
	else if(number == 2) setDerivatives(value);
	else
	{
		LOG("Invalid parameter for this Predictor.", QtCriticalMsg);
		exit(-1);
	}
	setType();
}

void ViPolynomialPredictor::setType()
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

bool ViPolynomialPredictor::validParameters()
{
	if(mEstimation == Best) return true;
	return validParameters(mMode, mWindowSize, mDegree, mDerivatives);
}

bool ViPolynomialPredictor::validParameters(const Mode &mode, const int &windowSize, const int &degree, const int &derivatives)
{
	if(mode == Normal) return windowSize >= degree + 1;
	else if(mode == Osculating) return windowSize >= degree + 1 && degree > derivatives && derivatives <= (windowSize / 2.0);
	else if(mode == Splines) return windowSize > 1;
}

void ViPolynomialPredictor::setPointers(const Mode &mode, const Estimation &estimation)
{
	mMode = mode;
	mEstimation = estimation;

	if(mMode == Normal)
	{
		if(mEstimation == Fixed) predictPointer = &ViPolynomialPredictor::predictFixed;
		else if(mEstimation == Best) predictPointer = &ViPolynomialPredictor::predictBestNormal;
		predictModelPointer = &ViPolynomialPredictor::predictModelNormal;
		estimateModelPointer = &ViPolynomialPredictor::estimateModelNormal;
	}
	else if(mMode == Osculating)
	{
		if(mEstimation == Fixed) predictPointer = &ViPolynomialPredictor::predictFixed;
		else if(mEstimation == Best) predictPointer = &ViPolynomialPredictor::predictBestOsculating;
		predictModelPointer = &ViPolynomialPredictor::predictModelOsculating;
		estimateModelPointer = &ViPolynomialPredictor::estimateModelOsculating;
	}
	else if(mMode == Splines)
	{
		if(mEstimation == Fixed) predictPointer = &ViPolynomialPredictor::predictFixedSplines;
		else if(mEstimation == Best) predictPointer = &ViPolynomialPredictor::predictBestSplines;
		predictModelPointer = &ViPolynomialPredictor::predictModelSplines;
		estimateModelPointer = &ViPolynomialPredictor::estimateModelSplines;
	}
}

bool ViPolynomialPredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViError *error, const int &channel)
{
	// Scaling because multiplying integers (or powers) might lead to variable overflow
	return (this->*predictPointer)(samples, size, predictedSamples, predictionCount, qreal(size + predictionCount - 1), error);
}

bool ViPolynomialPredictor::predictFixed(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling, ViError *error)
{
	if(validParameters(mMode, size, mDegree, mDerivatives))
	{
		ViEigenBaseVector *coefficients = (this->*estimateModelPointer)(mDegree, mDerivatives, samples, size, scaling);
		(this->*predictModelPointer)(mDegree, coefficients, predictedSamples, predictionCount, size, scaling, NULL);
		if(error != NULL)
		{
			qreal prediction[size];
			(this->*predictModelPointer)(mDegree, coefficients, prediction, size, 0, scaling, NULL);
			error->add(prediction, samples, size);
		}
		mEigen->clear(coefficients);
		return true;
	}
	LOG("Invalid parameter combination detected.", QtCriticalMsg);
	return false;
}

bool ViPolynomialPredictor::predictFixedSplines(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling, ViError *error)
{
	if(validParameters(mMode, size, mDegree, mDerivatives))
	{
		ViEigenBaseVector *coefficients = (this->*estimateModelPointer)(mDegree, mDerivatives, samples, size, scaling);

		// Use the middel spline for prediction
		int predictionOffset[predictionCount];
		splineOffsetPrediction(predictionOffset, predictionCount, size, mDegree + 1);
		(this->*predictModelPointer)(mDegree, coefficients, predictedSamples, predictionCount, size, scaling, predictionOffset);

		if(error != NULL)
		{
			qreal prediction[size];
			int offsets[size];
			splineOffsetModel(offsets, size, mDegree + 1);
			(this->*predictModelPointer)(mDegree, coefficients, prediction, size, 0, scaling, offsets);
			error->add(prediction, samples, size);
		}
		mEigen->clear(coefficients);
		return true;
	}
	LOG("Invalid parameter combination detected.", QtCriticalMsg);
	return false;
}

bool ViPolynomialPredictor::predictBestNormal(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling, ViError *error)
{
	static int i, bestDegree;
	static qreal currentMse, bestMse;

	ViEigenBaseVector *bestCoefficients = NULL;

	qreal prediction[size];
	bestMse = DBL_MAX;
	bestDegree = 1;

	for(i = 1; i <= mDegree; ++i)
	{
		if(validParameters(mMode, size, i, 0))
		{
			ViEigenBaseVector *currentCoefficients = (this->*estimateModelPointer)(i, 0, samples, size, scaling);
			(this->*predictModelPointer)(i, currentCoefficients, prediction, size, 0, scaling, NULL);
			currentMse = ViError::calculateMse(prediction, samples, size);
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
		(this->*predictModelPointer)(bestDegree, bestCoefficients, prediction, size, 0, scaling, NULL);
		error->add(prediction, samples, size);
	}

	(this->*predictModelPointer)(bestDegree, bestCoefficients, predictedSamples, predictionCount, size, scaling, NULL);
	mEigen->clear(bestCoefficients);

	return true;
}

bool ViPolynomialPredictor::predictBestOsculating(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling, ViError *error)
{
	static int i, j, bestDegree;
	static qreal currentMse, bestMse;
	static bool wasBad, wasBadAgain;

	ViEigenBaseVector *bestCoefficients = NULL;

	qreal prediction[size];

	bestMse = DBL_MAX;
	bestDegree = 1;
	wasBad = false;
	wasBadAgain = false;

	for(i = 1; i <= mDegree; ++i)
	{
		for(j = 1; j <= mDerivatives; ++j)
		{
			if(validParameters(mMode, size, i, j))
			{
				ViEigenBaseVector *currentCoefficients = (this->*estimateModelPointer)(i, j, samples, size, scaling);
				(this->*predictModelPointer)(i, currentCoefficients, prediction, size, 0, scaling, NULL);
				currentMse = ViError::calculateMse(prediction, samples, size);
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
		(this->*predictModelPointer)(bestDegree, bestCoefficients, prediction, size, 0, scaling, NULL);
		error->add(prediction, samples, size);
	}

	(this->*predictModelPointer)(bestDegree, bestCoefficients, predictedSamples, predictionCount, size, scaling, NULL);
	mEigen->clear(bestCoefficients);

	return true;
}

bool ViPolynomialPredictor::predictBestSplines(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling, ViError *error)
{
	static int i, j, bestDegree;
	static qreal currentMse, bestMse;
	static bool wasBad, wasBadAgain;

	ViEigenBaseVector *bestCoefficients = NULL;

	qreal prediction[size];

	bestMse = DBL_MAX;
	bestDegree = 1;
	wasBad = false;
	wasBadAgain = false;

	int offsets[size];

	for(i = 1; i <= mDegree; ++i)
	{
		for(j = 1; j <= mDerivatives; ++j)
		{
			if(validParameters(mMode, size, i, j))
			{
				ViEigenBaseVector *currentCoefficients = (this->*estimateModelPointer)(i, j, samples, size, scaling);
				splineOffsetModel(offsets, size, i + 1);
				(this->*predictModelPointer)(i, currentCoefficients, prediction, size, 0, scaling, offsets);
				currentMse = ViError::calculateMse(prediction, samples, size);
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
		splineOffsetModel(offsets, size, bestDegree + 1);
		(this->*predictModelPointer)(bestDegree, bestCoefficients, prediction, size, 0, scaling, offsets);
		error->add(prediction, samples, size);
	}

	int predictionOffset[predictionCount];
	splineOffsetPrediction(predictionOffset, predictionCount, size, bestDegree + 1);
	(this->*predictModelPointer)(bestDegree, bestCoefficients, predictedSamples, predictionCount, size, scaling, predictionOffset);
	mEigen->clear(bestCoefficients);
	return true;
}

ViEigenBaseVector* ViPolynomialPredictor::estimateModelNormal(const int &degree, const int &derivative, const qreal *samples, const int &size, const qreal &scaling)
{
	static int i, j;
	static qreal x;

	ViEigenBaseMatrix *matrix = mEigen->createMatrix(size, degree + 1);
	ViEigenBaseVector *vector = mEigen->createVector(size);

	for(i = 0; i < size; ++i)
	{
		vector->set(i, samples[i]);
		x = i / scaling;
		matrix->set(i, 0, 1);
		matrix->set(i, 1, x);
		for(j = 2; j <= degree; ++j) matrix->setPower(i, j, x, j);
	}

	ViEigenBaseVector *coefficients = mEigen->estimate(matrix, vector);
	mEigen->clear(matrix);
	mEigen->clear(vector);
	return coefficients;
}

void ViPolynomialPredictor::predictModelNormal(const int &degree, const ViEigenBaseVector *coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling, const int *offsets)
{
	static int i, j;
	static qreal x;

	ViEigenBaseMatrix *matrix = mEigen->createMatrix(size, degree + 1);
	for(i = 0; i < size; ++i)
	{
		x = (start + i) / scaling;
		matrix->set(i, 0, 1);
		matrix->set(i, 1, x);
		for(j = 2; j <= degree; ++j) matrix->setPower(i, j, x, j);
	}

	mEigen->solve(coefficients, matrix, prediction, size);
	mEigen->clear(matrix);
}


ViEigenBaseVector* ViPolynomialPredictor::estimateModelOsculating(const int &degree, const int &derivative, const qreal *samples, const int &size, const qreal &scaling)
{
	static int i, j, offset, start, end, derivativeCount, totalDerivatives, totalSize;
	static qreal x;

	derivativeCount = size - 2; // Average, we don't have derivatives for the first and last points
	totalDerivatives = derivativeCount * derivative;
	totalSize = size + totalDerivatives;

	ViEigenBaseMatrix *matrix = mEigen->createMatrix(totalSize, degree + 1);
	ViEigenBaseVector *vector = mEigen->createVector(totalSize);

	for(i = 0; i < size; ++i)
	{
		vector->set(i, samples[i]);
		x = i / scaling;
		matrix->set(i, 0, 1);
		matrix->set(i, 1, x);
		for(j = 2; j <= degree; ++j) matrix->setPower(i, j, x, j);
	}

	qreal derivatives[size];

	// Determine for how many samples we cannot calculate a derivitate, and skip those equations.
	// Der1 and der2 must skip 1 sample at the start and 1 at the end, der3 and der4 skip 2, etc
	start = ceil(derivative / 2.0);
	end = size - start;

	for(i = 1; i <= derivative; ++i)
	{
		ViDifferentiater::derivative(i, samples, size, derivatives);
		for(j = start; j < end; ++j)
		{
			offset = size + (derivativeCount * (i - 1)) + (j - 1);
			vector->set(offset, derivatives[j]);
			calculateDerivative(degree, j / scaling, matrix, offset, i);
		}
	}

	ViEigenBaseVector *coefficients = mEigen->estimate(matrix, vector);
	mEigen->clear(matrix);
	mEigen->clear(vector);
	return coefficients;
}

void ViPolynomialPredictor::predictModelOsculating(const int &degree, const ViEigenBaseVector *coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling, const int *offsets)
{
	static int i, j;
	static qreal x;

	ViEigenBaseMatrix *matrix = mEigen->createMatrix(size, degree + 1);
	for(i = 0; i < size; ++i)
	{
		x = (start + i) / scaling;
		matrix->set(i, 0, 1);
		matrix->set(i, 1, x);
		for(j = 2; j <= degree; ++j) matrix->setPower(i, j, x, j);
	}
	mEigen->solve(coefficients, matrix, prediction, size);
	mEigen->clear(matrix);
}

ViEigenBaseVector* ViPolynomialPredictor::estimateModelSplines(const int &degree, const int &derivative, const qreal *samples, const int &size, const qreal &scaling)
{
	static int i, j, lastDerivative, lastDerivativeCounter, extraEquations, columnOffset, rowOffset1, rowOffset2, splineCount, singleCoefficientCount, coefficientCount, derivativeCount, equationCount;
	static qreal x1, x2;

	if(size <= 1) return NULL;

	splineCount = size - 1;
	singleCoefficientCount = degree + 1;
	coefficientCount = splineCount * singleCoefficientCount;
	derivativeCount = degree - 1;

	extraEquations = 0;	
	equationCount = 0;
	equationCount += splineCount * 2; // Spline polynomials. Times 2 since we use the spline with 2 points
	equationCount += derivativeCount * (splineCount - 1); // Intermediate derivatives should be equal;
	while(equationCount < coefficientCount) // Add first spline == 0
	{
		++equationCount;
		++extraEquations;
	}

	ViEigenBaseMatrix *matrix = mEigen->createMatrix(equationCount, coefficientCount);
	ViEigenBaseVector *vector = mEigen->createVector(equationCount);

	// Add left spline polynomials
	for(i = 0; i < splineCount; ++i)
	{
		rowOffset1 = i * 2;
		rowOffset2 = rowOffset1 + 1;
		columnOffset = singleCoefficientCount * i;

		vector->set(rowOffset1, samples[i]);
		vector->set(rowOffset2, samples[i + 1]);

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

	// Add the deratives between neighbouring left splines
	for(i = 1; i <= derivativeCount; ++i)
	{
		rowOffset1 = (splineCount * 2) + ((i - 1) * splineCount);
		for(j = 0; j < splineCount; ++j)
		{
			rowOffset2 = rowOffset1 + j;
			columnOffset = j * singleCoefficientCount;
			x1 = (j + 1) / scaling;
			calculateDerivative(degree, x1, matrix, rowOffset2, i, columnOffset, 1);
			calculateDerivative(degree, x1, matrix, rowOffset2, i, columnOffset + singleCoefficientCount, -1); // -1: take the derivative on the right-hand side to the left of the equation
		}
	}

	// Set the derivitives for the first/last spline at point 0 to 0
	lastDerivative = 1;
	lastDerivativeCounter = 0;
	while(extraEquations > 0)
	{
		if(lastDerivativeCounter) calculateDerivative(degree, 1, matrix, equationCount - extraEquations, degree - lastDerivative, coefficientCount - singleCoefficientCount, 1);
		else calculateDerivative(degree, 0, matrix, equationCount - extraEquations, degree - lastDerivative, 0, 1);
		--extraEquations;
		++lastDerivativeCounter;
		if(lastDerivativeCounter == 2)
		{
			++lastDerivative;
			lastDerivativeCounter = 0;
		}
	}

	ViEigenBaseVector *coefficients = mEigen->estimate(matrix, vector);

	mEigen->clear(matrix);
	mEigen->clear(vector);
	return coefficients;
}

void ViPolynomialPredictor::predictModelSplines(const int &degree, const ViEigenBaseVector *coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling, const int *offsets)
{
	static int i, j;
	static qreal x;

	ViEigenBaseMatrix *matrix = mEigen->createMatrix(size, degree + 1);
	for(i = 0; i < size; ++i)
	{
		x = (start + i) / scaling;
		matrix->set(i, 0, 1);
		matrix->set(i, 1, x);
		for(j = 2; j <= degree; ++j) matrix->setPower(i, j, x, j);
	}
	mEigen->solve(coefficients, matrix, prediction, size, offsets);
	mEigen->clear(matrix);
}

void ViPolynomialPredictor::calculateDerivative(const int &degree, const qreal &x, ViEigenBaseMatrix *matrix, const int &rowOffset, const int &derivative)
{
	static int i, j;
	static qreal factorial;

	for(i = 0; i < derivative; ++i) matrix->set(rowOffset, i, 0);

	factorial = derivative;
	for(i = 1; i < derivative; ++i) factorial *= derivative - i;
	matrix->set(rowOffset, derivative, factorial);

	for(i = derivative + 1; i <= degree; ++i)
	{
		factorial = i;
		for(j = 1; j < derivative; ++j) factorial *= i - j;
		matrix->setPowerMulti(rowOffset, i, factorial, x, i - derivative);
	}
}

void ViPolynomialPredictor::calculateDerivative(const int &degree, const qreal &x, ViEigenBaseMatrix *matrix, const int &rowOffset, const int &derivative, const int &offset, const int multiplier)
{
	static int i, j;
	static qreal factorial;

	for(i = 0; i < derivative; ++i) matrix->set(rowOffset, i + offset, 0);

	factorial = derivative;
	for(i = 1; i < derivative; ++i) factorial *= derivative - i;
	matrix->set(rowOffset, derivative + offset, factorial * multiplier);

	for(i = derivative + 1; i <= degree; ++i)
	{
		factorial = i;
		for(j = 1; j < derivative; ++j) factorial *= i - j;
		matrix->setPowerMulti(rowOffset, i + offset, multiplier * factorial, x, i - derivative);
	}
}

void ViPolynomialPredictor::splineOffsetModel(int *offsets, const int &size, const int &coefficientCount)
{
	static int i;
	offsets[0] = 0;
	for(i = 1; i < size; ++i) offsets[i] = (i - 1) * coefficientCount;
}


void ViPolynomialPredictor::splineOffsetPrediction(int *offsets, const int &predictionCount, const int &size, const int &coefficientCount)
{
	static int i, offset;
	offset = (size - 2) * coefficientCount;
	for(i = 0; i < predictionCount; ++i) offsets[i] = offset;
}
