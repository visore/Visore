#ifndef VIENDDETECTOR_H

#include "viprocessor.h"
#include "viaudioposition.h"
#include <QQueue>

class ViEndDetector : public ViObserver
{
	Q_OBJECT

	signals:

		void songStarted(ViAudioPosition position);
		void songEnded(ViAudioPosition position);
		void recordStarted(ViAudioPosition position);
		void recordEnded(ViAudioPosition position);

	public:

		enum Type
		{
			SongStart = 0,
			SongEnd = 1,
			RecordStart = 2,
			RecordEnd = 3
		};

		ViEndDetector();
		void setThreshold(ViEndDetector::Type type, int milliseconds, qreal value);
		void initialize();
		void run();

	protected:

		void setSongStartCache();
		void setSongEndCache();
		void setRecordStartCache();
		void setRecordEndCache();

	private:

		bool mSongStarted;
		bool mRecordStarted;
		
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

#endif
