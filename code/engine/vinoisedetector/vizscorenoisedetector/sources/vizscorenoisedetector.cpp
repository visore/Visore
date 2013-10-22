#include <vizscorenoisedetector.h>
#include <qmath.h>
#include <viscaler.h>

ViZscoreNoiseDetector::ViZscoreNoiseDetector()
	: ViNoiseDetector()
{
}

void ViZscoreNoiseDetector::calculateNoise(const ViSampleChunk &samples)
{
	int i, size = samples.size();

	ViSampleChunk newSamples(size);
	for(i = 0; i < size; ++i)
	{
		newSamples[i] = ViScaler<qreal>::scale(samples[i], -1, 1, 0, 100);
	}

	// Calculate the mean
	qreal mean = 0;
	for(i = 0; i < size; ++i)
	{
		mean += newSamples[i];
	}
	mean /= size;

	// Calculate the standard diviation
	qreal standardDeviation = 0;
	for(i = 0; i < size; ++i)
	{
		standardDeviation += qPow(newSamples[i] - mean, 2);
	}
	standardDeviation = qSqrt(standardDeviation / size);

	// Calculate the z-score
	qreal value;
	for(i = 0; i < size; ++i)
	{
		value = qAbs((newSamples[i] - mean) / standardDeviation);


		//value = qAbs(qAbs(newSamples[i] - mean) - standardDeviation);


		//value = qAbs((newSamples[i] - mean));
		//cout<<newSamples[i]<<"  "<<mean<<"  "<<standardDeviation<<"  "<< value<<"  "<<endl;
		//value = qAbs((samples[i] - mean) );
		value /= 3.5;
/*value = log(value);
if(value < 0)value = 0;
if(value>1)value = 1;*/
		//value = ViScaler<qreal>::scale(value, 0, 2, 0, 1);
		setNoise(value);
	}


	/*int t = 0;
	int y = 8/t;*/
/*
	int i, size = samples.size();

		ViSampleChunk newSamples(size);
		QVector<qreal> v(size);
		for(i = 0; i < size; ++i)
		{
			newSamples[i] = ViScaler<qreal>::scale(samples[i], -1, 1, 0, 1);
			v[i] = newSamples[i];
		}
		qSort(v);
		qreal median = v[v.size()/2];

		// Calculate the mean
		qreal mean = 0;
		for(i = 0; i < size; ++i)
		{
			mean += newSamples[i];
		}
		mean /= size;

		// Calculate the standard diviation
		qreal mad = 0;
		QVector<qreal> v2(size);
		for(i = 0; i < size; ++i)
		{
			v2[i] = qAbs(newSamples[i] - median);
			//standardDeviation += qPow(newSamples[i] - mean, 2);
		}
		qSort(v2);
		mad = v2[v2.size()/2];

		// Calculate the z-score
		qreal value;
		for(i = 0; i < size; ++i)
		{
			//value = qAbs((newSamples[i] - mean) / standardDeviation);
			//cout<<newSamples[i]<<"  "<<mean<<"  "<<standardDeviation<<"  "<< value<<"  "<<log(value)<<endl;
			//value = qAbs((samples[i] - mean) );
			value = (0.6745 * (newSamples[i] - median)) / mad;

			value /= 3.5;

			//value = ViScaler<qreal>::scale(value, 0, 2, 0, 1);
			noise.set(i, value);
		}*/
}

ViZscoreNoiseDetector* ViZscoreNoiseDetector::clone()
{
	return new ViZscoreNoiseDetector(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViZscoreNoiseDetector* create()
{
	return new ViZscoreNoiseDetector();
}

#ifdef __cplusplus
}
#endif
