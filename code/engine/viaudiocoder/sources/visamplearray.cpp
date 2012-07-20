#include <visamplearray.h>

ViSampleArray::ViSampleArray()
{
	mSamples = 0;
	mSize = 0;
	mData = NULL;
}

ViSampleArray::ViSampleArray(const ViSampleArray &other)
{
	mSamples = other.mSamples;
	mSize = other.mSize;
	memcpy(mData, other.mData, mSize);
}

ViSampleArray::ViSampleArray(const QByteArray &other)
{
	mSamples = 0;
	mSize = other.size();
	mData = new qbyte[mSize];
	memcpy(mData, other.data(), mSize);
}

ViSampleArray::ViSampleArray(char *data, int size, int samples)
{
	mData = NULL;
	setData(data, size, samples);
}

ViSampleArray::ViSampleArray(qbyte8u *data, int size, int samples)
{
	mData = NULL;
	setData(data, size, samples);
}

ViSampleArray::ViSampleArray(qbyte16u *data, int size, int samples)
{
	mData = NULL;
	setData(data, size, samples);
}

ViSampleArray::ViSampleArray(qbyte32u *data, int size, int samples)
{
	mData = NULL;
	setData(data, size, samples);
}

ViSampleArray::ViSampleArray(qbyte8s *data, int size, int samples)
{
	mData = NULL;
	setData(data, size, samples);
}

ViSampleArray::ViSampleArray(qbyte16s *data, int size, int samples)
{
	mData = NULL;
	setData(data, size, samples);
}

ViSampleArray::ViSampleArray(qbyte32s *data, int size, int samples)
{
	mData = NULL;
	setData(data, size, samples);
}

ViSampleArray::ViSampleArray(qfloat *data, int size, int samples)
{
	mData = NULL;
	setData(data, size, samples);
}

ViSampleArray::ViSampleArray(qreal *data, int size, int samples)
{
	mData = NULL;
	setData(data, size, samples);
}

ViSampleArray::~ViSampleArray()
{
	clearData();
}

void ViSampleArray::setData(char *data, int size)
{
	clearData();
	mData = data;
	mSize = size;
}

void ViSampleArray::setData(qbyte8u *data, int size)
{
	clearData();
	mData = data;
	mSize = size * sizeof(qbyte8u);
}

void ViSampleArray::setData(qbyte16u *data, int size)
{
	clearData();
	mData = data;
	mSize = size * sizeof(qbyte16u);
}

void ViSampleArray::setData(qbyte32u *data, int size)
{
	clearData();
	mData = data;
	mSize = size * sizeof(qbyte32u);
}

void ViSampleArray::setData(qbyte8s *data, int size)
{
	clearData();
	mData = data;
	mSize = size * sizeof(qbyte8s);
}

void ViSampleArray::setData(qbyte16s *data, int size)
{
	clearData();
	mData = data;
	mSize = size * sizeof(qbyte16s);
}

void ViSampleArray::setData(qbyte32s *data, int size)
{
	clearData();
	mData = data;
	mSize = size * sizeof(qbyte32s);
}

void ViSampleArray::setData(qfloat *data, int size)
{
	clearData();
	mData = data;
	mSize = size * sizeof(qfloat);
}

void ViSampleArray::setData(qreal *data, int size)
{
	clearData();
	mData = data;
	mSize = size * sizeof(qreal);
}

void ViSampleArray::setData(char *data, int size, int samples)
{
	clearData();
	mData = data;
	mSize = size;
	mSamples = samples;
}

void ViSampleArray::setData(qbyte8u *data, int size, int samples)
{
	clearData();
	mData = data;
	mSize = size * sizeof(qbyte8u);
	mSamples = samples;
}

void ViSampleArray::setData(qbyte16u *data, int size, int samples)
{
	clearData();
	mData = data;
	mSize = size * sizeof(qbyte16u);
	mSamples = samples;
}

void ViSampleArray::setData(qbyte32u *data, int size, int samples)
{
	clearData();
	mData = data;
	mSize = size * sizeof(qbyte32u);
	mSamples = samples;
}

void ViSampleArray::setData(qbyte8s *data, int size, int samples)
{
	clearData();
	mData = data;
	mSize = size * sizeof(qbyte8s);
	mSamples = samples;
}

void ViSampleArray::setData(qbyte16s *data, int size, int samples)
{
	clearData();
	mData = data;
	mSize = size * sizeof(qbyte16s);
	mSamples = samples;
}

void ViSampleArray::setData(qbyte32s *data, int size, int samples)
{
	clearData();
	mData = data;
	mSize = size * sizeof(qbyte32s);
	mSamples = samples;
}

void ViSampleArray::setData(qfloat *data, int size, int samples)
{
	clearData();
	mData = data;
	mSize = size * sizeof(qfloat);
	mSamples = samples;
}

void ViSampleArray::setData(qreal *data, int size, int samples)
{
	clearData();
	mData = data;
	mSize = size * sizeof(qreal);
	mSamples = samples;
}

int ViSampleArray::samples()
{
	return mSamples;
}

void ViSampleArray::setSamples(int samples)
{
	mSamples = samples;
}

int ViSampleArray::size()
{
	return mSize;
}

void* ViSampleArray::data()
{
	return mData;
}

char* ViSampleArray::charData()
{
	return (char*) mData;
}

void ViSampleArray::clear()
{
	mSize = 0;
	mSamples = 0;
	clearData();
}

void ViSampleArray::clearData()
{
	if(mData != NULL)
	{
		delete [] (char*) mData;
		mData = NULL;
	}
}
