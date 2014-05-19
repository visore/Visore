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

		void addParam(qreal start, qreal end, qreal increase);
		bool nextParam();
		void initParams();

	public:

		ViPredictorBenchmarker();
		virtual ~ViPredictorBenchmarker();

		void benchmark();

	protected:

		void printFileHeader();

	private:

		QList<qreal> mParamsStart, mParamsEnd, mParamsIncrease, mParamsCurrent;

		ViPredictor *mPredictor;

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
