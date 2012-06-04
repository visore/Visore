#ifndef VIMATCHRESULT_H
#define VIMATCHRESULT_H

#include <QtGlobal>
#include <QString>

class ViMatchResultCombination
{

	public:

		ViMatchResultCombination(qreal worst = 0, qreal best = 0, qreal average = 0);
		void setWorst(qreal worst);
		void setBest(qreal best);
		void setAverage(qreal average);
		qreal worst();
		qreal best();
		qreal average();
		QString toString();

	private:

		qreal mWorst;
		qreal mBest;
		qreal mAverage;

};

class ViMatchResult
{

	public:

		enum ResultType
		{
			Best = 0,
			Worst = 1,
			Average = 2
		};

	public:

		ViMatchResult();
		void reset();
		QString toString();

		void setCrossCorrelation(ViMatchResultCombination combination);
		void setSampleDifference(ViMatchResultCombination combination);

		qreal crossCorrelation(ResultType type);
		qreal sampleDifference(ResultType type);

	private:
		
		ViMatchResultCombination mSampleDifference;
		ViMatchResultCombination mCrossCorrelation;
		
};

#endif
