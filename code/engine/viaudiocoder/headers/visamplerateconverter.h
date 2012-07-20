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
		void convert(const qfloat *input, qfloat *output, int samples);

	private:

		void (ViSampleRateConverter::*convertPointer)(const qfloat *input, qfloat *output, int samples);
		void copy(const qfloat *input, qfloat *output, int samples);
		void resample(const qfloat *input, qfloat *output, int samples);

	private:

		void *mResampler;
		qreal mFactor;

};

#endif
