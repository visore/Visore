#ifndef VIINTERPOLATORBENCHMARKER_H
#define VIINTERPOLATORBENCHMARKER_H

#include <viaudioobject.h>
#include <viinterpolator.h>
#include <QTime>

class ViInterpolatorBenchmarker : public QObject
{

	Q_OBJECT

	private slots:

		void quit();
		QString timeConversion(int msecs);

		void nextFile();
		void process();

		void addParam(QString name, qreal start, qreal end, qreal increase);
		bool nextParam();
		void initParams();

		void progress(qreal percentage);

	public:

		ViInterpolatorBenchmarker();
		virtual ~ViInterpolatorBenchmarker();

		void benchmark();

	protected:

		void printFileHeader();
		void printFileData(const QVector<qreal> &rmse, const qreal &averageRmse, const qint64 &time);
		void printTerminal(const QVector<qreal> &rmse, const qreal &averageRmse, const qint64 &time);

	private:

		QList<QString> mParamsNames;
		QList<qreal> mParamsStart, mParamsEnd, mParamsIncrease, mParamsCurrent;

		ViInterpolator *mInterpolator;

		QQueue<QString> mFiles;
		QString mCurrentFile;
		ViAudioObjectPointer mCurrentObject;

		QQueue<QString> mResults;
		QFile mOutputFile;
		QTextStream mOutputStream;
		QQueue<QString> mResults2;
		QFile mOutputFile2;
		QTextStream mOutputStream2;
		qreal mCurrentThreshold;

		QTime mTime;
		QTime mMainTime;
		int mTotalParamIterations;
		int mDoneParamIterations;

		int mTotalFiles;

};

#endif
