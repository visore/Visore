#include <vifourierpredictor.h>
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

	QString name = ViPredictor::name(replace, spaced) + " (" + mode + ", " + estimation + ")";
	if(spaced) return name;
	else return name.replace(" ", "").replace(",", "_");
}

void ViFourierPredictor::setDegree(const int &degree)
{
	mDegree = degree;
}

void ViFourierPredictor::setDerivatives(const int &derivatives)
{
	if(mMode == Normal) mDerivatives = 0;
	else mDerivatives = derivatives;
}

void ViFourierPredictor::setParameter(const int &number, const qreal &value)
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

void ViFourierPredictor::setType()
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

bool ViFourierPredictor::validParameters()
{
	if(mEstimation == Best) return true;
	return validParameters(mMode, mWindowSize, mDegree, mDerivatives);
}

bool ViFourierPredictor::validParameters(const Mode &mode, const int &windowSize, const int &degree, const int &derivatives)
{
	if(mode == Normal) return windowSize >= (degree * 2) + 1;
	else if(mode == Osculating) return (windowSize >= (degree * 2) + 1) && degree > derivatives;
	else if(mode == Splines)
	{
		if(windowSize <= 1) return false;
		int coefficients = (windowSize - 1) * ((degree * 2) + 1); // For every spline
		int equations = (windowSize - 1) * 2; // Fourier for each spline
		equations += (windowSize - 2) * derivatives; // Derivatives at all intermediate points
		equations += 1; // Set incoming spline (a0) equal to 0
		return equations >= coefficients && degree >= derivatives;
	}
}

void ViFourierPredictor::setPointers(const Mode &mode, const Estimation &estimation)
{
	mMode = mode;
	mEstimation = estimation;

	if(mMode == Normal)
	{
		if(mEstimation == Fixed) predictPointer = &ViFourierPredictor::predictFixed;
		else if(mEstimation == Best) predictPointer = &ViFourierPredictor::predictBestNormal;
		predictModelPointer = &ViFourierPredictor::predictModelNormal;
		estimateModelPointer = &ViFourierPredictor::estimateModelNormal;
	}
	else if(mMode == Osculating)
	{
		if(mEstimation == Fixed) predictPointer = &ViFourierPredictor::predictFixed;
		else if(mEstimation == Best) predictPointer = &ViFourierPredictor::predictBestOsculating;
		predictModelPointer = &ViFourierPredictor::predictModelOsculating;
		estimateModelPointer = &ViFourierPredictor::estimateModelOsculating;
	}
	else if(mMode == Splines)
	{
		if(mEstimation == Fixed) predictPointer = &ViFourierPredictor::predictFixedSplines;
		else if(mEstimation == Best) predictPointer = &ViFourierPredictor::predictBestSplines;
		predictModelPointer = &ViFourierPredictor::predictModelSplines;
		estimateModelPointer = &ViFourierPredictor::estimateModelSplines;
	}
}

bool ViFourierPredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViError *error, const int &channel)
{
	// Scaling because multiplying integers (or powers) might lead to variable overflow
	return (this->*predictPointer)(samples, size, predictedSamples, predictionCount, qreal(size + predictionCount - 1), error);
}

bool ViFourierPredictor::predictFixed(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling, ViError *error)
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

bool ViFourierPredictor::predictFixedSplines(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling, ViError *error)
{
	if(validParameters(mMode, size, mDegree, mDerivatives))
	{
		ViEigenBaseVector *coefficients = (this->*estimateModelPointer)(mDegree, mDerivatives, samples, size, scaling);

		// Use the middel spline for prediction
		int predictionOffset[predictionCount];
		splineOffsetPrediction(predictionOffset, predictionCount, size, (mDegree * 2) + 1);
		(this->*predictModelPointer)(mDegree, coefficients, predictedSamples, predictionCount, size, scaling, predictionOffset);

		if(error != NULL)
		{
			qreal prediction[size];
			int offsets[size];
			splineOffsetModel(offsets, size, (mDegree * 2) + 1);
			(this->*predictModelPointer)(mDegree, coefficients, prediction, size, 0, scaling, offsets);
			error->add(prediction, samples, size);
		}
		mEigen->clear(coefficients);
		return true;
	}
	LOG("Invalid parameter combination detected.", QtCriticalMsg);
	return false;
}

bool ViFourierPredictor::predictBestNormal(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling, ViError *error)
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

bool ViFourierPredictor::predictBestOsculating(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling, ViError *error)
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

bool ViFourierPredictor::predictBestSplines(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling, ViError *error)
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
				splineOffsetModel(offsets, size, (i * 2) + 1);
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
		splineOffsetModel(offsets, size, (bestDegree * 2) + 1);
		(this->*predictModelPointer)(bestDegree, bestCoefficients, prediction, size, 0, scaling, offsets);
		error->add(prediction, samples, size);
	}

	int predictionOffset[predictionCount];
	splineOffsetPrediction(predictionOffset, predictionCount, size, (bestDegree * 2) + 1);
	(this->*predictModelPointer)(bestDegree, bestCoefficients, predictedSamples, predictionCount, size, scaling, predictionOffset);
	mEigen->clear(bestCoefficients);
	return true;
}

ViEigenBaseVector* ViFourierPredictor::estimateModelNormal(const int &degree, const int &derivative, const qreal *samples, const int &size, const qreal &scaling)
{
	static int i, j;
	static qreal x, value;

	ViEigenBaseMatrix *matrix = mEigen->createMatrix(size, (2 * degree) + 1);
	ViEigenBaseVector *vector = mEigen->createVector(size);

	for(i = 0; i < size; ++i)
	{
		vector->set(i, samples[i]);
		x = i * scaling;
		matrix->set(i, 0, 0.5);
		for(j = 1; j <= degree; ++j)
		{
			value = x * j;
			matrix->setCos(i, j, value);
			matrix->setSin(i, j + degree, value);
		}
	}

	ViEigenBaseVector *coefficients = mEigen->estimate(matrix, vector);
	mEigen->clear(matrix);
	mEigen->clear(vector);
	return coefficients;
}

void ViFourierPredictor::predictModelNormal(const int &degree, const ViEigenBaseVector *coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling, const int *offsets)
{
	static int i, j;
	static qreal value, x;

	ViEigenBaseMatrix *matrix = mEigen->createMatrix(size, (2 * degree) + 1);
	for(i = 0; i < size; ++i)
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
	mEigen->solve(coefficients, matrix, prediction, size);
	mEigen->clear(matrix);
}


ViEigenBaseVector* ViFourierPredictor::estimateModelOsculating(const int &degree, const int &derivative, const qreal *samples, const int &size, const qreal &scaling)
{
	static int i, j, offset, start, end, derivativeCount, totalDerivatives, totalSize;
	static qreal x, value;

	derivativeCount = size - 4; // Average, we don't have derivatives for the first and last point for both sides
	totalDerivatives = derivativeCount * derivative;
	totalSize = size + totalDerivatives;

	ViEigenBaseMatrix *matrix = mEigen->createMatrix(totalSize, (2 * degree) + 1);
	ViEigenBaseVector *vector = mEigen->createVector(totalSize);

	for(i = 0; i < size; ++i)
	{
		vector->set(i, samples[i]);
		x = i * scaling;
		matrix->set(i, 0, 0.5);
		for(j = 1; j <= degree; ++j)
		{
			value = x * j;
			matrix->setCos(i, j, value);
			matrix->setSin(i, j + degree, value);
		}
	}

	// Determine for how many samples we cannot calculate a derivitate, and skip those equations.
	// Der1 and der2 must skip 1 sample at the start and 1 at the end, der3 and der4 skip 2, etc
	start = ceil(derivative / 2.0);
	end = size - start;

	qreal derivatives[size];

	for(i = 1; i <= derivative; ++i)
	{
		ViDifferentiater::derivative(i, samples, size, derivatives);
		for(j = start; j < end; ++j)
		{
			offset = size + (derivativeCount * (i - 1)) + (j - 1);
			vector->set(offset, derivatives[j]);
			calculateDerivative(degree, j * scaling, matrix, offset, i);
		}
	}

	ViEigenBaseVector *coefficients = mEigen->estimate(matrix, vector);
	mEigen->clear(matrix);
	mEigen->clear(vector);
	return coefficients;
}

void ViFourierPredictor::predictModelOsculating(const int &degree, const ViEigenBaseVector *coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling, const int *offsets)
{
	static int i, j;
	static qreal value, x;

	ViEigenBaseMatrix *matrix = mEigen->createMatrix(size, (2 * degree) + 1);
	for(i = 0; i < size; ++i)
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
	mEigen->solve(coefficients, matrix, prediction, size);
	mEigen->clear(matrix);
}

ViEigenBaseVector* ViFourierPredictor::estimateModelSplines(const int &degree, const int &derivative, const qreal *samples, const int &size, const qreal &scaling)
{
	static int i, j, extraEquations, columnOffset1, columnOffset2, rowOffset1, rowOffset2, splineCount, singleCoefficientCount, coefficientCount, derivativeCount, equationCount;
	static qreal x1, x2, value1, value2;

	if(size <= 1) return NULL;

	splineCount = size - 1;
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
	for(i = 0; i < splineCount; ++i)
	{
		rowOffset1 = i * 2;
		rowOffset2 = rowOffset1 + 1;
		columnOffset1 = singleCoefficientCount * i;
		columnOffset2 = columnOffset1 + degree;

		vector->set(rowOffset1, samples[i]);
		vector->set(rowOffset2, samples[i + 1]);

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

	// Add the deratives between neighbouring left splines
	for(i = 1; i <= derivative; ++i)
	{
		rowOffset1 = (splineCount * 2) + ((i - 1) * splineCount);
		for(j = 0; j < splineCount; ++j)
		{
			rowOffset2 = rowOffset1 + j;
			columnOffset1 = j * singleCoefficientCount;
			x1 = (j + 1) * scaling;
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

void ViFourierPredictor::predictModelSplines(const int &degree, const ViEigenBaseVector *coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling, const int *offsets)
{
	static int i, j;
	static qreal x, value;

	ViEigenBaseMatrix *matrix = mEigen->createMatrix(size, (2 * degree) + 1);
	for(i = 0; i < size; ++i)
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

	mEigen->solve(coefficients, matrix, prediction, size, offsets);
	mEigen->clear(matrix);
}

void ViFourierPredictor::calculateDerivative(const int &degree, const qreal &x, ViEigenBaseMatrix *matrix, const int &rowOffset, const int &derivative)
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

void ViFourierPredictor::calculateDerivative(const int &degree, const qreal &x, ViEigenBaseMatrix *matrix, const int &rowOffset, const int &derivative, const int &offset, const int multiplier)
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

void ViFourierPredictor::splineOffsetModel(int *offsets, const int &size, const int &coefficientCount)
{
	static int i;
	offsets[0] = 0;
	for(i = 1; i < size; ++i) offsets[i] = (i - 1) * coefficientCount;
}

void ViFourierPredictor::splineOffsetPrediction(int *offsets, const int &predictionCount, const int &size, const int &coefficientCount)
{
	static int i, offset;
	offset = (size - 2) * coefficientCount;
	for(i = 0; i < predictionCount; ++i) offsets[i] = offset;
}

/*#include <vifourierpredictor.h>
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

	QString name = ViPredictor::name(replace, spaced) + " (" + mode + ", " + estimation + ")";
	if(spaced) return name;
	else return name.replace(" ", "").replace(",", "_");
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
		int equations = (windowSize - 1) * 2; // Fourier for each spline
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

bool ViFourierPredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViErrorCollection *modelErrors)
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
	// http://en.wikipedia.org/wiki/Trigonometric_Fourier

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
			vector[offset] = ViDifferentiater::derivative(i, samples, size, j, error);
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
	static qreal x1, x2, value1, value2;
	static bool addFirstSpline;

	splineCount = (size - 1);
	singleCoefficientCount = ((degree * 2) + 1);
	coefficientCount = splineCount * singleCoefficientCount;
	derivativeCount = splineCount - 1;

	addFirstSpline = false;

	equationCount = 0;
	equationCount += splineCount * 2; // Spline Fouriers. Times 2 since we use the spline with 2 points
	equationCount += derivativeCount * derivative; // Intermediate derivatives should be equal;
	if(equationCount < coefficientCount) // Add first spline == 0
	{
		++equationCount;
		addFirstSpline = true;
	}

	ViMatrix matrix(equationCount, coefficientCount);
	ViVector vector(equationCount);

	// Add spline Fouriers
	for(i = 0; i < splineCount; ++i)
	{
		rowOffset1 = i * 2;
		rowOffset2 = rowOffset1 + 1;
		columnOffset1 = singleCoefficientCount * i;
		columnOffset2 = columnOffset1 + degree;

		vector[rowOffset1] = samples[i];
		vector[rowOffset2] = samples[i + 1];

		x1 = scaling * i;
		x2 = scaling * (i + 1);

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

	// Add the deratives between neighbouring splines
	for(i = 1; i <= derivative; ++i)
	{
		rowOffset1 = (splineCount * 2) + ((i - 1) * derivative);
		for(j = 0; j < derivativeCount; ++j)
		{
			rowOffset2 = rowOffset1 + j;
			columnOffset1 = j * singleCoefficientCount;
			x1 = scaling * (j + 1);
			calculateDerivative(degree, x1, matrix[rowOffset2], i, columnOffset1, 1);
			calculateDerivative(degree, x1, matrix[rowOffset2], i, columnOffset1 + singleCoefficientCount, -1); // -1: take the derivative on the right-hand side to the left of the equation
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
*/
