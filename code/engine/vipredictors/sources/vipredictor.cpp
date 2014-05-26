#include <vipredictor.h>
#include <viaudiodata.h>
#include <vilogger.h>
#include <float.h>

#define DEFAULT_WINDOW_SIZE 2048

ViPredictor::ViPredictor()
{
	setWindowSize(DEFAULT_WINDOW_SIZE);
}

ViPredictor::ViPredictor(const ViPredictor &other)
{
	mWindowSize = other.mWindowSize;
	mOffset = other.mOffset;
	mParameterNames = other.mParameterNames;
}

ViPredictor::~ViPredictor()
{

}

void ViPredictor::setWindowSize(const int &size)
{
	mWindowSize = size;
	setOffset(mWindowSize);
}

int ViPredictor::windowSize()
{
	return mWindowSize;
}

void ViPredictor::setOffset(const int &offset)
{
	mOffset = offset;
}

int ViPredictor::offset()
{
	return mOffset;
}

void ViPredictor::predict(ViBuffer *input, ViBuffer *output)
{
	qreal rmse[1];
	predict(input, output, 1, rmse);
}

void ViPredictor::predict(ViBuffer *input, ViBuffer *output, const int &predictionCount, qreal *rmse)
{
	int i, end, newCount;

	end = mWindowSize + predictionCount;
	for(i = 0; i < predictionCount; ++i) rmse[i] = 0;
	qint64 rmseCount = 0;

	ViAudioReadData inputData(input);
	ViAudioWriteData outputData(output);
	// Times 2 so that each channel has mWindowSize data
	inputData.setSampleCount(mWindowSize * 2);
	outputData.setSampleCount(mWindowSize * 2);

	ViSampleChunk outputSamples(mWindowSize);
	QVector<qreal> cache1, cache2;
	QList<qreal> output1, output2;
	qreal predictedSamples[predictionCount];
	int offset;

	ViSampleChunk offsetData(mOffset);
	for(i = 0; i < mOffset; ++i) offsetData[i] = 0;
	outputData.enqueueSplitSamples(offsetData, 0);
	outputData.enqueueSplitSamples(offsetData, 1);

	qint64 processedSamples = 0, totalSamples = inputData.bufferSamples();

	while(inputData.hasData())
	{
		inputData.read();
		ViSampleChunk &data1 = inputData.splitSamples(0);
		ViSampleChunk &data2 = inputData.splitSamples(1);

		offset = cache1.size();
		cache1.resize(offset + data1.size());
		for(i = 0; i < data1.size(); ++i) cache1[offset + i] = data1[i];

		while(cache1.size() > end)
		{
			if(predict(cache1.data(), mWindowSize, predictedSamples, predictionCount))
			{
				for(i = 0; i < predictionCount; ++i)
				{
					adjustValue(predictedSamples[i]);
					rmse[i] += qPow(predictedSamples[i] - cache1[mWindowSize + i], 2);
				}
				++rmseCount;
			}
			else
			{
				for(i = 0; i < predictionCount; ++i)
				{
					predictedSamples[i] = 0;
					rmse[i] += 4; // (-1 -1)^2
				}
				++rmseCount;
			}



			output1.append(predictedSamples[0]);
			if(output1.size() >= mWindowSize)
			{
				for(i = 0; i < mWindowSize; ++i) outputSamples[i] = output1[i];
				output1.clear();
				outputData.enqueueSplitSamples(outputSamples, 0);
			}
			cache1.removeFirst();
		}

		offset = cache2.size();
		cache2.resize(offset + data2.size());
		for(i = 0; i < data2.size(); ++i) cache2[offset + i] = data2[i];

		while(cache2.size() > end)
		{
			if(predict(cache2.data(), mWindowSize, predictedSamples, predictionCount))
			{
				for(i = 0; i < predictionCount; ++i)
				{
					adjustValue(predictedSamples[i]);
					rmse[i] += qPow(predictedSamples[i] - cache2[mWindowSize + i], 2);
				}
				++rmseCount;
			}
			else
			{
				for(i = 0; i < predictionCount; ++i)
				{
					predictedSamples[i] = 0;
					rmse[i] += 4; // (-1 -1)^2
				}
				++rmseCount;
			}


			output2.append(predictedSamples[0]);
			if(output2.size() >= mWindowSize)
			{
				for(i = 0; i < mWindowSize; ++i) outputSamples[i] = output2[i];
				output2.clear();
				outputData.enqueueSplitSamples(outputSamples, 1);
			}
			cache2.removeFirst();
		}

		processedSamples += data1.size() + data2.size();
		setProgress((100.0 * processedSamples) / totalSamples);
	}

	newCount = predictionCount - 1;
	while(cache1.size() > mWindowSize)
	{
		if(predict(cache1.data(), mWindowSize, predictedSamples, newCount))
		{
			for(i = 0; i < newCount; ++i)
			{
				adjustValue(predictedSamples[i]);
				rmse[i] += qPow(predictedSamples[i] - cache1[mWindowSize + i], 2);
			}
			++rmseCount;
		}
		else
		{
			for(i = 0; i < predictionCount; ++i)
			{
				predictedSamples[i] = 0;
				rmse[i] += 4; // (-1 -1)^2
			}
			++rmseCount;
		}


		output1.append(predictedSamples[0]);
		cache1.removeFirst();
		--newCount;
	}

	newCount = predictionCount - 1;
	while(cache2.size() > mWindowSize)
	{
		if(predict(cache2.data(), mWindowSize, predictedSamples, newCount))
		{
			for(i = 0; i < newCount; ++i)
			{
				adjustValue(predictedSamples[i]);
				rmse[i] += qPow(predictedSamples[i] - cache2[mWindowSize + i], 2);
			}
			++rmseCount;
		}
		else
		{
			for(i = 0; i < predictionCount; ++i)
			{
				predictedSamples[i] = 0;
				rmse[i] += 4; // (-1 -1)^2
			}
			++rmseCount;
		}

		output2.append(predictedSamples[0]);
		cache2.removeFirst();
		--newCount;
	}

	outputSamples.resize(output1.size());
	for(i = 0; i < output1.size(); ++i) outputSamples[i] = output1[i];
	outputData.enqueueSplitSamples(outputSamples, 0);

	outputSamples.resize(output2.size());
	for(i = 0; i < output2.size(); ++i) outputSamples[i] = output2[i];
	outputData.enqueueSplitSamples(outputSamples, 1);

	for(i = 0; i < predictionCount; ++i) rmse[i] = qSqrt(rmse[i] / rmseCount); // Devided 2, since samples are in the range [-1,1] (aka RMSE is [0, 2]) and we want the RMSE to be in [0,1]
}

void ViPredictor::adjustValue(qreal &value)
{
	if(value > 1) value = 1;
	else if (value < -1) value = -1;
}

void ViPredictor::setParameter(const int &number, const qreal &value)
{
	LOG("No parameters implemented for this predictor.", QtCriticalMsg);
	exit(-1);
}

void ViPredictor::setParameter(const QString &name, const qreal &value)
{
	QString newName = name.trimmed().toLower().replace(" ", "");
	for(int i = 0; i < mParameterNames.size(); ++i)
	{
		QString currentName = mParameterNames[i].trimmed().toLower().replace(" ", "");
		if(newName == currentName)
		{
			setParameter(i, value);
			return;
		}
	}
}

void ViPredictor::setParameters(const qreal &parameter1)
{
	if(mParameterNames.size() < 1)
	{
		LOG("Invalid parameter count for this predictor.", QtCriticalMsg);
		exit(-1);
	}
	setParameter(0, parameter1);
}

void ViPredictor::setParameters(const qreal &parameter1, const qreal &parameter2)
{
	if(mParameterNames.size() < 2)
	{
		LOG("Invalid parameter count for this predictor.", QtCriticalMsg);
		exit(-1);
	}
	setParameter(0, parameter1);
	setParameter(1, parameter2);
}

void ViPredictor::setParameters(const qreal &parameter1, const qreal &parameter2, const qreal &parameter3)
{
	if(mParameterNames.size() < 3)
	{
		LOG("Invalid parameter count for this predictor.", QtCriticalMsg);
		exit(-1);
	}
	setParameter(0, parameter1);
	setParameter(1, parameter2);
	setParameter(2, parameter3);
}

void ViPredictor::setParameters(const qreal &parameter1, const qreal &parameter2, const qreal &parameter3, const qreal &parameter4)
{
	if(mParameterNames.size() < 4)
	{
		LOG("Invalid parameter count for this predictor.", QtCriticalMsg);
		exit(-1);
	}
	setParameter(0, parameter1);
	setParameter(1, parameter2);
	setParameter(2, parameter3);
	setParameter(3, parameter4);
}

void ViPredictor::setParameters(const qreal &parameter1, const qreal &parameter2, const qreal &parameter3, const qreal &parameter4, const qreal &parameter5)
{
	if(mParameterNames.size() < 5)
	{
		LOG("Invalid parameter count for this predictor.", QtCriticalMsg);
		exit(-1);
	}
	setParameter(0, parameter1);
	setParameter(1, parameter2);
	setParameter(2, parameter3);
	setParameter(3, parameter4);
	setParameter(4, parameter5);
}

QString ViPredictor::parameterName(const int &index, const bool &allCaps)
{
	if(index >= mParameterNames.size()) return "";
	if(allCaps) return mParameterNames[index].toUpper();
	return mParameterNames[index];
}

void ViPredictor::addParameterName(const QString &name)
{
	mParameterNames.append(name);
}

ViModelPredictor::ViModelPredictor(const int &degreeCount, const Estimation &estimation)
	: ViPredictor()
{
	mDegrees.resize(degreeCount);
	mDegrees.fill(0);
	mBestDegrees.resize(degreeCount);

	mEstimation = estimation;
	if(mEstimation == Fixed) predictPointer = &ViModelPredictor::predictFixed;
	else if(mEstimation == Best) predictPointer = &ViModelPredictor::predictBest;
}

ViModelPredictor::ViModelPredictor(const ViModelPredictor &other)
	: ViPredictor(other)
{
	mDegrees = other.mDegrees;
	mBestDegrees = other.mBestDegrees;

	mEstimation = other.mEstimation;
	if(mEstimation == Fixed) predictPointer = &ViModelPredictor::predictFixed;
	else if(mEstimation == Best) predictPointer = &ViModelPredictor::predictBest;
}

ViModelPredictor::~ViModelPredictor()
{
}

void ViModelPredictor::setDegree(const int &degree, const int &degreeIndex)
{
	mDegrees[degreeIndex] = degree;
	mBestDegrees[degreeIndex].clear();
	mBestDegrees[degreeIndex].resize(degree);
	mBestDegrees[degreeIndex].fill(0);
}

int ViModelPredictor::degree(const int &degreeIndex)
{
	return mDegrees[degreeIndex];
}

QVector<int> ViModelPredictor::bestDegrees(const int &degreeIndex)
{
	return mBestDegrees[degreeIndex];
}

int ViModelPredictor::bestDegrees(const int &degree, const int &degreeIndex)
{
	return mBestDegrees[degreeIndex][degree];
}

void ViModelPredictor::increaseBestDegrees(const int &degree)
{
	increaseBestDegrees(0, degree);
}

void ViModelPredictor::increaseBestDegrees(const int &degreeIndex, const int &degree)
{
	mBestDegrees[degreeIndex][degree] += 1;
}

bool ViModelPredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount)
{
	(this->*predictPointer)(samples, size, predictedSamples, predictionCount);
	return true;
}

void ViModelPredictor::predictFixed(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount)
{
	static int i;
	static ViVector coefficients;
	if(estimateModel(degree(), coefficients, samples, size)) predictModel(degree(), coefficients, predictedSamples, predictionCount, size);
	else for(i = 0; i < predictionCount; ++i) predictedSamples[i] = 0;

	/*
	// Recursive prediction
	static int i, index;
	static ViVector coefficients;

	qreal newSamples[size + predictionCount];
	for(i = 0; i < size; ++i) newSamples[i] = samples[i];

	for(i = 0; i < predictionCount; ++i)
	{
		index = size + i;
		if(estimateModel(degree(), coefficients, newSamples, index)) predictModel(degree(), coefficients, predictedSamples + i, 1, index);
		else predictedSamples[i] = 0;
		newSamples[index] = predictedSamples[i];
	}*/
}

void ViModelPredictor::predictBest(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount)
{
	static int i;
	static ViVector currentCoefficients, bestCoefficients;
	static qreal currentScore, bestScore;

	qreal prediction[size];
	bestScore = DBL_MAX;
	for(i = 1; i <= degree(); ++i)
	{
		if(estimateModel(i, currentCoefficients, samples, size))
		{
			predictModel(i, currentCoefficients, prediction, size, 0);
			currentScore = calculateScore(samples, prediction, size);
			if(currentScore < bestScore)
			{
				bestScore = currentScore;
				bestCoefficients = currentCoefficients;
			}
			else break;
		}
	}
	--i; // Important: last iteration will increase i before exiting the loop
	increaseBestDegrees(i - 1);
	predictModel(i, bestCoefficients, predictedSamples, predictionCount, size);
}

bool ViModelPredictor::estimateModel(const int &degree, ViVector &coefficients, const qreal *samples, const int &size)
{
}

void ViModelPredictor::predictModel(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start)
{
}

qreal ViModelPredictor::calculateScore(const qreal *observed, const qreal *predicted, const int &size)
{
	static int i;
	static qreal mse;
	mse = 0;
	for(i = 0; i < size; ++i) mse += qPow(predicted[i] - observed[i], 2);
	return mse / size;
}
