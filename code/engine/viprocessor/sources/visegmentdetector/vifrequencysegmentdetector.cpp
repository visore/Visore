#include <vifrequencysegmentdetector.h>

//Specifies how many of the frequencies have to be at least the user specified threshold.
//Eg: 0.8 means that at least 80% of the frequencies must be equal or higher that the value threshold.
#define RANGE_THRESHOLD 0.7

ViFrequencySegmentDetector::ViFrequencySegmentDetector()
	: ViSegmentDetector()
{
	setThreshold(ViSegmentDetector::SongStart, ViRange(0.05, 0.4), ViRange(0.00015, 1.0), 1000);
    setThreshold(ViSegmentDetector::SongEnd, ViRange(0.05, 0.5), ViRange(0.0, 0.00008), 1700);
	setThreshold(ViSegmentDetector::RecordStart, ViRange(0.05, 0.2), ViRange(0.0001, 1.0), 500);
	setThreshold(ViSegmentDetector::RecordEnd, ViRange(0.05, 0.3), ViRange(0.0, 0.00005), 7000);

	mAnalyzer.disableProgress();
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

void ViFrequencySegmentDetector::addSpectrum(ViRealSpectrum spectrum)
{
	mMutex.lock();
	mSpectrums.enqueue(spectrum);
	mMutex.unlock();
}

void ViFrequencySegmentDetector::initialize()
{
	mMutex.lock();
	if(isMultiShot())
	{
		mAnalyzer.setMultiShot();
	}
	QObject::connect(&mAnalyzer, SIGNAL(changed(ViRealSpectrum, qint64)), this, SLOT(addSpectrum(ViRealSpectrum)), Qt::DirectConnection);
	mTotalSamples = 0;
	mRecordStartTotalValue = 0;
	mRecordEndTotalValue = 0;
	mSongStartTotalValue = 0;
	mSongEndTotalValue = 0;
	mRecordStartAverages.clear();
	mRecordEndAverages.clear();
	mSongStartAverages.clear();
	mSongEndAverages.clear();
	mSpectrums.clear();
	mAnalyzer.process(object(), type1());
	mMutex.unlock();
}

void ViFrequencySegmentDetector::execute()
{
	ViSampleChunk &theSamples = samples();

	mTotalSamples += theSamples.size();
	mMutex.lock();
	bool isEmpty = mSpectrums.isEmpty();
	mMutex.unlock();
	while(!isEmpty)
	{
		mMutex.lock();
		ViRealSpectrum spectrum = mSpectrums.dequeue();
		mMutex.unlock();
		if(!mSongRunning)
		{
			updateRecordStartAverage(spectrum);
			if(	!mRecordRunning &&
				mRecordStartAverages.size() * spectrum.interval().position(ViAudioPosition::Milliseconds) >= mRecordStartTimeThreshold &&
				inRange(mRecordStartAverages, mRecordStartValueThreshold))
			{
				setRecordStart(ViAudioPosition(mTotalSamples, ViAudioPosition::Samples, format1()));
			}

			updateSongStartAverage(spectrum);

			if(	mRecordRunning &&
				mSongStartAverages.size() * spectrum.interval().position(ViAudioPosition::Milliseconds) >= mSongStartTimeThreshold &&
				inRange(mSongStartAverages, mSongStartValueThreshold))
			{
				setSongStart(ViAudioPosition(mTotalSamples, ViAudioPosition::Samples, format1()));
			}

			if(mRecordRunning)
			{
				updateRecordEndAverage(spectrum);
				if(	mRecordEndAverages.size() * spectrum.interval().position(ViAudioPosition::Milliseconds) >= mRecordEndTimeThreshold &&
					inRange(mRecordEndAverages, mRecordEndValueThreshold))
				{
					setRecordEnd(ViAudioPosition(mTotalSamples, ViAudioPosition::Samples, format1()));
                    //initialize();
				}
			}
		}
		else if(mRecordRunning)
		{
			updateSongEndAverage(spectrum);
			if(	mSongEndAverages.size() * spectrum.interval().position(ViAudioPosition::Milliseconds) >= mSongEndTimeThreshold &&
				inRange(mSongEndAverages, mSongEndValueThreshold))
			{
				setSongEnd(ViAudioPosition(mTotalSamples, ViAudioPosition::Samples, format1()));
			}
		}
		mMutex.lock();
		isEmpty = mSpectrums.isEmpty();
		mMutex.unlock();
	}
}

void ViFrequencySegmentDetector::finalize()
{
	mMutex.lock();
	QObject::disconnect(&mAnalyzer, SIGNAL(changed(ViRealSpectrum, qint64)), this, SLOT(addSpectrum(ViRealSpectrum)));
	if(isMultiShot())
	{
		mAnalyzer.stop();
	}
	mRecordStartAverages.clear();
	mRecordEndAverages.clear();
	mSongStartAverages.clear();
	mSongEndAverages.clear();
	mSpectrums.clear();
    mMutex.unlock();
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

void ViFrequencySegmentDetector::updateRecordStartAverage(ViRealSpectrum &spectrum)
{
	qreal total = 0;
	int start = mRecordStartRangeThreshold.start() * spectrum.size();
	int end = mRecordStartRangeThreshold.end() * spectrum.size();
	for(int i = start; i < end; ++i)
	{
		total += spectrum[i].polar().amplitude().real();
	}
	total /= end - start;

	if(mRecordStartAverages.size() >= mRecordStartTimeThreshold / spectrum.interval().position(ViAudioPosition::Milliseconds))
	{
		mRecordStartTotalValue -= mRecordStartAverages.dequeue();
	}
	mRecordStartTotalValue += total;
	mRecordStartAverages.enqueue(total);
}

void ViFrequencySegmentDetector::updateRecordEndAverage(ViRealSpectrum &spectrum)
{
	qreal total = 0;
	int start = mRecordEndRangeThreshold.start() * spectrum.size();
	int end = mRecordEndRangeThreshold.end() * spectrum.size();
	for(int i = start; i < end; ++i)
	{
		total += spectrum[i].polar().amplitude().real();
	}
	total /= end - start;

	if(mRecordEndAverages.size() >= mRecordEndTimeThreshold / spectrum.interval().position(ViAudioPosition::Milliseconds))
	{
		mRecordEndTotalValue -= mRecordEndAverages.dequeue();
	}
	mRecordEndTotalValue += total;
	mRecordEndAverages.enqueue(total);
}

void ViFrequencySegmentDetector::updateSongStartAverage(ViRealSpectrum &spectrum)
{
	qreal total = 0;
	int start = mSongStartRangeThreshold.start() * spectrum.size();
	int end = mSongStartRangeThreshold.end() * spectrum.size();
	for(int i = start; i < end; ++i)
	{
		total += spectrum[i].polar().amplitude().real();
	}
	total /= end - start;

	if(mSongStartAverages.size() >= mSongStartTimeThreshold / spectrum.interval().position(ViAudioPosition::Milliseconds))
	{
		mSongStartTotalValue -= mSongStartAverages.dequeue();
	}
	mSongStartTotalValue += total;
	mSongStartAverages.enqueue(total);
}

void ViFrequencySegmentDetector::updateSongEndAverage(ViRealSpectrum &spectrum)
{
	qreal total = 0;
	int start = mSongEndRangeThreshold.start() * spectrum.size();
	int end = mSongEndRangeThreshold.end() * spectrum.size();
	for(int i = start; i < end; ++i)
	{
		total += spectrum[i].polar().amplitude().real();
	}
	total /= end - start;

	if(mSongEndAverages.size() >= mSongEndTimeThreshold / spectrum.interval().position(ViAudioPosition::Milliseconds))
	{
		mSongEndTotalValue -= mSongEndAverages.dequeue();
	}
	mSongEndTotalValue += total;
	mSongEndAverages.enqueue(total);
}
