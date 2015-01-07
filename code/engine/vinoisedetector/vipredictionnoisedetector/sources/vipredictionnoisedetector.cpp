#include <vipredictionnoisedetector.h>
#include <vinoisecreator.h>

#define DEFAULT_THRESHOLD 0.5

ViPredictionNoiseDetector::ViPredictionNoiseDetector(ViPredictor *predictor, PredictionMode mode)
	: ViNoiseDetector()
{
	mPredictor = predictor;

	QStringList parameters = mPredictor->parameters();
	for(int i = 0; i < parameters.size(); ++i) addParameter(parameters[i]);
	addParameter("Threshold");

	setScale(4);

	setThreshold(DEFAULT_THRESHOLD);

	mPredictionCount = 0;
	mPredictions = NULL;
	setPredictionMode(mode);

	mMaxNoise = ViNoiseCreator::maximumNoiseSize();

	QObject::connect(this, SIGNAL(parameterChanged(QString,qreal)), this, SLOT(changeParameter(QString,qreal)));
}

ViPredictionNoiseDetector::~ViPredictionNoiseDetector()
{
	delete mPredictor;
	delete [] mPredictions;
}

QString ViPredictionNoiseDetector::name(QString replace, bool spaced)
{
	QString n = ViNoiseDetector::name(replace, spaced);
	if(spaced) n += " ";
	else n += "_";
	return n + mPredictor->name(replace, spaced);
}

void ViPredictionNoiseDetector::setThreshold(const qreal threshold)
{
	mThreshold = threshold;
}

void ViPredictionNoiseDetector::setPredictionMode(PredictionMode mode)
{
	mPredictionMode = mode;

	if(mPredictionMode == Batch) mPredictionCount = ViNoiseCreator::maximumNoiseSize();
	else mPredictionCount = 1;

	delete [] mPredictions;
	mPredictions = new qreal[mPredictionCount];
}

bool ViPredictionNoiseDetector::validParameters()
{
	return mPredictor->validParameters();
}

void ViPredictionNoiseDetector::changeParameter(QString name, qreal value)
{
	if(name == "Threshold") setThreshold(value);
	else
	{
		mPredictor->setParameter(name, value);

		setOffset(mPredictor->offset());
		mWindowSize = mPredictor->windowSize();
		mRequiredSize = mWindowSize + mPredictionCount;
	}
}

void ViPredictionNoiseDetector::initialize(const int &channelCount)
{
	mPredictor->initialize(channelCount, mPredictionCount);

	mNoiseCounter.clear();
	mNoiseCounter.append(0);
	mNoiseCounter.append(0);
}

void ViPredictionNoiseDetector::detect(QVector<qreal> &samples, QVector<qreal> &noise, const int &channel)
{
	if(mPredictionMode == Batch) detectBatch(samples, noise, channel);
	else detectRecurrent(samples, noise, channel);
}

void ViPredictionNoiseDetector::detectBatch(QVector<qreal> &samples, QVector<qreal> &noise, const int &channel)
{
	while(samples.size() >= mRequiredSize)
	{
		mPredictor->predict(samples.data(), mWindowSize, mPredictions, mPredictionCount, NULL, channel);
		mDifference = abs(mPredictions[0] - samples[mWindowSize]);
		if(mDifference > mThreshold)
		{
			noise.append(mDifference);
			for(int i =1; i < mPredictionCount; ++i)
			{
				noise.append(abs(mPredictions[i] - samples[mWindowSize+i]));
			}
			samples.remove(0, mPredictionCount);
		}
		else
		{
			noise.append(mDifference);
			samples.removeFirst();
		}
	}
}

void ViPredictionNoiseDetector::detectRecurrent(QVector<qreal> &samples, QVector<qreal> &noise, const int &channel)
{
	while(samples.size() >= mRequiredSize)
	{
		mPredictor->predict(samples.data(), mWindowSize, mPredictions, mPredictionCount, NULL, channel);
		mDifference = abs(mPredictions[0] - samples[mWindowSize]);
		if(mDifference > mThreshold)
		{
			mNoiseCounter[channel] += 1;
			if(mNoiseCounter[channel] <= mMaxNoise)
			{
				samples[mWindowSize] = 0;//samples[mWindowSize-1];
			}
			else
			{
				mNoiseCounter[channel] = 0;
			}
		}
		else
		{
			mNoiseCounter[channel] = 0;
		}

		noise.append(mDifference);
		samples.removeFirst();
	}
}
