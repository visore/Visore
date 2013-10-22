#include <vinoise.h>
#include<vilogger.h>
#define MINIMIZE_THRESHOLD 2
#define SNR_THRESHOLD 49 // 2% of the data is noisy (98 / 2)

#define WINDOW_SIZE 1024
#define DEFAULT_TYPE Maximum
#define DEFAULT_THRESHOLD 0.1

ViNoise::ViNoise(const int &size)
{
	resize(size);
	setMaskType(DEFAULT_TYPE);
	setThreshold(DEFAULT_THRESHOLD);
}

ViNoise::ViNoise(const ViNoise &other)
{
	mMask = other.mMask;
	mData = other.mData;
	setPointer = other.setPointer;
	mThreshold = other.mThreshold;
}

ViNoise::~ViNoise()
{
}

void ViNoise::setMaskType(const MaskType &type)
{
	if(type == Direct) setPointer = &ViNoise::setDirect;
	else if(type == Mean) setPointer = &ViNoise::setMean;
	else if(type == Maximum) setPointer = &ViNoise::setMaximum;
}

void ViNoise::setThreshold(const qreal &threshold)
{
	mThreshold = threshold;
}

void ViNoise::resize(const int &size)
{
	mMask.resize(size);
	mData.resize(size);
	for(int i = 0; i < size; ++i)
	{
		mMask[i] = 0;
		mData[i] = 0;
	}
}

void ViNoise::clear()
{
	resize(mMask.size());
}

int ViNoise::size() const
{
	return mMask.size();
}

qreal ViNoise::snr() const
{
	int signal = 0, noise = 0;
	for(int i = 0; i < mMask.size(); ++i)
	{
		if(mMask[i]) ++noise;
		else ++signal;
	}
	return signal / qreal(noise);
}

bool ViNoise::isNoisy() const
{
	return snr() <= SNR_THRESHOLD;
}

void ViNoise::minimize()
{
	int i, j, end, counter = 0;
	qreal first, last;
	bool noiseStarted = false;
	for(i = 0; i < mMask.size(); ++i)
	{
		if(mMask[i])
		{
			if(noiseStarted && counter > 0)
			{
				last = mData[i];
				end = i - counter;
				for(j = i - 1; j >= end; --j)
				{
					mMask[j] = 1;
					mData[j] = (first + last) / 2;
				}
				first = last;
				counter = 0;
			}
			else
			{
				first = mData[i];
				noiseStarted = true;
			}
		}
		else if(noiseStarted)
		{
			++counter;
			if(counter > MINIMIZE_THRESHOLD)
			{
				noiseStarted = false;
				counter = 0;
			}
		}
	}
}

ViSampleChunk& ViNoise::data()
{
	return mData;
}

ViSampleChunk& ViNoise::mask()
{
	generateMask();
	return mMask;
}

void ViNoise::set(const int &index, const qreal &value)
{
	if(value > 1) mData[index] = 1;
	else mData[index] = value;

	//(this->*setPointer)(index, value);
}

void ViNoise::set(const int &index, const ViNoise &other)
{
	int newIndex;
	for(int i = 0; i < other.size(); ++i)
	{
		newIndex = index + i;
		mData[newIndex] = other.mData[i];
		mMask[newIndex] = other.mMask[i];
	}
}

void ViNoise::setDirect(const int &index, const qreal &value)
{
	if(value > mThreshold) mMask[index] = 1;
	else mMask[index] = 0;
}

void ViNoise::setMean(const int &index, const qreal &value)
{
	if(index <= WINDOW_SIZE) return;

	int newIndex = index - WINDOW_SIZE;

	qreal mean = 0;
	int count = 0;

	int start = newIndex - WINDOW_SIZE;
	count += newIndex - start;
	if(start >= 0)
	{
		for(int i = start; i < newIndex; ++i)
		{
			mean += mData[i];
		}
	}

	int end = newIndex + 1 + WINDOW_SIZE;
	count += WINDOW_SIZE;
	for(int i = newIndex + 1; i < end; ++i)
	{
		mean += mData[i];
	}

	mean /= count;

	if(mData[newIndex] >= (mean / mThreshold)) mMask[newIndex] = 1;
	else mMask[newIndex] = 0;
}

void ViNoise::setMaximum(const int &index, const qreal &value)
{
	if(index <= WINDOW_SIZE) return;

	int newIndex = index - WINDOW_SIZE;
	qreal maximum = 0;

	int start = newIndex - WINDOW_SIZE;
	if(start >= 0)
	{
		for(int i = start; i < newIndex; ++i)
		{
			if(mData[i] > maximum) maximum = mData[i];
		}
	}

	int end = newIndex + 1 + WINDOW_SIZE;
	for(int i = newIndex + 1; i < end; ++i)
	{
		if(mData[i] > maximum) maximum = mData[i];
	}
	cout<<maximum<<endl;
	if(mData[newIndex] >= (maximum * mThreshold)) mMask[newIndex] = 1;
	else mMask[newIndex] = 0;
}

void ViNoise::generateMask()
{
	/*qreal maximum = 0;
	for(int i = 0; i < mData.size(); ++i)
	{
		if(mData[i] > maximum) maximum = mData[i];
	}
	maximum *= mThreshold;
	for(int i = 0; i < mData.size(); ++i)
	{
		if(mData[i] >= maximum) mMask[i] = 1;
		else mMask[i] = 0;
	}*/

	qreal mean = 0;
	for(int i = 0; i < mData.size(); ++i)
	{
		mean += mData[i];
	}
	mean /= mData.size();
	mean /= mThreshold;
	for(int i = 0; i < mData.size(); ++i)
	{
		if(mData[i] >= mean) mMask[i] = 1;
		else mMask[i] = 0;
	}
}

const bool& ViNoise::at(const int &index) const
{
	const bool& value = mMask[index];
	return value;
}

const bool& ViNoise::operator [] (const int &index) const
{
	const bool& value = mMask[index];
	return value;
}
