#include <vipredictor.h>
#include <viaudiodata.h>
#include <vilogger.h>
#include <float.h>

#define DEFAULT_WINDOW_SIZE 2048

ViPredictor::ViPredictor()
{
	setWindowSize(DEFAULT_WINDOW_SIZE);
	mPredictionIndex = 0;
}

ViPredictor::ViPredictor(const ViPredictor &other)
{
	mWindowSize = other.mWindowSize;
	mOffset = other.mOffset;
	mParameterNames = other.mParameterNames;
	mPredictionIndex = other.mPredictionIndex;
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

void ViPredictor::predict(ViBuffer *input, ViBuffer *output, ViError *modelError)
{
	predict(input, output, 1, modelError);
}

void ViPredictor::predict(ViBuffer *input, ViBuffer *output, const int &predictionCount, ViError *modelError)
{
	if(modelError != NULL) modelError->clear();

	if(mPredictionIndex >= predictionCount)
	{
		LOG("The prediction index of " + QString::number(mPredictionIndex) + " can not be used with a prediction count of " + QString::number(predictionCount), QtFatalMsg);
		exit(-1);
	}

	initialize(input->format().channelCount(), predictionCount);

	int i, end, newCount;

	end = mWindowSize + predictionCount;

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
			if(predict(cache1.data(), mWindowSize, predictedSamples, predictionCount, modelError, 0))
			{
				for(i = 0; i < predictionCount; ++i) adjustValue(predictedSamples[i]);
			}
			else
			{
				for(i = 0; i < predictionCount; ++i) predictedSamples[i] = 0;
			}

			output1.append(predictedSamples[mPredictionIndex]);
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
			if(predict(cache2.data(), mWindowSize, predictedSamples, predictionCount, modelError, 1))
			{
				for(i = 0; i < predictionCount; ++i) adjustValue(predictedSamples[i]);
			}
			else
			{
				for(i = 0; i < predictionCount; ++i) predictedSamples[i] = 0;
			}

			output2.append(predictedSamples[mPredictionIndex]);
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
		if(predict(cache1.data(), mWindowSize, predictedSamples, newCount, modelError, 0))
		{
			for(i = 0; i < newCount; ++i) adjustValue(predictedSamples[i]);
		}
		else
		{
			for(i = 0; i < predictionCount; ++i) predictedSamples[i] = 0;
		}

		output1.append(predictedSamples[mPredictionIndex]);
		cache1.removeFirst();
		--newCount;
	}

	newCount = predictionCount - 1;
	while(cache2.size() > mWindowSize)
	{
		if(predict(cache2.data(), mWindowSize, predictedSamples, newCount, modelError, 1))
		{
			for(i = 0; i < newCount; ++i) adjustValue(predictedSamples[i]);
		}
		else
		{
			for(i = 0; i < predictionCount; ++i) predictedSamples[i] = 0;
		}

		output2.append(predictedSamples[mPredictionIndex]);
		cache2.removeFirst();
		--newCount;
	}

	outputSamples.resize(output1.size());
	for(i = 0; i < output1.size(); ++i) outputSamples[i] = output1[i];
	outputData.enqueueSplitSamples(outputSamples, 0);

	outputSamples.resize(output2.size());
	for(i = 0; i < output2.size(); ++i) outputSamples[i] = output2[i];
	outputData.enqueueSplitSamples(outputSamples, 1);
}

void ViPredictor::adjustValue(qreal &value)
{
	if(value > 1) value = 1;
	else if (value < -1) value = -1;
}

void ViPredictor::predict(ViBuffer *input, ViBuffer *output, ViErrorCollection *predictionErrors, ViError *modelError)
{
	predict(input, output, 1, predictionErrors, modelError);
}

void ViPredictor::predict(ViBuffer *input, ViBuffer *output, const int &predictionCount, ViErrorCollection *predictionErrors, ViError *modelError)
{
	if(predictionErrors != NULL) predictionErrors->clear();
	if(modelError != NULL) modelError->clear();

	if(mPredictionIndex >= predictionCount)
	{
		LOG("The prediction index of " + QString::number(mPredictionIndex) + " can not be used with a prediction count of " + QString::number(predictionCount), QtFatalMsg);
		exit(-1);
	}

	initialize(input->format().channelCount(), predictionCount);

	int i, end, newCount;

	end = mWindowSize + predictionCount;

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
			if(predict(cache1.data(), mWindowSize, predictedSamples, predictionCount, modelError, 0))
			{
				for(i = 0; i < predictionCount; ++i) adjustValue(predictedSamples[i]);
			}
			else
			{
				for(i = 0; i < predictionCount; ++i) predictedSamples[i] = 0;
			}
			for(i = 0; i < predictionCount; ++i) predictionErrors->at(i + 1).add(predictedSamples[i], cache1[i]);

			output1.append(predictedSamples[mPredictionIndex]);
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
			if(predict(cache2.data(), mWindowSize, predictedSamples, predictionCount, modelError, 1))
			{
				for(i = 0; i < predictionCount; ++i) adjustValue(predictedSamples[i]);
			}
			else
			{
				for(i = 0; i < predictionCount; ++i) predictedSamples[i] = 0;
			}
			for(i = 0; i < predictionCount; ++i) predictionErrors->at(i + 1).add(predictedSamples[i], cache2[i]);

			output2.append(predictedSamples[mPredictionIndex]);
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
		if(predict(cache1.data(), mWindowSize, predictedSamples, newCount, modelError, 0))
		{
			for(i = 0; i < newCount; ++i) adjustValue(predictedSamples[i]);
		}
		else
		{
			for(i = 0; i < predictionCount; ++i) predictedSamples[i] = 0;
		}
		for(i = 0; i < newCount; ++i) predictionErrors->at(i + 1).add(predictedSamples[i], cache1[i]);

		output1.append(predictedSamples[mPredictionIndex]);
		cache1.removeFirst();
		--newCount;
	}

	newCount = predictionCount - 1;
	while(cache2.size() > mWindowSize)
	{
		if(predict(cache2.data(), mWindowSize, predictedSamples, newCount, modelError, 1))
		{
			for(i = 0; i < newCount; ++i) adjustValue(predictedSamples[i]);
		}
		else
		{
			for(i = 0; i < predictionCount; ++i) predictedSamples[i] = 0;
		}
		for(i = 0; i < newCount; ++i) predictionErrors->at(i + 1).add(predictedSamples[i], cache2[i]);

		output2.append(predictedSamples[mPredictionIndex]);
		cache2.removeFirst();
		--newCount;
	}

	outputSamples.resize(output1.size());
	for(i = 0; i < output1.size(); ++i) outputSamples[i] = output1[i];
	outputData.enqueueSplitSamples(outputSamples, 0);

	outputSamples.resize(output2.size());
	for(i = 0; i < output2.size(); ++i) outputSamples[i] = output2[i];
	outputData.enqueueSplitSamples(outputSamples, 1);
}

bool ViPredictor::predict(qreal *samples, const int &size, qreal &prediction, const int &channel)
{
	return predict(samples, size, &prediction, 1, NULL, channel);
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

bool ViPredictor::validParameters()
{
	return true;
}

bool ViPredictor::hasParameter(const QString &name)
{
	return mParameterNames.contains(name);
}

QString ViPredictor::parameterName(const int &index, const bool &allCaps)
{
	if(index >= mParameterNames.size()) return "";
	if(allCaps) return mParameterNames[index].toUpper();
	return mParameterNames[index];
}

QStringList ViPredictor::parameters()
{
	return mParameterNames;
}

void ViPredictor::addParameterName(const QString &name)
{
	mParameterNames.append(name);
}

void ViPredictor::initialize(const int &channelCount, const int &predictionCount)
{
}

void ViPredictor::setPredictionIndex(const int &index)
{
	mPredictionIndex = index;
}
