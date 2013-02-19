/*#ifndef VISAMPLEENDDETECTOR_H
#define VISAMPLEENDDETECTOR_H

#include "vienddetector.h"
#include <QQueue>

class ViSampleEndDetector : public ViSegmentDetector
{

	public:

		ViSampleEndDetector();
		void setThreshold(ViSegmentDetector::Type type, int milliseconds, qreal value);
		void initialize();
		void execute();

	protected:

		void setSongStartCache();
		void setSongEndCache();
		void setRecordStartCache();
		void setRecordEndCache();

	private:
		
		int mSongStartTime;
		int mSongEndTime;
		int mRecordStartTime;
		int mRecordEndTime;
		
		qreal mSongStartValue;
		qreal mSongEndValue;
		qreal mRecordStartValue;
		qreal mRecordEndValue;

		int mSongStartSampleThreshold;
		int mSongEndSampleThreshold;
		int mRecordStartSampleThreshold;
		int mRecordEndSampleThreshold;

		qreal mSongStartValueThreshold;
		qreal mSongEndValueThreshold;
		qreal mRecordStartValueThreshold;
		qreal mRecordEndValueThreshold;

		QQueue<qreal> mSongCache;
		QQueue<qreal> mRecordCache;

		qreal mTotalSongValue;
		qreal mTotalRecordValue;
		qint64 mSampleCounter;

};

#endif*/
