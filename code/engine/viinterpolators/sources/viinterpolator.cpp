#include <viinterpolator.h>
#include <vilogger.h>

ViInterpolator::ViInterpolator()
	: ViLibrary()
{
	mMaximumSamples = -1;
}

ViInterpolator::ViInterpolator(const int &maximumSamples)
	: ViLibrary()
{
	mMaximumSamples = maximumSamples;
}

ViInterpolator::ViInterpolator(const ViInterpolator &other)
	: ViLibrary(other)
{
	mMaximumSamples = other.mMaximumSamples;
}

ViInterpolator::~ViInterpolator()
{
}

void ViInterpolator::setMaximumSamples(const int &count)
{
	mMaximumSamples = count;
}

int ViInterpolator::maximumSamples() const
{
	return mMaximumSamples;
}

bool ViInterpolator::interpolate(ViSampleChunk &samples, const ViNoise &noise)
{
	bool signalStarted = false, success = true;
	int end, noiseLength = 0, noiseStart = -1;

	for(int i = 0; i < noise.size(); ++i)
	{
		if(noise[i])
		{
			if(signalStarted)
			{
				end = noiseStart + noiseLength;
				success &= interpolate(samples.data(), noiseStart, samples.data() + end, i - end, samples.data() + noiseStart, noiseLength);

				noiseStart = i;
				noiseLength = 1;
				signalStarted = false;
			}
			else
			{
				if(noiseStart < 0)
				{
					noiseStart = i;
				}
				++noiseLength;
			}
		}
		else if(noiseStart >= 0)
		{
			signalStarted = true;
		}
	}

	if(noiseStart >= 0)
	{
		end = noiseStart + noiseLength;
		success &= interpolate(samples.data(), noiseStart, samples.data() + end, noise.size() - end, samples.data() + noiseStart, noiseLength);
	}

	return success;
}

bool ViInterpolator::interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	if(mMaximumSamples < 1)
	{
		return interpolateSamples(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize);
	}

	static int newLeftSize, newRightSize, i;
	if(leftSize > mMaximumSamples)
	{
		leftSamples += (leftSize - mMaximumSamples);
		newLeftSize = mMaximumSamples;
	}
	else
	{
		newLeftSize = leftSize;
	}

	if(rightSize > mMaximumSamples)
	{
		newRightSize = mMaximumSamples;
	}
	else
	{
		newRightSize = rightSize;
	}

	bool result = interpolateSamples(leftSamples, newLeftSize, rightSamples, newRightSize, outputSamples, outputSize);
	for(i = 0; i < outputSize; ++i)
	{
		qreal &value = outputSamples[i];
		if(value > 1) value = 1;
		else if(value < -1) value = -1;
	}
	return result;
}

ViElement ViInterpolator::exportData()
{
	ViElement root("interpolator");
	root.addChild("name", name());
	return root;
}

bool ViInterpolator::importData(ViElement element)
{
	return element.name() == "interpolator" && element.child("name").toString() == name();
}

ViDegreeInterpolator::ViDegreeInterpolator()
	: ViInterpolator()
{
	setDegree(1);
}

ViDegreeInterpolator::ViDegreeInterpolator(const int &degree)
	: ViInterpolator()
{
	setDegree(degree);
}

ViDegreeInterpolator::ViDegreeInterpolator(const int &maximumSamples, const int &degree)
	: ViInterpolator(maximumSamples)
{
	setDegree(degree);
}

ViDegreeInterpolator::ViDegreeInterpolator(const ViDegreeInterpolator &other)
	: ViInterpolator(other)
{
	mDegree = other.mDegree;
}

ViDegreeInterpolator::~ViDegreeInterpolator()
{
}

void ViDegreeInterpolator::setDegree(const int &degree)
{
	if(degree < 1)
	{
		LOG("Invalid degree " + QString::number(degree) + " specified. Setting degree to 1.", QtCriticalMsg);
		mDegree = 1;
	}
	else
	{
		mDegree = degree;
	}
}

int ViDegreeInterpolator::degree() const
{
	return mDegree;
}

ViAutoDegreeInterpolator::ViAutoDegreeInterpolator()
	: ViDegreeInterpolator()
{
	setMaximumSamples();
}

ViAutoDegreeInterpolator::ViAutoDegreeInterpolator(const int &degree)
	: ViDegreeInterpolator(degree)
{
	setMaximumSamples();
}

ViAutoDegreeInterpolator::ViAutoDegreeInterpolator(const ViDegreeInterpolator &other)
	: ViDegreeInterpolator(other)
{
	setMaximumSamples();
}

ViAutoDegreeInterpolator::~ViAutoDegreeInterpolator()
{
}

void ViAutoDegreeInterpolator::setMaximumSamples(const int &count)
{
	if(mDegree % 2)
	{
		ViInterpolator::setMaximumSamples(qCeil(mDegree / 2.0));
	}
	else
	{
		ViInterpolator::setMaximumSamples((mDegree / 2) + 1);
	}
}
