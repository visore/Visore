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
	setThreshold(ViSegmentDetector::SongEnd, ViRange(0.02, 0.1), ViRange(0.0, 0.0001), 2000);
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
	mRecordStartTotalValue = 0;
	mRecordEndTotalValue = 0;
	mSongStartTotalValue = 0;
	mSongEndTotalValue = 0;
	mRecordStartAverages.clear();
	mRecordEndAverages.clear();
	mSongStartAverages.clear();
	mSongEndAverages.clear();
}

void ViFrequencySegmentDetector::initialize()
{
	mTotalSamples = 0;
	clear();
}

void ViFrequencySegmentDetector::execute(const int &channel)
{
	int sampleCount = data().sampleCount();
	ViRealSpectrum spectrum(sampleCount, format(), currentFrequencies());
	mTotalSamples += sampleCount;
	int milliseconds = ViAudioPosition::convertPosition(sampleCount, ViAudioPosition::Samples, ViAudioPosition::Milliseconds, format());

	if(!mSongRunning)
	{
		updateRecordStartAverage(spectrum, milliseconds);
		if(	!mRecordRunning &&
			mRecordStartAverages.size() >= mRecordStartTimeThreshold / milliseconds &&
			inRange(mRecordStartAverages, mRecordStartValueThreshold))
		{
			clear();
			setRecordStart(ViAudioPosition(mTotalSamples, ViAudioPosition::Samples, format()));
		}

		updateSongStartAverage(spectrum, milliseconds);
		if(	mRecordRunning &&
			mSongStartAverages.size() >= mSongStartTimeThreshold / milliseconds &&
			inRange(mSongStartAverages, mSongStartValueThreshold))
		{
			clear();
			setSongStart(ViAudioPosition(mTotalSamples, ViAudioPosition::Samples, format()));
		}

		if(mRecordRunning)
		{
			updateRecordEndAverage(spectrum, milliseconds);
			if(	mRecordEndAverages.size() >= mRecordEndTimeThreshold / milliseconds &&
				inRange(mRecordEndAverages, mRecordEndValueThreshold))
			{
				clear();
				setRecordEnd(ViAudioPosition(mTotalSamples, ViAudioPosition::Samples, format()));
			}
		}
	}
	else if(mRecordRunning)
	{
		updateSongEndAverage(spectrum, milliseconds);
		if(	mSongEndAverages.size() >= mSongEndTimeThreshold / milliseconds &&
			inRange(mSongEndAverages, mSongEndValueThreshold))
		{
			clear();
			setSongEnd(ViAudioPosition(mTotalSamples, ViAudioPosition::Samples, format()));
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

void ViFrequencySegmentDetector::updateRecordStartAverage(const ViRealSpectrum &spectrum, const int &milliseconds)
{
	qreal total = 0;
	int start = mRecordStartRangeThreshold.start() * spectrum.size();
	int end = mRecordStartRangeThreshold.end() * spectrum.size();
	for(int i = start; i < end; ++i)
	{
		total += spectrum[i].polar().amplitude().real();
	}
	total /= end - start;
	if(mRecordStartAverages.size() >= mRecordStartTimeThreshold / milliseconds)
	{
		mRecordStartTotalValue -= mRecordStartAverages.dequeue();
	}
	mRecordStartTotalValue += total;
	mRecordStartAverages.enqueue(total);
}

void ViFrequencySegmentDetector::updateRecordEndAverage(const ViRealSpectrum &spectrum, const int &milliseconds)
{
	qreal total = 0;
	int start = mRecordEndRangeThreshold.start() * spectrum.size();
	int end = mRecordEndRangeThreshold.end() * spectrum.size();
	for(int i = start; i < end; ++i)
	{
		total += spectrum[i].polar().amplitude().real();
	}
	total /= end - start;

	if(mRecordEndAverages.size() >= mRecordEndTimeThreshold / milliseconds)
	{
		mRecordEndTotalValue -= mRecordEndAverages.dequeue();
	}
	mRecordEndTotalValue += total;
	mRecordEndAverages.enqueue(total);
}

void ViFrequencySegmentDetector::updateSongStartAverage(const ViRealSpectrum &spectrum, const int &milliseconds)
{
	qreal total = 0;
	int start = mSongStartRangeThreshold.start() * spectrum.size();
	int end = mSongStartRangeThreshold.end() * spectrum.size();
	for(int i = start; i < end; ++i)
	{
		total += spectrum[i].polar().amplitude().real();
	}
	total /= end - start;

	if(mSongStartAverages.size() >= mSongStartTimeThreshold / milliseconds)
	{
		mSongStartTotalValue -= mSongStartAverages.dequeue();
	}
	mSongStartTotalValue += total;
	mSongStartAverages.enqueue(total);
}

void ViFrequencySegmentDetector::updateSongEndAverage(const ViRealSpectrum &spectrum, const int &milliseconds)
{
	qreal total = 0;
	int start = mSongEndRangeThreshold.start() * spectrum.size();
	int end = mSongEndRangeThreshold.end() * spectrum.size();
	for(int i = start; i < end; ++i)
	{
		total += spectrum[i].polar().amplitude().real();
	}
	total /= end - start;

	if(mSongEndAverages.size() >= mSongEndTimeThreshold / milliseconds)
	{
		mSongEndTotalValue -= mSongEndAverages.dequeue();
	}
	mSongEndTotalValue += total;
	mSongEndAverages.enqueue(total);
}
