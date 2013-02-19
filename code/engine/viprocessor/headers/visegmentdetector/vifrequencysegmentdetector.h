#ifndef VIFREQUENCYSEGMENTDETECTOR_H
#define VIFREQUENCYSEGMENTDETECTOR_H

#include <visegmentdetector.h>
#include <vispectrumanalyzer.h>
#include <vispectrum.h>
#include <viaverager.h>
#include <virange.h>
#include <QQueue>

class ViFrequencySegmentDetector : public ViSegmentDetector
{

	Q_OBJECT

	public slots:

		void addSpectrum(ViRealSpectrum spectrums);

	public:

		ViFrequencySegmentDetector();

		// rangeThreshold: Range of the spectrum that will be observered
		// valueThreshold: Range the average amplitude should fall in
		// timeThreshold: The number of milliseconds the spectrum should have a 'valueThreshold' average.
		void setThreshold(ViSegmentDetector::Type type, ViRange rangeThreshold, ViRange valueThreshold, qint64 timeThreshold);

		void initialize();
		void execute();
		void finalize();

	private:

		bool inRange(QQueue<qreal> &averages, ViRange &range);

		void updateRecordStartAverage(ViRealSpectrum &spectrum);
		void updateRecordEndAverage(ViRealSpectrum &spectrum);
		void updateSongStartAverage(ViRealSpectrum &spectrum);
		void updateSongEndAverage(ViRealSpectrum &spectrum);

	private:

		ViSpectrumAnalyzer mAnalyzer;

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

		qreal mRecordStartTotalValue;
		qreal mRecordEndTotalValue;
		qreal mSongStartTotalValue;
		qreal mSongEndTotalValue;

		qint64 mTotalSamples;

		QQueue<qreal> mRecordStartAverages;
		QQueue<qreal> mRecordEndAverages;
		QQueue<qreal> mSongStartAverages;
		QQueue<qreal> mSongEndAverages;

		QQueue<ViRealSpectrum> mSpectrums;
		QMutex mMutex;

};

#endif
