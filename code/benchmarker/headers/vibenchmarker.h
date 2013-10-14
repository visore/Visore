#ifndef VIBENCHMARKER_H
#define VIBENCHMARKER_H

#include <vibuffer.h>
#include <vinoise.h>
#include <viaudiocoder.h>
#include <viinterpolator.h>
#include <QTime>

class ViBenchMarker : public QObject
{

	Q_OBJECT

	private slots:

		void processNext();
		void process();

		void quit();

	public:

		ViBenchMarker();
		virtual ~ViBenchMarker();

		void benchmark(QString path);
		void benchmark(QStringList paths);

	private:

		void clear();
		void benchmark();
		ViNoise createNoise(int length);

	private:

		struct ViBenchMarkInfo
		{
			QString mFile;
			qreal mAccuracy;
			qreal mDifferences;
			qint64 mCount;
		};

		ViBuffer mBuffer;
		ViBuffer mBuffer2;
		ViAudioCoder mCoder;

		QString mFile;
		QStringList mFiles;
		QStringList mOriginalFiles;
		ViInterpolator *mInterpolator;
		ViNoise mNoise;
		QList< QList<ViBenchMarkInfo> > mInfos;

		QList<ViNoise> mNoises;

		int mCurrentNoiseSize;

		QTime mTime;

};

#endif
