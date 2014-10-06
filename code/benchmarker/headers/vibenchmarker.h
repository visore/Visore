#ifndef ViBenchmarker_H
#define ViBenchmarker_H

#include <viaudioobject.h>
#include <viinterpolator.h>
#include <QTime>

class ViBenchmarker : public QObject
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

		ViBenchmarker();
		virtual ~ViBenchmarker();

		void benchmark(QString folder = "");

	protected:

		void printFileHeader(QString filepath);
		void printFileDataAll(QString filepath, ViErrorCollection &interpolationErrors, ViErrorCollection &modelErrors, const qint64 &time);
		void printFileDataMinified(QString filepath, ViErrorCollection &interpolationErrors, ViErrorCollection &modelErrors, const qint64 &time);
		void printFileDataSummary(QString genre, ViErrorCollection &interpolationErrors, ViErrorCollection &modelErrors, const qint64 &time);
		void printTerminal(ViErrorCollection &interpolationErrors, ViErrorCollection &modelErrors, const qint64 &time);

		QString genre(QString path);

	private:

		QList<QString> mParamsNames;
		QList<qreal> mParamsStart, mParamsEnd, mParamsIncrease, mParamsCurrent;

		ViInterpolator *mInterpolator;

		QQueue<QString> mFiles;
		QQueue<QString> mSummaryFiles;
		QString mCurrentFile;

		QString mAllFile;
		QString mMiniFile;

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
