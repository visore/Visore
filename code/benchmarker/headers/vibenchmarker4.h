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

		void nextFile();
		void process1();
		void process2();

        void generateNoise();
        int addNoise(ViSampleChunk &s, int offset);
        void addNoise1(ViSampleChunk &s, int offset, int length);
        void addNoise2(ViSampleChunk &s, int offset, int length);
        void addNoise3(ViSampleChunk &s, int offset, int length);

	public:

		ViBenchMarker4();
		virtual ~ViBenchMarker4();

		void benchmark();

	private:

        QString TYPE;

		QQueue<QString> mFiles;
		QString mCurrentFile;
		ViAudioObjectPointer mCurrentObject;

		QFile mOutputFile;
		QTextStream mOutputStream;
		qreal mCurrentThreshold;

        ViSampleChunk mNoise1;
        ViSampleChunk mNoise2;
        int mNoiseType;
        int mNoiseLength;
        int mNoiseCount;

};

#endif
