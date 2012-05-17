#ifndef VICOMPARERESULT_H
#define VICOMPARERESULT_H

#include <QtGlobal>
#include <QString>

class ViCompareResult
{
	public:

		ViCompareResult();

		void setNumberOfSamples(qint64 numberOfSamples);
		void setNumberOfMatchingSamples(qint64 numberOfMatchingSamples);
		void setNumberOfNoneMatchingSamples(qint64 numberOfNoneMatchingSamples);
		void setStandardDeviation(qreal standardDeviation);
		void setAverageDifference(qreal averageDifference);
		void setMaximumDifference(qreal maximumDifference);
		void setMinimumDifference(qreal minimumDifference);

		qint64 numberOfSamples();
		qint64 numberOfMatchingSamples();
		qint64 numberOfNoneMatchingSamples();
		qreal standardDeviation();
		qreal averageDifference();
		qreal maximumDifference();
		qreal minimumDifference();

		void reset();
		QString toString();

	private:

		qint64 mNumberOfSamples;
		qint64 mNumberOfMatchingSamples;
		qint64 mNumberOfNoneMatchingSamples;

		qreal mStandardDeviation;
		qreal mAverageDifference;
		qreal mMaximumDifference;
		qreal mMinimumDifference;
	
};

#endif
