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

		void addDir(QString dirName);

	public:

		ViInterpolatorBenchmarker();
		virtual ~ViInterpolatorBenchmarker();

		void benchmark(QString folder = "");

	protected:

		void printFileHeader();
		void printFileData(ViErrorCollection &interpolationErrors, ViErrorCollection &modelErrors, const qint64 &time);
		void printTerminal(ViErrorCollection &interpolationErrors, ViErrorCollection &modelErrors, const qint64 &time);

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
		qreal mBestScore;

		QTime mTime;
		QTime mMainTime;
		int mTotalParamIterations;
		int mDoneParamIterations;

		int mTotalFiles;

		int mQuitCount;

};

#endif
