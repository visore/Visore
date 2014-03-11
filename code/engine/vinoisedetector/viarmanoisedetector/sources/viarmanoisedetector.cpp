#include <viarmanoisedetector.h>
#include <CArma.h>

#define WINDOW_SIZE 32

ViArmaNoiseDetector::ViArmaNoiseDetector(const int &degree)
    : ViNoiseDetector()
{
    mWindowData = NULL;
    mArma = new CArma();
    setDegree(degree);
    setWindowSize(WINDOW_SIZE);
    setOffset(mWindowSize);

    mArma->setOrder(4);
}

ViArmaNoiseDetector::~ViArmaNoiseDetector()
{
    delete mArma;
    if(mWindowData != NULL)
    {
        delete [] mWindowData;
        mWindowData = NULL;
    }
}

QString ViArmaNoiseDetector::name(QString replace, bool spaced)
{
	QString n = ViNoiseDetector::name(replace, spaced);
	if(spaced) n += " ";
	return n + QString::number(mDegree);
}

void ViArmaNoiseDetector::setWindowSize(int size)
{
    mWindowSize = size;
    mArma->setWindowSize(mWindowSize);
    if(mWindowData != NULL)
    {
        delete [] mWindowData;
    }
    mWindowData = new double[mWindowSize];
}

void ViArmaNoiseDetector::setDegree(const int &degree)
{
	mDegree = degree;
}

int ViArmaNoiseDetector::degree()
{
	return mDegree;
}

void ViArmaNoiseDetector::calculateNoise(QQueue<qreal> &samples)
{
    static int i;
    while(samples.size() > mWindowSize)
	{
        for(i = 0; i < mWindowSize; ++i)
        {
            mWindowData[i] = samples[i];
        }
        mArma->setSamples(mWindowData, mWindowSize);
        //setNoise(mArma->predict()/3);
        setNoise(qAbs(samples[mWindowSize]-mArma->predict()));

		samples.removeFirst();
	}
}

ViArmaNoiseDetector* ViArmaNoiseDetector::clone()
{
    return new ViArmaNoiseDetector(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViArmaNoiseDetector* create()
{
    return new ViArmaNoiseDetector();
}

#ifdef __cplusplus
}
#endif
