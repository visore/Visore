#ifndef VINOISEBATCHER_H
#define VINOISEBATCHER_H

#include <viaudioobject.h>
#include <vinoisedetector.h>
#include <QTime>

class ViNoiseBatcher : public QObject
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

		ViNoiseBatcher();
		virtual ~ViNoiseBatcher();

		void benchmark(QString folder = "");

	protected:

		void printFileHeader(QString filepath);
		void printFileData(ViClassificationErrorCollection &errors, const qint64 &time);
		void printTerminal(ViClassificationErrorCollection &errors, const qint64 &time);

		qreal roundThreshold(const qreal &value);

		QString genre(QString path);

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

		qreal mThreshold;

		QQueue<QString> mSummaryFiles;

};

#endif
