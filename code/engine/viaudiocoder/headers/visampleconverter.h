#ifndef VISAMPLECONVERTER_H
#define VISAMPLECONVERTER_H

#include <viaudioformat.h>
#include <visamplechannelconverter.h>
#include <visamplesizeconverter.h>
#include <visamplerateconverter.h>

class ViSampleConverter
{

	public:

		ViSampleConverter();
		ViSampleConverter(ViAudioFormat inputFormat, ViAudioFormat outputFormat);
		bool initialize(ViAudioFormat inputFormat, ViAudioFormat outputFormat);
		void* convert(const void *input, int &samples, int &size);

	private:

		ViSampleSizeConverter mFloatConverter;
		ViSampleRateConverter mRateConverter;
		ViSampleSizeConverter mSizeConverter;
		ViSampleChannelConverter mChannelConverter;

		int mFloatSampleSize;
		int mRateSampleSize;
		int mSizeSampleSize;
		int mChannelSampleSize;

		qreal mFloatDifference;
		qreal mRateDifference;
		qreal mSizeDifference;
		qreal mChannelDifference;

};

#endif
