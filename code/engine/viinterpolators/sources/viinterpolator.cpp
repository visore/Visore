#include <viinterpolator.h>
#include<vilogger.h>
ViInterpolator::ViInterpolator()
	: ViLibrary()
{
}

ViInterpolator::ViInterpolator(const ViInterpolator &other)
	: ViLibrary(other)
{
}

ViInterpolator::~ViInterpolator()
{
}

bool ViInterpolator::interpolate(ViSampleChunk &samples, const ViNoise &noise)
{
	bool signalStarted = false, success = true;
	int end, noiseLength = 0, noiseStart = -1;

	for(int i = 0; i < noise.size(); ++i)
	{
		if(noise[i])
		{
			if(signalStarted)
			{
				end = noiseStart + noiseLength;
				success &= interpolateSamples(samples.data(), noiseStart, samples.data() + end, noise.size() - end, samples.data() + noiseStart, noiseLength);

				noiseStart = i;
				noiseLength = 1;
				signalStarted = false;
			}
			else
			{
				if(noiseStart < 0)
				{
					noiseStart = i;
				}
				++noiseLength;
			}
		}
		else if(noiseStart >= 0)
		{
			signalStarted = true;
		}
	}

	if(noiseStart >= 0)
	{
		end = noiseStart + noiseLength;
		success &= interpolateSamples(samples.data(), noiseStart, samples.data() + end, noise.size() - end, samples.data() + noiseStart, noiseLength);
	}

	return success;
}
