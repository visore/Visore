#include <vipredictor.h>
#include <viaudiodata.h>
#include <vilogger.h>

#define DEFAULT_WINDOW_SIZE 2048
#define DEFAULT_DEGREE 1

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
			predict(cache1.data(), mWindowSize, predictedSamples, predictionCount);

			for(i = 0; i < predictionCount; ++i) rmse[i] += qPow(predictedSamples[i] - cache1[mWindowSize + i], 2);
			++rmseCount;

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
			predict(cache2.data(), mWindowSize, predictedSamples, predictionCount);

			for(i = 0; i < predictionCount; ++i) rmse[i] += qPow(predictedSamples[i] - cache2[mWindowSize + i], 2);
			++rmseCount;

			output2.append(predictedSamples[0]);
			if(output2.size() >= mWindowSize)
			{
				for(i = 0; i < mWindowSize; ++i) outputSamples[i] = output2[i];
				output2.clear();
				outputData.enqueueSplitSamples(outputSamples, 1);
			}
			cache2.removeFirst();
		}
	}

	newCount = predictionCount - 1;
	while(cache1.size() > mWindowSize)
	{
		predict(cache1.data(), mWindowSize, predictedSamples, newCount);

		//for(i = 0; i < newCount; ++i) rmse[i] += qPow(predictedSamples[i] - cache1[mWindowSize + i], 2);
		//++rmseCount;

		output1.append(predictedSamples[0]);
		cache1.removeFirst();
		--newCount;
	}

	newCount = predictionCount - 1;
	while(cache2.size() > mWindowSize)
	{
		predict(cache2.data(), mWindowSize, predictedSamples, newCount);

		//for(i = 0; i < newCount; ++i) rmse[i] += qPow(predictedSamples[i] - cache2[mWindowSize + i], 2);
		//++rmseCount;

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

void ViPredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal *expectations, qreal *mse)
{
	predict(samples, size, predictedSamples, predictionCount);
	for(int i = 0; i < predictionCount; ++i) mse[i] = qPow(predictedSamples[i] - expectations[i], 2);
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

ViDegreePredictor::ViDegreePredictor()
	: ViPredictor()
{
	setDegree(DEFAULT_DEGREE);
}

ViDegreePredictor::ViDegreePredictor(const ViDegreePredictor &other)
	: ViPredictor(other)
{
	mDegree = other.mDegree;
}

ViDegreePredictor::~ViDegreePredictor()
{
}

void ViDegreePredictor::setDegree(const int &degree)
{
	mDegree = degree;
}

int ViDegreePredictor::degree()
{
	return mDegree;
}
