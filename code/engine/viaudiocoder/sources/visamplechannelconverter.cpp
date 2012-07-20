#include <visamplechannelconverter.h>

ViSampleChannelConverter::ViSampleChannelConverter()
{
	convert = NULL;
}

ViSampleChannelConverter::ViSampleChannelConverter(int inputChannels, int outputChannels, ViAudioFormat::SampleType sampleType, int sampleSize)
{
	ViSampleChannelConverter();
	initialize(inputChannels, outputChannels, sampleType, sampleSize);
}

bool ViSampleChannelConverter::initialize(int inputChannels, int outputChannels, ViAudioFormat::SampleType sampleType, int sampleSize)
{
	convert = NULL;

	if(inputChannels == 1)
	{
		if(outputChannels == 1)
		{
			if(sampleType == ViAudioFormat::Float)
			{
				convert = &ViChannelConverter<qfloat>::convertMonoToMono;
			}
			else if(sampleType == ViAudioFormat::Real)
			{
				convert = &ViChannelConverter<qreal>::convertMonoToMono;
			}
			else if(sampleType == ViAudioFormat::SignedInt)
			{
				if(sampleSize == 8) convert = &ViChannelConverter<qbyte8s>::convertMonoToMono;
				else if(sampleSize == 16) convert = &ViChannelConverter<qbyte16s>::convertMonoToMono;
				else if(sampleSize == 32) convert = &ViChannelConverter<qbyte32s>::convertMonoToMono;
			}
			else if(sampleType == ViAudioFormat::UnSignedInt)
			{
				if(sampleSize == 8) convert = &ViChannelConverter<qbyte8u>::convertMonoToMono;
				else if(sampleSize == 16) convert = &ViChannelConverter<qbyte16u>::convertMonoToMono;
				else if(sampleSize == 32) convert = &ViChannelConverter<qbyte32u>::convertMonoToMono;
			}
		}
		else if(outputChannels == 2)
		{
			if(sampleType == ViAudioFormat::Float)
			{
				convert = &ViChannelConverter<qfloat>::convertMonoToStereo;
			}
			else if(sampleType == ViAudioFormat::Real)
			{
				convert = &ViChannelConverter<qreal>::convertMonoToStereo;
			}
			else if(sampleType == ViAudioFormat::SignedInt)
			{
				if(sampleSize == 8) convert = &ViChannelConverter<qbyte8s>::convertMonoToStereo;
				else if(sampleSize == 16) convert = &ViChannelConverter<qbyte16s>::convertMonoToStereo;
				else if(sampleSize == 32) convert = &ViChannelConverter<qbyte32s>::convertMonoToStereo;
			}
			else if(sampleType == ViAudioFormat::UnSignedInt)
			{
				if(sampleSize == 8) convert = &ViChannelConverter<qbyte8u>::convertMonoToStereo;
				else if(sampleSize == 16) convert = &ViChannelConverter<qbyte16u>::convertMonoToStereo;
				else if(sampleSize == 32) convert = &ViChannelConverter<qbyte32u>::convertMonoToStereo;
			}
		}
	}
	else if(inputChannels == 2)
	{
		if(outputChannels == 1)
		{
			if(sampleType == ViAudioFormat::Float)
			{
				convert = &ViChannelConverter<qfloat>::convertStereoToMono;
			}
			else if(sampleType == ViAudioFormat::Real)
			{
				convert = &ViChannelConverter<qreal>::convertStereoToMono;
			}
			else if(sampleType == ViAudioFormat::SignedInt)
			{
				if(sampleSize == 8) convert = &ViChannelConverter<qbyte8s>::convertStereoToMono;
				else if(sampleSize == 16) convert = &ViChannelConverter<qbyte16s>::convertStereoToMono;
				else if(sampleSize == 32) convert = &ViChannelConverter<qbyte32s>::convertStereoToMono;
			}
			else if(sampleType == ViAudioFormat::UnSignedInt)
			{
				if(sampleSize == 8) convert = &ViChannelConverter<qbyte8u>::convertStereoToMono;
				else if(sampleSize == 16) convert = &ViChannelConverter<qbyte16u>::convertStereoToMono;
				else if(sampleSize == 32) convert = &ViChannelConverter<qbyte32u>::convertStereoToMono;
			}
		}
		else if(outputChannels == 2)
		{
			if(sampleType == ViAudioFormat::Float)
			{
				convert = &ViChannelConverter<qfloat>::convertStereoToStereo;
			}
			else if(sampleType == ViAudioFormat::Real)
			{
				convert = &ViChannelConverter<qreal>::convertStereoToStereo;
			}
			else if(sampleType == ViAudioFormat::SignedInt)
			{
				if(sampleSize == 8) convert = &ViChannelConverter<qbyte8s>::convertStereoToStereo;
				else if(sampleSize == 16) convert = &ViChannelConverter<qbyte16s>::convertStereoToStereo;
				else if(sampleSize == 32) convert = &ViChannelConverter<qbyte32s>::convertStereoToStereo;
			}
			else if(sampleType == ViAudioFormat::UnSignedInt)
			{
				if(sampleSize == 8) convert = &ViChannelConverter<qbyte8u>::convertStereoToStereo;
				else if(sampleSize == 16) convert = &ViChannelConverter<qbyte16u>::convertStereoToStereo;
				else if(sampleSize == 32) convert = &ViChannelConverter<qbyte32u>::convertStereoToStereo;
			}
		}
	}

	return convert != NULL;
}
