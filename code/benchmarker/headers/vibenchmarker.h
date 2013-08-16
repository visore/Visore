#ifndef VIBENCHMARKER_H
#define VIBENCHMARKER_H

#include <vibuffer.h>
#include <vinoise.h>
#include <viaudiocoder.h>

class ViBenchMarker : public QObject
{

	Q_OBJECT

	private slots:

		void process();
		void quit();

	public:

		void benchmark(QString path);

	private:

		void createNoise(int length);

	private:

		QString mPath;
		ViBuffer mBuffer;
		ViBuffer mBuffer2;
		ViAudioCoder mCoder;
		ViNoise mNoise;

};

#endif
