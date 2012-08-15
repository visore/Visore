#ifndef VIFREQUENCYENDDETECTOR_H
#define VIFREQUENCYENDDETECTOR_H

#include "vienddetector.h"
#include "vispectrum.h"
#include "viaverager.h"
#include "virange.h"
#include <QQueue>

class ViFrequencyEndDetector : public ViEndDetector
{

	Q_OBJECT

	public slots:

		void addSpectrum(ViRealSpectrum spectrums);

	public:

		ViFrequencyEndDetector();

		// rangeThreshold: Range of the spectrum that will be observered
		// valueThreshold: Range the average amplitude should fall in
		// timeThreshold: The number of milliseconds the spectrum should have a 'valueThreshold' average.
		void setThreshold(ViEndDetector::Type type, ViRange rangeThreshold, ViRange valueThreshold, qint64 timeThreshold);

		void initialize();
		void run();

	private:

		bool inRange(QQueue<qreal> &averages, ViRange &range);

		void updateRecordStartAverage(ViRealSpectrum &spectrum);
		void updateRecordEndAverage(ViRealSpectrum &spectrum);
		void updateSongStartAverage(ViRealSpectrum &spectrum);
		void updateSongEndAverage(ViRealSpectrum &spectrum);

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
