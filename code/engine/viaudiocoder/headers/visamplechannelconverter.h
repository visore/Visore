#ifndef VISAMPLECHANNELCONVERTER_H
#define VISAMPLECHANNELCONVERTER_H

#include <vicoder.h>
#include <viaudioformat.h>
#include <vichannelconverter.h>

class ViSampleChannelConverter
{

	public:

		ViSampleChannelConverter();
		ViSampleChannelConverter(int inputChannels, int outputChannels, ViAudioFormat::SampleType sampleType, int sampleSize);
		bool initialize(int inputChannels, int outputChannels, ViAudioFormat::SampleType sampleType, int sampleSize);
		void (*convert)(const void *input, void *output, int samples);

};

#endif
