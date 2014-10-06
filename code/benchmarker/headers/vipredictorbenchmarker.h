#ifndef VIPREDICTORBENCHMARKER_H
#define VIPREDICTORBENCHMARKER_H

#include <viaudioobject.h>
#include <vipredictor.h>
#include <QTime>

class ViPredictorBenchmarker : public QObject
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

		ViPredictorBenchmarker();
		virtual ~ViPredictorBenchmarker();

		void benchmark(QString folder = "");

	protected:

		void printFileHeader();
		void printFileData(ViErrorCollection &predictionErrors, ViError &modelError, const qint64 &time);
		void printTerminal(ViErrorCollection &predictionErrors, ViError &modelError, const qint64 &time);

	private:

		QList<QString> mParamsNames;
		QList<qreal> mParamsStart, mParamsEnd, mParamsIncrease, mParamsCurrent;

		ViPredictor *mPredictor;

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
		qreal mBestScore;

		int mTotalFiles;

};

#endif
