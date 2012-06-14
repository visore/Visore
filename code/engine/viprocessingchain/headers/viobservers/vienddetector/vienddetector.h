#ifndef VIENDDETECTOR_H

#include "viprocessor.h"
#include "viaudioposition.h"
#include <QQueue>

class ViEndDetector : public ViObserver
{
	Q_OBJECT

	signals:

		void endDetected(ViAudioPosition position);

	public:

		ViEndDetector();
		void setThreshold(int milliseconds, qreal value);
		void setMinimumLength(int milliseconds);
		void initialize();
		void run();

	protected:

		void clear();

	private:
		
		bool mDetected;
		qint64 mSampleCounter;
		qreal mTotalValue;

		int mMillisecondThreshold;
		int mMinimumMilliseconds;
		qreal mValue;

		qint64 mSamplesThreshold;
		qint64 mMinimumSamples;
		qreal mValueThreshold;

		QQueue<qreal> mCache;

};

#endif
