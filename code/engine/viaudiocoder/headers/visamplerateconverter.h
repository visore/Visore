#ifndef VISAMPLERATECONVERTER_H
#define VISAMPLERATECONVERTER_H

#include <vicoder.h>

class ViSampleRateConverter
{

	public:

		enum Mode
		{
			HighViuality = 0,
			LowViuality = 1
		};

		ViSampleRateConverter();
		ViSampleRateConverter(short channels, qreal inputRate, qreal outputRate, ViSampleRateConverter::Mode mode = ViSampleRateConverter::HighViuality);
		~ViSampleRateConverter();
		bool initialize(short channels, qreal inputRate, qreal outputRate, ViSampleRateConverter::Mode mode = ViSampleRateConverter::HighViuality);
		int convert(const qfloat *input, qfloat *output, int samples);

	private:

		int (ViSampleRateConverter::*convertPointer)(const qfloat *input, qfloat *output, int samples);
		int copy(const qfloat *input, qfloat *output, int samples);
		int resample(const qfloat *input, qfloat *output, int samples);

	private:

		void *mResampler;
		qreal mFactor;

};

#endif
