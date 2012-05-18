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
		void setData(ViAudioBuffer *buffer, ViFrequencySpectrum *spectrum);
		void setPositions(qint64 start, qint64 end);
		void run();

	private:
		
		int (*pcmToRealPointer)(char*, float*, int);

		ViFourierTransformer mTransformer;
		ViAudioFormat mFormat;
		ViFrequencySpectrum *mSpectrum;
		ViAudioBufferStream *mStream;
		qint64 mStart;
		QList<qint32> mSizes;

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

	private:

		ViAudioBuffer *mBuffer;
		ViSpectrumAnalyzerThread mThread;
		ViFrequencySpectrum mSpectrum;
};

#endif
