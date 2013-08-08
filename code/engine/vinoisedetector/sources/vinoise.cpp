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
	mNoise = other.mNoise;
	mData = other.mData;
}

ViNoise::~ViNoise()
{
}

void ViNoise::resize(const int &size)
{
	mNoise.resize(size);
	mData.resize(size);
	for(int i = 0; i < size; ++i)
	{
		mNoise[i] = false;
		mData[i] = 0;
	}
}

void ViNoise::clear()
{
	resize(mNoise.size());
}

int ViNoise::size() const
{
	return mNoise.size();
}

qreal ViNoise::snr() const
{
	int signal = 0, noise = 0;
	for(int i = 0; i < mNoise.size(); ++i)
	{
		if(mNoise[i]) ++noise;
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
	int counter = 0;
	bool started = false;
	for(int i = 0; i < mNoise.size(); ++i)
	{
		if(started)
		{
			if(mData[i] >= NOISE_THRESHOLD)
			{
				int end = i - counter;
				for(int j = i - 1; j >= end; --j)
				{
					mNoise[j] = true;
					mData[i] = 1;
					//LOG("xxx3");
				}
				counter == 0;
			}
			else
			{
				++counter;
				if(counter > MINIMIZE_THRESHOLD)
				{
					counter = 0;
					started = false;
				}
			}
		}
		else
		{
			if(mData[i] >= NOISE_THRESHOLD)
			{
				started = true;
				counter == 0;
			}
		}




		/*if(mNoise[i])
		{
			if(counter <= MINIMIZE_THRESHOLD)
			{
				int end = i - counter;
				for(int j = i - 1; j >= end; --j)
				{
					mNoise[j] = true;
					mData[i] = 1;
				}
			}
			counter = 0;
		}
		else
		{
			++counter;
		}*/
	}
}

ViSampleChunk& ViNoise::data()
{
	return mData;
}

void ViNoise::set(const int &index, const qreal &value)
{
	//mData[index] = value;
	mData[index] = 0;
	if(value > NOISE_THRESHOLD)
	{LOG("xxx3");
		mNoise[index] = true;
		mData[index] = 1;
	}
}

bool ViNoise::at(const int &index) const
{
	return mNoise[index];
}

bool ViNoise::operator [] (const int &index) const
{
	return mNoise[index];
}
