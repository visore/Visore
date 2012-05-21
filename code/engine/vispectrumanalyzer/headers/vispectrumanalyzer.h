#ifndef VISPECTRUMANALYZER_H
#define VISPECTRUMANALYZER_H

#include "vifouriertransformer.h"
#include "viaudioposition.h"
#include "vipcmconverter.h"
#include "viaudiobuffer.h"
#include "vifrequencyspectrum.h"
#include "vierror.h"
#include <QThread>

class ViSpectrumAnalyzerThread : public QThread
{
	Q_OBJECT

	public:

		ViSpectrumAnalyzerThread();
		void setData(ViAudioBuffer *buffer, ViFloatFrequencySpectrum *spectrum);
		void setPositions(qint64 start, qint64 end);
		void setBlockSize(qint32 size);
		void run();

	private:
		
		int (*pcmToRealPointer)(char*, float*, int);

		ViAudioFormat mFormat;
		ViFloatFrequencySpectrum *mSpectrum;
		ViAudioBufferStream *mStream;
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
		ViFloatFrequencySpectrum& spectrum();

		void setBlockSize(qint32 size);
		qint32 fftSize();

	private:

		ViAudioBuffer *mBuffer;
		ViSpectrumAnalyzerThread mThread;
		ViFloatFrequencySpectrum mSpectrum;
		qint32 mBlockSize;

};

#endif
