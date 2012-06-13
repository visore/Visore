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
		void initialize();
		void run();

	protected:

		void clear();

	private:
		
		bool mDetected;
		qint64 mSampleCounter;
		qreal mTotalValue;
		qreal mValueThreshold;
		int mMillisecondThreshold;
		int mSamplesThreshold;
		QQueue<qreal> mCache;

};

#endif
