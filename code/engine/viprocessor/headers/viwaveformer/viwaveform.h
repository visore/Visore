#ifndef VIWAVEFORM_H
#define VIWAVEFORM_H

#define ZOOM_LEVEL_INCREASE 2
#define MAXIMUM_ZOOM_LEVELS 17
#define FIRST_ZOOM_LEVEL 2
#define UNSIGNED_CHAR_HALF_VALUE 127
#define SUMMARY_CUTOFF 200

#include <QVector>
#include <QMutex>
#include <qmath.h>

class ViWaveForm
{

	public:

		ViWaveForm(qint16 level = 0);
		~ViWaveForm();
		void append(qreal value);
		qint32 size(qint16 level);
		qint64 samples();
		unsigned char maximum(qint32 position, qint16 level);
		unsigned char minimum(qint32 position, qint16 level);
		unsigned char maximumAverage(qint32 position, qint16 level);
		unsigned char minimumAverage(qint32 position, qint16 level);
		unsigned char average(qint32 position, qint16 level);
		void clear();
		bool isUnderCutoff(qint16 level);
		bool isEmpty();

	private:

		void reset();
		void scaleValues(qreal *maximum, qreal *minimum, qreal *averageMaximum, qreal *averageMinimum, qreal *average);
		void appendValues(qreal maximum, qreal minimum, qreal averageMaximum, qreal averageMinimum, qreal average);
		void appendResults();

	private:

		qreal mMaximum;
		qreal mMinimum;
		qreal mAverageMaximum;
		qreal mAverageMinimum;
		qreal mAverage;
		qint64 mMaximumCounter;
		qint64 mMinimumCounter;
		qint32 mTotalCounter;
		ViWaveForm *mNextLevel;
		qint16 mLevel;
		bool mIsUnderCutoff;

		QMutex mMutex;

		QVector<unsigned char> mMaximums;
		QVector<unsigned char> mMinimums;
		QVector<unsigned char> mAverageMaximums;
		QVector<unsigned char> mAverageMinimums;
		QVector<unsigned char> mAverages;

};

#endif
