#ifndef VIBENCHMARKER4_H
#define VIBENCHMARKER4_H

#include <vibuffer.h>
#include <vinoise.h>
#include <viaudiocoder.h>
#include <viinterpolator.h>
#include <QTime>
#include<vicrosscorrelator.h>
#include <viproject.h>
#include <viaudioobjectchain.h>

class ViBenchMarker4 : public QObject
{

	Q_OBJECT

	private slots:

		void quit();
		QString timeConversion(int msecs);

		void nextFile();
		void process1(bool generate = true);
		void process2();

		void generateNoise();

		bool nextParam();
		void initParams();

		void write(ViSampleChunk &data1, ViSampleChunk &data2, QString path);

	public:

		ViBenchMarker4();
		virtual ~ViBenchMarker4();

		void benchmark();

	private:

		QList<qreal> mParamsStart, mParamsEnd, mParamsIncrease, mParamsCurrent;

		ViInterpolator *mInterpolator;

		QQueue<QString> mFiles;
		QQueue<QString> mResults;
		QString mCurrentFile;
		ViAudioObjectPointer mCurrentObject;

		ViSampleChunk mNoise1;
		ViSampleChunk mNoise2;
		int mNoiseLength;
		int mNoiseCount;

		QFile mOutputFile;
		QTextStream mOutputStream;
		qreal mCurrentThreshold;

		int mNoChange;
		QTime mTime;

		QTime mMainTime;
		int mTotalParamIterations;
		int mDoneParamIterations;

		int mTotalFiles;

};

#endif
