#include <vipredictionnoisedetector.h>
#include <vinoisecreator.h>

#define NOISE_THRESHOLD 0.63

ViPredictionNoiseDetector::ViPredictionNoiseDetector(ViPredictor *predictor)
	: ViNoiseDetector()
{
	mPredictor = predictor;

	QStringList parameters = mPredictor->parameters();
	for(int i = 0; i < parameters.size(); ++i) addParameter(parameters[i]);
	setScale(4);

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

bool ViPredictionNoiseDetector::validParameters()
{
	return mPredictor->validParameters();
}

void ViPredictionNoiseDetector::changeParameter(QString name, qreal value)
{
	mPredictor->setParameter(name, value);

	setOffset(mPredictor->offset());
	mWindowSize = mPredictor->windowSize();
	mRequiredSize = mWindowSize + ViNoiseCreator::maximumNoiseSize();
}

void ViPredictionNoiseDetector::initialize()
{

}

void ViPredictionNoiseDetector::detect(QVector<qreal> &samples, QVector<qreal> &noise)
{
	int i;
	while(samples.size() >= mRequiredSize)
	{
		mPredictor->predict(samples.data(), mWindowSize, mPrediction);
		mDifference = abs(mPrediction - samples[mWindowSize]);
		if(mDifference > NOISE_THRESHOLD)
		{
			mPredictor->predict(samples.data(), mWindowSize, mPredictions, mPredictionCount);
			for(i = 0; i < mPredictionCount; ++i)
			{
				mDifference = abs(mPredictions[i] - samples[mWindowSize + i]);
				if(mDifference < NOISE_THRESHOLD) break;
				samples[mWindowSize + i] = samples[mWindowSize + i - 1];
				noise.append(mDifference);
				//noise.append(10);
			}
			samples.remove(0, i);
		}
		else
		{
			//noise.append(0);
			noise.append(mDifference);
			samples.removeFirst();
		}
	}
}
