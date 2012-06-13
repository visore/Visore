#ifndef VIENDDETECTOR_H

#include "viprocessor.h"
#include <QQueue>

class ViEndDetector : public ViObserver
{
	Q_OBJECT

	signals:

		void endDetected();

	public:

		ViEndDetector();
		void setThreshold(int milliseconds, qreal value);
		void initialize();
		void run();

	protected:

		void clear();

	private:
		
		qreal mConfidence;
		qreal mTotalValue;
		int mMillisecondThreshold;
		int mSamplesThreshold;
		int mValueThreshold;
		QQueue<qreal> mCache;

};

#endif
