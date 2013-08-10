#include <vinoise.h>

#include<vilogger.h>

#define MINIMIZE_THRESHOLD 8
#define SNR_THRESHOLD 19 // 5% of the data is noisy (95 / 5)
#define NOISE_THRESHOLD 0.01 // Values range from 0 to 1. Everything above this threshold is considered noise

ViNoise::ViNoise(const int &size)
{
	resize(size);
}

ViNoise::ViNoise(const ViNoise &other)
{
	mMask = other.mMask;
	mData = other.mData;
}

ViNoise::~ViNoise()
{
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
	return snr() >= SNR_THRESHOLD;
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
	return mMask;
}

void ViNoise::set(const int &index, const qreal &value)
{
	if(value > 1)
	{
		mData[index] = 1;
	}
	else
	{
		mData[index] = value;
	}

	if(value > NOISE_THRESHOLD)
	{
		mMask[index] = 1;
	}
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

bool ViNoise::at(const int &index) const
{
	return mMask[index];
}

bool ViNoise::operator [] (const int &index) const
{
	return mMask[index];
}
