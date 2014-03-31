#ifndef ViBenchMarker2_H
#define ViBenchMarker2_H

#include <vibuffer.h>
#include <vinoise.h>
#include <viaudiocoder.h>
#include <viinterpolator.h>
#include <QTime>
#include<vicrosscorrelator.h>
#include <viproject.h>
#include <viaudioobjectchain.h>

class ViBenchMarker2 : public QObject
{

	Q_OBJECT

	private slots:

        void progress(qreal i);

		void quit();

		void nextFile();
		void process1();
		void process2();

		void generateNoise();
		int addNoise(ViSampleChunk &s, int offset);
		void addNoise1(ViSampleChunk &s, int offset, int length);
		void addNoise2(ViSampleChunk &s, int offset, int length);
		void addNoise3(ViSampleChunk &s, int offset, int length);

	public:

		ViBenchMarker2();
		virtual ~ViBenchMarker2();

		void benchmark();

	private:

		ViNoiseDetector *mDetector;

		QQueue<QString> mFiles;
		QString mCurrentFile;
		ViAudioObjectPointer mCurrentObject;

		ViSampleChunk mNoise1;
		ViSampleChunk mNoise2;
		int mNoiseType;
		int mNoiseLength;
		int mNoiseCount;

		QFile mOutputFile;
		QTextStream mOutputStream;
		qreal mCurrentThreshold;

		QTime time;

};

#endif
