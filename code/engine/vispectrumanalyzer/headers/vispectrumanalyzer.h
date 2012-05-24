#ifndef VISPECTRUMANALYZER_H
#define VISPECTRUMANALYZER_H

#include "vifouriertransformer.h"
#include "viaudioposition.h"
#include "vipcmconverter.h"
#include "viaudiobuffer.h"
#include "vispectrum.h"
#include "vierror.h"
#include <QThread>

class ViSpectrumAnalyzerThread : public QThread
{
	Q_OBJECT

	public:

		ViSpectrumAnalyzerThread();
		~ViSpectrumAnalyzerThread();
		void setData(ViAudioBuffer *buffer, ViFloatSpectrum *spectrum);
		void setPositions(qint64 start, qint64 end);
		void setBlockSize(qint32 size);
		void setWindowFunction(ViWindowFunction<float> *windowFunction);
		void run();

	private:
		
		int (*pcmToRealPointer)(char*, float*, int);

		ViAudioFormat mFormat;
		ViFloatSpectrum *mSpectrum;
		ViAudioBufferStream *mStream;
		ViWindowFunction<float> *mWindowFunction;
		qint64 mStart;
		QList<qint32> mSizes;
		qint32 mBlockSize;

};

class ViSpectrumAnalyzer : public QObject
{
	Q_OBJECT

	signals:

		void finished();

	public:

		ViSpectrumAnalyzer(ViAudioBuffer *buffer);
		void analyze();
		void analyze(ViAudioPosition start, ViAudioPosition end);
		ViFloatSpectrum& spectrum();

		void setBlockSize(qint32 size);
		qint32 blockSize();

		void setWindowFunction(QString functionName);

	private:

		ViAudioBuffer *mBuffer;
		ViSpectrumAnalyzerThread mThread;
		ViFloatSpectrum mSpectrum;
		qint32 mBlockSize;

};

#endif
