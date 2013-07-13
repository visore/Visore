#ifndef VIFREQUENCYSEGMENTDETECTOR_H
#define VIFREQUENCYSEGMENTDETECTOR_H

#include <visegmentdetector.h>
#include <vispectrum.h>
#include <viaverager.h>
#include <virange.h>
#include <QQueue>

class ViFrequencySegmentDetector : public ViSegmentDetector
{

	public:

		ViFrequencySegmentDetector();

		// rangeThreshold: Range of the spectrum that will be observered
		// valueThreshold: Range the average amplitude should fall in
		// timeThreshold: The number of milliseconds the spectrum should have a 'valueThreshold' average.
		void setThreshold(ViSegmentDetector::Type type, ViRange rangeThreshold, ViRange valueThreshold, qint64 timeThreshold);

		void initialize();
		void execute(const int &channel);
		void finalize();

	private:

		void clear();

		bool inRange(QQueue<qreal> &averages, ViRange &range);

		void updateRecordStartAverage(const ViRealSpectrum &spectrum, const bool &isFull);
		void updateRecordEndAverage(const ViRealSpectrum &spectrum, const bool &isFull);
		void updateSongStartAverage(const ViRealSpectrum &spectrum, const bool &isFull);
		void updateSongEndAverage(const ViRealSpectrum &spectrum, const bool &isFull);

	private:

		ViRange mRecordStartRangeThreshold;
		ViRange mRecordStartValueThreshold;
		qint64 mRecordStartTimeThreshold;
		ViRange mRecordEndRangeThreshold;
		ViRange mRecordEndValueThreshold;
		qint64 mRecordEndTimeThreshold;

		ViRange mSongStartRangeThreshold;
		ViRange mSongStartValueThreshold;
		qint64 mSongStartTimeThreshold;
		ViRange mSongEndRangeThreshold;
		ViRange mSongEndValueThreshold;
		qint64 mSongEndTimeThreshold;

		qint64 mTotalSamples;

		QQueue<qreal> mRecordStartAverages;
		QQueue<qreal> mRecordEndAverages;
		QQueue<qreal> mSongStartAverages;
		QQueue<qreal> mSongEndAverages;

};

#endif
