#ifndef VIMATCHRESULT_H
#define VIMATCHRESULT_H

#include <QtGlobal>
#include <QString>

class ViMatchResultCombination
{

	public:

		ViMatchResultCombination(qreal minimum = 0, qreal maximum = 0, qreal average = 0);
		void setMinimum(qreal minimum);
		void setMaximum(qreal maximum);
		void setAverage(qreal average);
		qreal minimum();
		qreal maximum();
		qreal average();
		QString toString();

	private:

		qreal mMinimum;
		qreal mMaximum;
		qreal mAverage;

};

class ViMatchResult
{

	public:

		enum ResultType
		{
			Maximum = 0,
			Minimum = 1,
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
