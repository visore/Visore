#ifndef VIBENCHMARKER_H
#define VIBENCHMARKER_H

#include <vibuffer.h>
#include <vinoise.h>
#include <viaudiocoder.h>
#include <viinterpolator.h>
#include <QTime>
#include<vicrosscorrelator.h>
#include <viproject.h>
#include <viaudioobjectchain.h>

class ViBenchMarker : public QObject
{

	Q_OBJECT

	private slots:

		void loadProject(QString path);
		void processProject();
		void processProject2();
		void process();

		void processTuning();

		void printProgress(qreal progress);
		void quit();

	public:

		ViBenchMarker();
		virtual ~ViBenchMarker();

		void benchmark();

	private:

		void clear();

	private:

		ViBuffer mBuffer;
		ViBuffer mBuffer2;
		ViAudioCoder mCoder;

		QList<ViNoise> mNoises;

		int mCurrentNoiseSize;
		int mCurrentDegree;

		QTime mTime;

		qint64 mTotalTime;

		ViNoiseDetector *mDetector;

		QQueue<QString> mProjects;
		ViProject mProject;
		ViAudioObjectPointer mObject;
		int mCurrentObject;

		ViAudioObjectChain mObjectChain;

		QMap<qreal,qint64> TP, TN, FP, FN;
		qreal mCurrentThreshold;
		qreal mOldProgress;
		qreal mTotalProgress;

		QFile mOutputFile;
		QTextStream mOutputStream;

};

#endif
