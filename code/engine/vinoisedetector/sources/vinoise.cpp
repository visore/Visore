#include <vinoise.h>
#include<vilogger.h>
#define MINIMIZE_THRESHOLD 2
#define SNR_THRESHOLD 49 // 2% of the data is noisy (98 / 2)

#define WINDOW_SIZE 1024
#define DEFAULT_THRESHOLD 0.02

ViNoise::ViNoise(ViSampleChunk *data, ViSampleChunk *mask, const qreal &threshold)
{
	mMask = mask;
	mData = data;
	setThreshold(threshold);
}

ViNoise::ViNoise(const int &size)
{
	mMask = new ViSampleChunk();
	mData = new ViSampleChunk();
	resize(size);
	setThreshold(DEFAULT_THRESHOLD);
}

ViNoise::ViNoise(const ViNoise &other)
{
	mMask = other.mMask;
	mData = other.mData;
	setThreshold(mThreshold = other.mThreshold);
}

ViNoise::~ViNoise()
{
	if(mMask != NULL)
	{
		delete mMask;
		mMask = NULL;
	}
	if(mData != NULL)
	{
		delete mData;
		mData = NULL;
	}
}

void ViNoise::setThreshold(const qreal &threshold)
{
	mThreshold = threshold;
}

void ViNoise::resize(const int &size)
{
	mMask->resize(size);
	mData->resize(size);
	for(int i = 0; i < size; ++i)
	{
		(*mMask)[i] = 0;
		(*mData)[i] = 0;
	}
}

void ViNoise::clear()
{
	resize(mMask->size());
}

int ViNoise::size() const
{
	return mMask->size();
}

qreal ViNoise::snr() const
{
	int signal = 0, noise = 0;
	for(int i = 0; i < mMask->size(); ++i)
	{
		if((*mMask)[i]) ++noise;
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
	for(i = 0; i < mMask->size(); ++i)
	{
		if((*mMask)[i])
		{
			if(noiseStarted && counter > 0)
			{
				last = (*mData)[i];
				end = i - counter;
				for(j = i - 1; j >= end; --j)
				{
					(*mMask)[j] = 1;
					(*mData)[j] = (first + last) / 2;
				}
				first = last;
				counter = 0;
			}
			else
			{
				first = (*mData)[i];
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

ViSampleChunk* ViNoise::data()
{
	return mData;
}

ViSampleChunk* ViNoise::mask()
{
	return mMask;
}

void ViNoise::generateMask(const MaskType &type)
{
	if(type == Direct) generateDirectMask();
	else if(type == Mean) generateMeanMask();
	else if(type == Maximum) generateMaximumMask();
}

void ViNoise::generateDirectMask()
{
	for(int i = 0; i < mData->size(); ++i)
	{
		if((*mData)[i] >= mThreshold) (*mMask)[i] = 1;
		else (*mMask)[i] = 0;
	}
}

void ViNoise::generateMeanMask()
{
	qreal mean = 0;
	for(int i = 0; i < mData->size(); ++i)
	{
		mean += (*mData)[i];
	}
	mean /= mData->size();
	mean /= (1-mThreshold);
	for(int i = 0; i < mData->size(); ++i)
	{
		if((*mData)[i] >= mean) (*mMask)[i] = 1;
		else (*mMask)[i] = 0;
	}
}

void ViNoise::generateMaximumMask()
{
	qreal maximum = 0;
	for(int i = 0; i < mData->size(); ++i)
	{
		if((*mData)[i] > maximum) maximum = (*mData)[i];
	}
	maximum *= mThreshold;
	for(int i = 0; i < mData->size(); ++i)
	{
		if((*mData)[i] >= maximum) (*mMask)[i] = 1;
		else (*mMask)[i] = 0;
	}
}

void ViNoise::set(const int &index, const qreal &value)
{
	if(value > 1) (*mData)[index] = 1;
	else (*mData)[index] = value;
}

void ViNoise::set(const int &index, const ViNoise &other)
{
	int newIndex;
	for(int i = 0; i < other.size(); ++i)
	{
		newIndex = index + i;
		(*mData)[newIndex] = (*other.mData)[i];
		(*mMask)[newIndex] = (*other.mMask)[i];
	}
}

const bool& ViNoise::at(const int &index) const
{
	const bool& value = (*mMask)[index];
	return value;
}

const bool& ViNoise::operator [] (const int &index) const
{
	const bool& value = (*mMask)[index];
	return value;
}
