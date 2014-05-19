#include <vipredictor.h>
#include <viaudiodata.h>
#include <vilogger.h>

#define DEFAULT_WINDOW_SIZE 2048
#define DEFAULT_DEGREE 1

ViPredictor::ViPredictor()
{
	setWindowSize(DEFAULT_WINDOW_SIZE);
	setOffset(0);
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
	int i;

	ViAudioReadData inputData(input);
	ViAudioWriteData outputData(output);
	inputData.setSampleCount(mWindowSize * 2); // times 2 so that each channel has mWindowSize data
	outputData.setSampleCount(mWindowSize * 2);

	ViSampleChunk outputSamples(mWindowSize);
	QVector<qreal> cache1, cache2, output1, output2;
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

		while(cache1.size() >= mWindowSize)
		{
			offset = cache1.size() - mWindowSize;
			predict(cache1.data() + offset, mWindowSize, predictedSamples, predictionCount);
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

		while(cache2.size() >= mWindowSize)
		{
			offset = cache2.size() - mWindowSize;
			predict(cache2.data() + offset, mWindowSize, predictedSamples, predictionCount);
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

	while(cache1.size() > 1)
	{
		predict(cache1.data(), cache1.size(), predictedSamples, predictionCount);
		output1.append(predictedSamples[0]);
		cache1.removeFirst();
	}
	cache1.append(0);
	outputSamples.resize(output1.size());
	for(i = 0; i < output1.size(); ++i) outputSamples[i] = output1[i];
	outputData.enqueueSplitSamples(outputSamples, 0);

	while(cache2.size() > 1)
	{
		predict(cache2.data(), cache2.size(), predictedSamples, predictionCount);
		output2.append(predictedSamples[0]);
		cache2.removeFirst();
	}
	cache2.append(0);
	outputSamples.resize(output2.size());
	for(i = 0; i < output2.size(); ++i) outputSamples[i] = output2[i];
	outputData.enqueueSplitSamples(outputSamples, 1);
}

void ViPredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal *expectations, qreal *mse)
{
	predict(samples, size, predictedSamples, predictionCount);
	for(int i = 0; i < predictionCount; ++i) mse[i] = qPow(predictedSamples[i] - expectations[i], 2);
}

void ViPredictor::setParameters(const qreal &parameter1)
{
	LOG("1 is an invalid parameter count for this predictor.", QtCriticalMsg);
	exit(-1);
}

void ViPredictor::setParameters(const qreal &parameter1, const qreal &parameter2)
{
	LOG("2 is an invalid parameter count for this predictor.", QtCriticalMsg);
	exit(-1);
}

void ViPredictor::setParameters(const qreal &parameter1, const qreal &parameter2, const qreal &parameter3)
{
	LOG("3 is an invalid parameter count for this predictor.", QtCriticalMsg);
	exit(-1);
}

void ViPredictor::setParameters(const qreal &parameter1, const qreal &parameter2, const qreal &parameter3, const qreal &parameter4)
{
	LOG("4 is an invalid parameter count for this predictor.", QtCriticalMsg);
	exit(-1);
}

void ViPredictor::setParameters(const qreal &parameter1, const qreal &parameter2, const qreal &parameter3, const qreal &parameter4, const qreal &parameter5)
{
	LOG("5 is an invalid parameter count for this predictor.", QtCriticalMsg);
	exit(-1);
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
