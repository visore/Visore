#ifndef VIBENCHMARKER3_H
#define VIBENCHMARKER3_H

#include <vibuffer.h>
#include <vinoise.h>
#include <viaudiocoder.h>
#include <viinterpolator.h>
#include <QTime>
#include<vicrosscorrelator.h>
#include <viproject.h>
#include <viaudioobjectchain.h>

class ViBenchMarker3 : public QObject
{

	Q_OBJECT

	private slots:

		void quit();
		QString timeConversion(int msecs);

		void nextFile();
		void process1(bool generate = true);
		void process2();

		void addParam(qreal start, qreal end, qreal increase);
		bool nextParam();
		void initParams();

	public:

		enum Type
		{
			Arma,
			MahalanobisNoiseDetector,
			Mad,
			Fourier,
			Zscore,
			NearestNeighbour
		};

		ViBenchMarker3();
		virtual ~ViBenchMarker3();

		void benchmark();

	protected:

		void printFileHeader();
		void printFileData(const qint64 &time, const qint64 &TP, const qint64 &TN, const qint64 &FP, const qint64 &FN, const QVector<qreal> &lengthTP, const QVector<qreal> &lengthFN);
		void printTerminal(const qint64 &time, const qint64 &TP, const qint64 &TN, const qint64 &FP, const qint64 &FN, const qreal &bestMatthews);

		qreal sensitivity(const qint64 &TP, const qint64 &FN);
		qreal specificity(const qint64 &TN, const qint64 &FP);
		qreal matthews(const qint64 &TP, const qint64 &TN, const qint64 &FP, const qint64 &FN);

	private:

		QList<qreal> mParamsStart, mParamsEnd, mParamsIncrease, mParamsCurrent;

		ViNoiseDetector *mDetector;
		qreal mBestMatthews;

		QQueue<QString> mFiles;
		QQueue<QString> mResults;
		QString mCurrentFile;
		ViAudioObjectPointer mCurrentObject;

		QFile mOutputFile;
		QTextStream mOutputStream;
		qreal mCurrentThreshold;

		QTime mTime;
		QTime mMainTime;
		int mTotalParamIterations;
		int mDoneParamIterations;

		int mTotalFiles;

};

#endif
