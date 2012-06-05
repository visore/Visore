#ifndef VICORRELATIONRESULT_H
#define VICORRELATIONRESULT_H

#include <QtGlobal>
#include <QString>

class ViCorrelationResultCombination
{

	public:

		ViCorrelationResultCombination(qreal worst = 0, qreal best = 0, qreal average = 0);
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

class ViCorrelationResult
{

	public:

		enum ResultType
		{
			Best = 0,
			Worst = 1,
			Average = 2
		};

	public:

		ViCorrelationResult();
		void reset();
		QString toString();

		void setCrossCorrelation(ViCorrelationResultCombination combination);
		void setSampleCorrelation(ViCorrelationResultCombination combination);

		qreal crossCorrelation(ResultType type);
		qreal sampleCorrelation(ResultType type);

	private:
		
		ViCorrelationResultCombination mSampleCorrelation;
		ViCorrelationResultCombination mCrossCorrelation;
		
};

#endif
