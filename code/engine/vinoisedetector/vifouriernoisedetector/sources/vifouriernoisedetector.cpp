#include <vifouriernoisedetector.h>
#include <vipowercalculator.h>

#define WINDOW_SIZE 64

ViFourierNoiseDetector::ViFourierNoiseDetector()
	: ViNoiseDetector()
{
	mTransformer = new ViFourierTransformer();
	mInput = NULL;
	mOutput = NULL;

	addParameter("Window Size");
	addParameter("Range Start");
	addParameter("Range End");

	setScale(10);
}

ViFourierNoiseDetector::ViFourierNoiseDetector(const ViFourierNoiseDetector &other)
	: ViNoiseDetector(other)
{
	mTransformer = new ViFourierTransformer();
	mInput = NULL;
	mOutput = NULL;

	setWindowSize(other.mWindowSize);
	setRange(other.mStart, other.mEnd);
}

ViFourierNoiseDetector::~ViFourierNoiseDetector()
{
	delete mTransformer;
	if(mInput != NULL) delete [] mInput;
	if(mOutput != NULL) delete [] mOutput;
}

void ViFourierNoiseDetector::setWindowSize(const int &size)
{
	mWindowSize = size;
	setOffset(mWindowSize / 2);
	mTransformer->setSize(mWindowSize);

	if(mInput != NULL) delete [] mInput;
	mInput = new qreal[mWindowSize];
	if(mOutput != NULL) delete [] mOutput;
	mOutput = new qreal[mWindowSize];
}

void ViFourierNoiseDetector::setStart(const qreal &start)
{
	mStart = start;
}

void ViFourierNoiseDetector::setEnd(const qreal &end)
{
	mEnd = end;
}

void ViFourierNoiseDetector::setRange(const qreal &start, const qreal &end)
{
	mStart = start;
	mEnd = end;
}

bool ViFourierNoiseDetector::validParameters()
{
	return parameter("Window Size") > 1 && ViPowerCalculator::isPower(parameter("Window Size"), 2) && parameter("Range Start") >= 0 && parameter("Range End") >= 0 && parameter("Range Start") <= 100 && parameter("Range End") <= 100 && parameter("Range Start") < parameter("Range End");
}

void ViFourierNoiseDetector::initialize()
{
	setWindowSize(parameter("Window Size"));
	setRange(parameter("Range Start"), parameter("Range End"));

	int halfWindowSize = mWindowSize / 2; // Because the frequency spectrum is a mirror on both sides

	mStartSample = floor(halfWindowSize * (mStart / 100.0));
	if(mStartSample < 0) mStartSample = 0;

	mEndSample = ceil(halfWindowSize * (mEnd / 100.0));
	if(mEndSample > halfWindowSize) mEndSample = halfWindowSize;

	mSampleCount = mEndSample - mStartSample + 1;
}

void ViFourierNoiseDetector::detect(QVector<qreal> &samples, QVector<qreal> &noise)
{
	static int i;
	static qreal value;

	while(samples.size() >= mWindowSize)
	{
		for(i = 0; i < mWindowSize; ++i) mInput[i] = samples[i];
		mTransformer->forwardTransform(mInput, mOutput);

        value = 0;
		for(i = mStartSample; i <= mEndSample; ++i) value += abs(mOutput[i - 1] - mOutput[i]);

		noise.append(value / mSampleCount);
        samples.removeFirst();
	}
}
