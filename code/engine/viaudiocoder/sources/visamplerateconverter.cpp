#include <visamplerateconverter.h>
#include <libresample.h>

ViSampleRateConverter::ViSampleRateConverter()
{
	mResampler = NULL;
	mFactor = 1;
}

ViSampleRateConverter::ViSampleRateConverter(short channels, qreal inputRate, qreal outputRate, ViSampleRateConverter::Mode mode)
{
	ViSampleRateConverter();
	initialize(channels, inputRate, outputRate, mode);
}

ViSampleRateConverter::~ViSampleRateConverter()
{
	if(mResampler != NULL)
	{
		resample_close(mResampler);
		mResampler = NULL;
	}
}

bool ViSampleRateConverter::initialize(short channels, qreal inputRate, qreal outputRate, ViSampleRateConverter::Mode mode)
{
	if(mResampler != NULL)
	{
		resample_close(mResampler);
	}

	mFactor = outputRate / inputRate;

	if(mFactor == 1)
	{
		convertPointer = &ViSampleRateConverter::copy;
	}
	else
	{
		convertPointer = &ViSampleRateConverter::resample;
		mResampler = resample_open(mode == ViSampleRateConverter::HighViuality, mFactor, mFactor);
	}

	return true;
}

void ViSampleRateConverter::convert(const qfloat *input, qfloat *output, int samples)
{
	(this->*convertPointer)(input, output, samples);
}

void ViSampleRateConverter::copy(const qfloat *input, qfloat *output, int samples)
{
	memcpy(output, input, sizeof(qfloat) * samples);
}

void ViSampleRateConverter::resample(const qfloat *input, qfloat *output, int samples)
{
	int used;
	resample_process(mResampler, mFactor, (float*) input, samples, 0, &used, output, ceil(samples * mFactor));
}
