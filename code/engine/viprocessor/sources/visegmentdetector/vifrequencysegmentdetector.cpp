#include <vifrequencysegmentdetector.h>

//Specifies how many of the frequencies have to be at least the user specified threshold.
//Eg: 0.8 means that at least 80% of the frequencies must be equal or higher that the value threshold.
#define RANGE_THRESHOLD 0.8

ViFrequencySegmentDetector::ViFrequencySegmentDetector()
	: ViSegmentDetector(ViProcessor::Combined)
{
	/*setThreshold(ViSegmentDetector::SongStart, ViRange(0.05, 0.4), ViRange(0.00015, 1.0), 1000);
	setThreshold(ViSegmentDetector::SongEnd, ViRange(0.05, 0.5), ViRange(0.0, 0.00008), 2000);
	setThreshold(ViSegmentDetector::RecordStart, ViRange(0.0, 0.4), ViRange(0.0001, 1.0), 500);
	setThreshold(ViSegmentDetector::RecordEnd, ViRange(0.05, 0.3), ViRange(0.0, 0.00005), 10000);*/

	setThreshold(ViSegmentDetector::SongStart, ViRange(0.02, 0.05), ViRange(0.0001, 1.0), 500);
	setThreshold(ViSegmentDetector::SongEnd, ViRange(0.02, 0.1), ViRange(0.0, 0.00012), 2000);
	setThreshold(ViSegmentDetector::RecordStart, ViRange(0, 0.01), ViRange(0.0015, 1.0), 500);
	setThreshold(ViSegmentDetector::RecordEnd, ViRange(0.02, 0.1), ViRange(0.0, 0.0001), 10000);
}

void ViFrequencySegmentDetector::setThreshold(ViSegmentDetector::Type type, ViRange rangeThreshold, ViRange valueThreshold, qint64 timeThreshold)
{
	if(type == ViSegmentDetector::SongStart)
	{
		mSongStartRangeThreshold = rangeThreshold;
		mSongStartValueThreshold = valueThreshold;
		mSongStartTimeThreshold = timeThreshold;
	}
	else if(type == ViSegmentDetector::SongEnd)
	{
		mSongEndRangeThreshold = rangeThreshold;
		mSongEndValueThreshold = valueThreshold;
		mSongEndTimeThreshold = timeThreshold;
	}
	else if(type == ViSegmentDetector::RecordStart)
	{
		mRecordStartRangeThreshold = rangeThreshold;
		mRecordStartValueThreshold = valueThreshold;
		mRecordStartTimeThreshold = timeThreshold;
	}
	else if(type == ViSegmentDetector::RecordEnd)
	{
		mRecordEndRangeThreshold = rangeThreshold;
		mRecordEndValueThreshold = valueThreshold;
		mRecordEndTimeThreshold = timeThreshold;
	}
}

void ViFrequencySegmentDetector::clear()
{
	mRecordStartAverages.clear();
	mRecordEndAverages.clear();
	mSongStartAverages.clear();
	mSongEndAverages.clear();
	mTotalSamples = 0;
}

void ViFrequencySegmentDetector::initialize()
{
	clear();
}

void ViFrequencySegmentDetector::execute(const int &channel)
{
	int sampleCount = data().sampleCount();
	ViRealSpectrum spectrum(sampleCount, format(), currentFrequencies(), true);
	mTotalSamples += sampleCount;
	int milliseconds = ViAudioPosition::convertPosition(sampleCount, ViAudioPosition::Samples, ViAudioPosition::Milliseconds, format());

	if(mRecordRunning)
	{
		if(mSongRunning)
		{
			bool isFull = mSongEndAverages.size() >= mSongEndTimeThreshold / milliseconds;
			updateSongEndAverage(spectrum, isFull);
			if(isFull && inRange(mSongEndAverages, mSongEndValueThreshold))
			{
				setSongEnd(ViAudioPosition(mTotalSamples, ViAudioPosition::Samples, format()));
				clear();
			}
		}
		else
		{
			bool isFull = mSongStartAverages.size() >= mSongStartTimeThreshold / milliseconds;
			updateSongStartAverage(spectrum, isFull);
			if(isFull && inRange(mSongStartAverages, mSongStartValueThreshold))
			{
				setSongStart(ViAudioPosition(mTotalSamples, ViAudioPosition::Samples, format()));
				clear();
			}
			else
			{
				isFull = mRecordEndAverages.size() >= mRecordEndTimeThreshold / milliseconds;
				updateRecordEndAverage(spectrum, isFull);
				if(isFull && inRange(mRecordEndAverages, mRecordEndValueThreshold))
				{
					setRecordEnd(ViAudioPosition(mTotalSamples, ViAudioPosition::Samples, format()));
					clear();
				}
			}
		}
	}
	else
	{
		bool isFull = mRecordStartAverages.size() >= mRecordStartTimeThreshold / milliseconds;
		updateRecordStartAverage(spectrum, isFull);
		if(isFull && inRange(mRecordStartAverages, mRecordStartValueThreshold))
		{
			setRecordStart(ViAudioPosition(mTotalSamples, ViAudioPosition::Samples, format()));
			clear();
		}
	}
}

void ViFrequencySegmentDetector::finalize()
{
	clear();
}

bool ViFrequencySegmentDetector::inRange(QQueue<qreal> &averages, ViRange &range)
{
	int counter = 0;
	for(int i = 0; i < averages.size(); ++i)
	{
		if(range.contains(averages[i]))
		{
			++counter;
		}
	}
	if(counter / qreal(averages.size()) > RANGE_THRESHOLD)
	{
		return true;
	}
	return false;
}

void ViFrequencySegmentDetector::updateRecordStartAverage(const ViRealSpectrum &spectrum, const bool &isFull)
{
	qreal total = 0;
	int start = mRecordStartRangeThreshold.start() * spectrum.size();
	int end = mRecordStartRangeThreshold.end() * spectrum.size();
	for(int i = start; i < end; ++i)
	{
		total += spectrum[i].polar().amplitude().real();
	}
	total /= end - start;
	if(isFull)
	{
		mRecordStartAverages.dequeue();
	}
	mRecordStartAverages.enqueue(total);
}

void ViFrequencySegmentDetector::updateRecordEndAverage(const ViRealSpectrum &spectrum, const bool &isFull)
{
	qreal total = 0;
	int start = mRecordEndRangeThreshold.start() * spectrum.size();
	int end = mRecordEndRangeThreshold.end() * spectrum.size();
	for(int i = start; i < end; ++i)
	{
		total += spectrum[i].polar().amplitude().real();
	}
	total /= end - start;

	if(isFull)
	{
		mRecordEndAverages.dequeue();
	}
	mRecordEndAverages.enqueue(total);
}

void ViFrequencySegmentDetector::updateSongStartAverage(const ViRealSpectrum &spectrum, const bool &isFull)
{
	qreal total = 0;
	int start = mSongStartRangeThreshold.start() * spectrum.size();
	int end = mSongStartRangeThreshold.end() * spectrum.size();
	for(int i = start; i < end; ++i)
	{
		total += spectrum[i].polar().amplitude().real();
	}
	total /= end - start;

	if(isFull)
	{
		mSongStartAverages.dequeue();
	}
	mSongStartAverages.enqueue(total);
}

void ViFrequencySegmentDetector::updateSongEndAverage(const ViRealSpectrum &spectrum, const bool &isFull)
{
	qreal total = 0;
	int start = mSongEndRangeThreshold.start() * spectrum.size();
	int end = mSongEndRangeThreshold.end() * spectrum.size();
	for(int i = start; i < end; ++i)
	{
		total += spectrum[i].polar().amplitude().real();
	}
	total /= end - start;

	if(isFull)
	{
		mSongEndAverages.dequeue();
	}
	mSongEndAverages.enqueue(total);
}
