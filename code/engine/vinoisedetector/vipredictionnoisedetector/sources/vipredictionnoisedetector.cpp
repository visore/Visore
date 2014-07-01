#include <vipredictionnoisedetector.h>
#include <vinoisecreator.h>

#define DEFAULT_THRESHOLD 0.63

ViPredictionNoiseDetector::ViPredictionNoiseDetector(ViPredictor *predictor)
	: ViNoiseDetector()
{
	mPredictor = predictor;

	QStringList parameters = mPredictor->parameters();
	for(int i = 0; i < parameters.size(); ++i) addParameter(parameters[i]);
	addParameter("Threshold");
	setScale(4);

	setThreshold(DEFAULT_THRESHOLD);

	mPredictionCount = ViNoiseCreator::maximumNoiseSize();
	mPredictions = new qreal[mPredictionCount];

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

void ViPredictionNoiseDetector::detect(QVector<qreal> &samples, QVector<qreal> &noise)
{
	static int counter, i, j;
	qreal max;
	while(samples.size() >= mRequiredSize)
	{
		if(samples.size() <= mWindowSize) mPredictor->predict(samples.data(), samples.size(), mPrediction);
		else  mPredictor->predict(samples.data() + (samples.size() - mWindowSize), mWindowSize, mPrediction);
		mDifference = abs(mPrediction - samples[mWindowSize]);

		if(mDifference > mThreshold)
		{
			if(samples.size() <= mWindowSize) mPredictor->predict(samples.data(), samples.size(), mPredictions, mPredictionCount);
			else mPredictor->predict(samples.data() + (samples.size() - mWindowSize), mWindowSize, mPredictions, mPredictionCount);

			counter = 0;
			//noise.append(mDifference);
			noise.append(100);
			max = mDifference;

			for(i = mPredictionCount - 1; i > 0; --i)
			{
				mDifference = abs(mPredictions[i] - samples[mWindowSize + i]);
				if(mDifference > mThreshold)
				{
					//noise.append(mDifference);
					noise.append(100);

					if(mDifference > max) max = mDifference;
					for(j = 1; j < i; ++j)
					{
						mDifference = abs(mPredictions[j] - samples[mWindowSize + j]);
						if(mDifference > max) max = mDifference;
					}
					for(j = 1; j < i; ++j) noise.append(100);//noise.append(max);
					samples.remove(0, i);
					break;
				}
			}
			samples.removeFirst();
		}
		else
		{
			//noise.append(mDifference);
			noise.append(0);
			samples.removeFirst();
		}
	}
}
