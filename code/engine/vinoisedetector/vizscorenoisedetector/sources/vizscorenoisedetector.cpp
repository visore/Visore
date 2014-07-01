#include <vizscorenoisedetector.h>
#include <vimath.h>

#define DEFAULT_WINDOW_SIZE 128

ViZscoreNoiseDetector::ViZscoreNoiseDetector()
	: ViNoiseDetector()
{
	setScale(30);
	setWindowSize(DEFAULT_WINDOW_SIZE);
	addParameter("Window Size");

	QObject::connect(this, SIGNAL(parameterChanged(QString,qreal)), this, SLOT(changeParameter(QString,qreal)));
}

void ViZscoreNoiseDetector::setWindowSize(const int &size)
{
	mWindowSize = size;
	mHalfWindow = qFloor(size / 2.0);
	setOffset(mHalfWindow);
}

bool ViZscoreNoiseDetector::validParameters()
{
	return parameter("Window Size") > 0;
}

void ViZscoreNoiseDetector::changeParameter(QString name, qreal value)
{
	if(name == "Window Size") setWindowSize(value);
	else
	{
		LOG("The noise detector doe not have a parameter named " + name + ".", QtFatalMsg);
		exit(-1);
	}
}

void ViZscoreNoiseDetector::detect(QVector<qreal> &samples, QVector<qreal> &noise)
{
	static qreal mean, standardDeviation;
	while(samples.size() >= mWindowSize)
	{
		mean = ViMath<qreal>::mean(samples.constData(), samples.size());
		standardDeviation = ViMath<qreal>::standardDeviationBiased(samples.constData(), samples.size(), mean);
		noise.append(abs((samples[mHalfWindow] - mean) / standardDeviation));
		samples.removeFirst();
	}
}
