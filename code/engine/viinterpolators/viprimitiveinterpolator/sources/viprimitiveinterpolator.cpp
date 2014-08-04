#include <viprimitiveinterpolator.h>
#include <vimath.h>

ViPrimitiveInterpolator::ViPrimitiveInterpolator(const Mode &mode)
	: ViInterpolator()
{
	mMode = mode;
	if(mMode == Zero) interpolatePointer = &ViPrimitiveInterpolator::interpolateZero;
	else if(mMode == Cosine) interpolatePointer = &ViPrimitiveInterpolator::interpolateCosine;
	else if(mMode == Random)
	{
		interpolatePointer = &ViPrimitiveInterpolator::interpolateRandom;
		addParameterName("Window Size");
	}
	else if(mMode == Mirror) interpolatePointer = &ViPrimitiveInterpolator::interpolateMirror;
	else if(mMode == Duplicate) interpolatePointer = &ViPrimitiveInterpolator::interpolateDuplicate;
	else if(mMode == Similarity)
	{
		mSamples = 0;
		addParameterName("Window Size"); // The window used to search for similar samples
		addParameterName("Samples"); // The number of sequential samples that should be similar
		interpolatePointer = &ViPrimitiveInterpolator::interpolateSimilarity;
	}
	else if(mMode == Lanczos)
	{
		addParameterName("Window Size");
		interpolatePointer = &ViPrimitiveInterpolator::interpolateLanczos;
	}
}

QString ViPrimitiveInterpolator::name(QString replace, bool spaced)
{
	QString mode;

	if(mMode == Zero) mode = "Zero";
	else if(mMode == Random) mode = "Random";
	else if(mMode == Cosine) mode = "Cosine";
	else if(mMode == Mirror) mode = "Mirror";
	else if(mMode == Duplicate) mode = "Duplicate";
	else if(mMode == Similarity) mode = "Similarity";
	else if(mMode == Lanczos) mode = "Lanczos";

	QString name = ViInterpolator::name(replace, spaced) + " (" + mode + ")";
	if(spaced) return name;
	else return name.replace(" ", "");
}

void ViPrimitiveInterpolator::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else if(number == 1) mSamples = value;
	else
	{
		LOG("Invalid parameter for this interpolator.", QtCriticalMsg);
		exit(-1);
	}
}

bool ViPrimitiveInterpolator::validParameters()
{
	if(mMode == Similarity)
	{
		if(mSamples > mWindowSize) return false;
	}
	if((mMode == Mirror || mMode == Duplicate || mMode == Similarity) && mDirection != Bidirectional)
	{
		LOG("The primitive interpoaltor using similarities should be executed in bidirectional mode. Changing mode to bidiretional.", QtFatalMsg);
		setDirection(ViInterpolator::Bidirectional);
		//exit(-1);
	}
	return true;
}

bool ViPrimitiveInterpolator::interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel)
{
	(this->*interpolatePointer)(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize, error);
	return true;
}

void ViPrimitiveInterpolator::interpolateZero(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error)
{
	static int i;
	for(i = 0; i < outputSize; ++i) outputSamples[i] = 0;
}

void ViPrimitiveInterpolator::interpolateRandom(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error)
{
	ViMath<qreal>::noise(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize);
}

void ViPrimitiveInterpolator::interpolateCosine(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error)
{
	static qreal currentRatio, ratio;
	static int i;

	ratio = 1.0 / (outputSize + 1);
	for(i = 0; i < outputSize; ++i)
	{
		currentRatio = (1 - cos((ratio * (i + 1)) * M_PI)) / 2;
		outputSamples[i] = (leftSamples[leftSize - 1] * (1 - currentRatio)) + (rightSamples[0] * currentRatio);
	}
}

void ViPrimitiveInterpolator::interpolateMirror(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error)
{
	for(int i = 0; i < outputSize; ++i)
	{
		outputSamples[i] = leftSamples[leftSize - 1 - i];
	}
}

void ViPrimitiveInterpolator::interpolateDuplicate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error)
{
	for(int i = 0; i < outputSize; ++i)
	{
		outputSamples[i] = leftSamples[leftSize - outputSize + i];
	}
}

void ViPrimitiveInterpolator::interpolateSimilarity(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error)
{
	qreal differences[mSamples - 1];
	qreal lastSample = 0;
	qreal bestScore1, bestScore2;
	qreal bestDifference1, bestDifference2;

	for(int i = 0; i < outputSize; ++i)
	{
		calculateDifferences(leftSamples, leftSize,  outputSamples, i, differences, lastSample);

		similarity(leftSamples, leftSize, differences, bestScore1, bestDifference1);
		similarity(rightSamples, rightSize, differences, bestScore2, bestDifference2);

		if(bestScore2 > bestScore1) outputSamples[i] = lastSample + bestDifference2;
		else outputSamples[i] = lastSample + bestDifference1;
	}
}


void ViPrimitiveInterpolator::interpolateLanczos(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error)
{
	static int i, j, a;
	static qreal total, result, scaledX, l, newX, newxPi, scaledA, size, offset;

	offset = leftSize + outputSize;
	size = offset + rightSize;
	a = min(leftSize, rightSize);
	scaledA = min(leftSize, rightSize) / size;

	for(i = 0; i < outputSize; ++i)
	{
		total = 0;
		result = 0;
		scaledX = (leftSize + i) / size;

		// Left
		for(j = leftSize - a; j < leftSize; ++j)
		{
			l = 0;
			newX = scaledX - (j / size);
			if(newX > -scaledA && newX < scaledA)
			{
				newxPi = newX * M_PI;
				l = (sin(newxPi) / newxPi) * (sin(newxPi / scaledA) / (newxPi /scaledA));
				if(!std::isnan(l))
				{
					total += l;
					result += leftSamples[j] * l;
				}
			}
		}

		// Right
		for(int j = 0; j < a; ++j)
		{
			l = 0;
			newX = scaledX - ((j + offset) / size);
			if(newX > -scaledA && newX < scaledA)
			{
				newxPi = newX * M_PI;
				l = (sin(newxPi) / newxPi) * (sin(newxPi / scaledA) / (newxPi /scaledA));
				if(!std::isnan(l))
				{
					total += l;
					result += rightSamples[j] * l;
				}
			}
		}

		outputSamples[i] = result / total;
	}
}

void ViPrimitiveInterpolator::calculateDifferences(const qreal *samples, const int &size,  qreal *outputSamples, const int &outputSize, qreal *differences, qreal &lastSample)
{
	int counter = 0;

	for(int i = size - mSamples + outputSize; i < size - 1; ++i)
	{
		differences[counter] = samples[i] - samples[i + 1];
		++counter;
	}

	if(outputSize > 0)
	{
		if(outputSize < mSamples)
		{
			differences[counter] = samples[size - 1] - outputSamples[0];
			++counter;

			for(int i = 0; i < outputSize - 1; ++i)
			{
				differences[counter] = outputSamples[i] - outputSamples[i + 1];
				++counter;
			}

			if(outputSize <= 1) lastSample = outputSamples[0];
			else lastSample =  outputSamples[outputSize - 1];
		}
		else
		{
			for(int i = outputSize - mSamples; i < outputSize - 1; ++i)
			{
				differences[counter] = outputSamples[i] - outputSamples[i + 1];
				++counter;
			}
			lastSample = outputSamples[outputSize - 1];
		}
	}
	else
	{
		lastSample = samples[size - 1];
	}
}

void ViPrimitiveInterpolator::similarity(const qreal *samples, const int &size, const qreal *differences, qreal &bestScore, qreal &bestDifference)
{
	static int i, j, end1, end2;
	static qreal currentScore;

	bestScore = DBL_MAX;
	bestDifference = 0;
	end1 = size - mSamples;
	end2 = mSamples - 1;

	for(i = 0; i < end1; ++i)
	{
		currentScore = 0;
		for(j = 0; j < end2; ++j)
		{
			currentScore += pow((samples[i + j] - samples[i + j + 1]) - differences[j], 2);
		}
		if(currentScore < bestScore)
		{
			bestScore = currentScore;
			bestDifference = samples[i + end2] - samples[i + mSamples];
		}
	}
}


ViPrimitiveInterpolator* ViPrimitiveInterpolator::clone()
{
	return new ViPrimitiveInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViPrimitiveInterpolator* create()
{
	return new ViPrimitiveInterpolator();
}

#ifdef __cplusplus
}
#endif
