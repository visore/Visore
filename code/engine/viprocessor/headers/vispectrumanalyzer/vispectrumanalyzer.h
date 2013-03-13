#ifndef VISPECTRUMANALYZER_H
#define VISPECTRUMANALYZER_H

#include <vispectrum.h>
#include <viprocessor.h>
#include <vifouriertransformer.h>

class ViSpectrumAnalyzer : public ViProcessor
{

	Q_OBJECT

	signals:

		void changed(ViRealSpectrum spectrum, qint64 milliseconds);

	public:

		ViSpectrumAnalyzer();
		~ViSpectrumAnalyzer();

		void setWindowFunction(QString functionName);
		void setInterval(int milliseconds);

		void initialize();
		void execute();
		void finalize();

	private:

		int mWindowSize;
		int mHalfWindowSize;
		int mInterval;
		int mMilliseconds;
		ViFourierTransformer mTransformer;
		ViRealSpectrum mSpectrum;
		qreal *mBuffer;
		int mBufferSize;
		int mSampleCounter;
		qint64 mCurrentPosition;

};

#endif