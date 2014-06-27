#ifndef VINOISEDETECTORBENCHMARKER_H
#define VINOISEDETECTORBENCHMARKER_H

#include <viaudioobject.h>
#include <vinoisedetector.h>
#include <QTime>

class ViNoiseDetectorBenchmarker : public QObject
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

		void progressDetect(qreal percentage);
		void progressThreshold();
		void clearProgress();

		void addDir(QString dirName);

	public:

		ViNoiseDetectorBenchmarker();
		virtual ~ViNoiseDetectorBenchmarker();

		void benchmark(QString folder = "");

		void calculateThreshold(ViBuffer *noise, ViBuffer *size, qreal &bestThreshold, ViClassificationErrorCollection &bestErrors, qreal &bestMatthews, const qreal &maxNoise);

	protected:

		void printFileHeader();
		void printFileData(ViClassificationErrorCollection &errors, const qint64 &time, const qreal &threshold);
		void printTerminal(ViClassificationErrorCollection &errors, const qint64 &time, const qreal &threshold, const qreal &maxMatthews);

		qreal roundThreshold(const qreal &value);

	private:

		QList<QString> mParamsNames;
		QList<qreal> mParamsStart, mParamsEnd, mParamsIncrease, mParamsCurrent;

		ViNoiseDetector *mDetector;

		QQueue<QString> mFiles;
		QString mCurrentFile;
		ViAudioObjectPointer mCurrentObject;

		QQueue<QString> mResults;
		QFile mOutputFile;
		QTextStream mOutputStream;
		QQueue<QString> mResults2;
		QFile mOutputFile2;
		QTextStream mOutputStream2;

		QTime mTime;
		QTime mMainTime;
		int mTotalParamIterations;
		int mDoneParamIterations;

		int mTotalFiles;

};

#endif
