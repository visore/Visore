/*
This is an adaption of the Qt Spectrum demo.
*/

#ifndef VISPECTRUMANALYSER_H
#define VISPECTRUMANALYSER_H

#include <QByteArray>
#include <QObject>
#include <QVector>
#include <QDir>
#include <QFile>
#include <QTextStream>

#include "vifrequencyspectrum.h"
#include "vispectrum.h"
#include "viessentials.h"
#include "FFTRealFixLenParam.h"

using namespace ffft;

QT_FORWARD_DECLARE_CLASS(QAudioFormat)
QT_FORWARD_DECLARE_CLASS(QThread)

class ViFourierWrapper;
class ViSpectrumAnalyserThreadPrivate;

class ViSpectrumAnalyserThread : public QObject
{
	Q_OBJECT
	
	public:
		ViSpectrumAnalyserThread(QObject *parent);
		~ViSpectrumAnalyserThread();

	public slots:
		void setWindowFunction(ViWindowFunction type);
		void calculateSpectrum(const QByteArray &buffer, int inputFrequency, int bytesPerSample);

	signals:
		void calculationComplete(const ViFrequencySpectrum &spectrum);

	private:
		void calculateWindow();

	private:
		#ifndef DISABLE_FFT
			ViFourierWrapper *mFourierTransform;
		#endif
		const int mSamplesNumber;

		ViWindowFunction mWindowFunction;

		#ifdef DISABLE_FFT
			typedef qreal ViDataType;
		#else
			typedef FFTRealFixLenParam::DataType ViDataType;
		#endif

		QVector<ViDataType> mWindow;
		QVector<ViDataType> mInput;
		QVector<ViDataType> mOutput;

		ViFrequencySpectrum mSpectrum;

		#ifdef SPECTRUM_ANALYSER_SEPARATE_THREAD
			QThread* mThread;
		#endif
};

class ViSpectrumAnalyser : public QObject
{
	Q_OBJECT
	
	public:
		ViSpectrumAnalyser(QObject *parent = 0);
		~ViSpectrumAnalyser();

		#ifdef DUMP_SPECTRUMANALYSER
			void setOutputPath(const QString &outputPath);
		#endif

	public:
		//Set the windowing function which is applied before calculating the FFT
		void setWindowFunction(ViWindowFunction type);

		/*
		 * Calculate a frequency spectrum
		 *
		 * \param buffer       Audio data
		 * \param format       Format of audio data
		 *
		 * Frequency spectrum is calculated asynchronously.  The result is returned
		 * via the spectrumChanged signal.
		 *
		 * An ongoing calculation can be cancelled by calling cancelCalculation().
		 *
		 */
		void calculate(const QByteArray &buffer, const QAudioFormat &format);

		//Check whether the object is ready to perform another calculation
		bool isReady() const;

		/*
		 * Cancel an ongoing calculation
		 *
		 * Note that cancelling is asynchronous.
		 */
		void cancelCalculation();

	signals:
		void spectrumChanged(const ViFrequencySpectrum &spectrum);

	private slots:
		void calculationComplete(const ViFrequencySpectrum &spectrum);

	private:
		void calculateWindow();

	private:
		ViSpectrumAnalyserThread* mThread;

		enum ViProcessState
		{
			Idle,
			Busy,
			Cancelled
		};

		ViProcessState mState;

		#ifdef DUMP_SPECTRUMANALYSER
			QDir mOutputDir;
			int mCount;
			QFile mTextFile;
			QTextStream mTextStream;
		#endif
};

#endif

