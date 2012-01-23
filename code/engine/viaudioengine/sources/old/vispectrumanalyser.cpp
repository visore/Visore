#include "vispectrumanalyser.h"

#include <qmath.h>
#include <qmetatype.h>
#include <QAudioFormat>
#include <QThread>
#include "vifourierwrapper.h"

ViSpectrumAnalyserThread::ViSpectrumAnalyserThread(QObject *parent)
	: QObject(parent)
#ifndef DISABLE_FFT
	, mFourierTransform(new ViFourierWrapper)
#endif
	, mSamplesNumber(cSpectrumLengthSamples)
	, mWindowFunction(cDefaultWindowFunction)
	, mWindow(cSpectrumLengthSamples, 0.0)
	, mInput(cSpectrumLengthSamples, 0.0)
	, mOutput(cSpectrumLengthSamples, 0.0)
	, mSpectrum(cSpectrumLengthSamples)
#ifdef SPECTRUM_ANALYSER_SEPARATE_THREAD
	, mThread(new QThread(this))
#endif
{
#ifdef SPECTRUM_ANALYSER_SEPARATE_THREAD
	// moveToThread() cannot be called on a QObject with a parent
	setParent(0);
	moveToThread(mThread);
	mThread->start();
#endif
	calculateWindow();
}

ViSpectrumAnalyserThread::~ViSpectrumAnalyserThread()
{
#ifndef DISABLE_FFT
	delete mFourierTransform;
#endif
}

void ViSpectrumAnalyserThread::setWindowFunction(ViWindowFunction type)
{
	mWindowFunction = type;
	calculateWindow();
}

void ViSpectrumAnalyserThread::calculateWindow()
{
	for(int i = 0; i < mSamplesNumber; ++i)
	{
		ViDataType x = 0.0;

		switch(mWindowFunction)
		{
			case NoWindow:
				x = 1.0;
				break;
			case HannWindow:
				x = 0.5 * (1 - qCos((2 * M_PI * i) / (mSamplesNumber - 1)));
				break;
			default:
			Q_ASSERT(false);
		}
		mWindow[i] = x;
	}
}

void ViSpectrumAnalyserThread::calculateSpectrum(const QByteArray &buffer, int inputFrequency, int bytesPerSample)
{
#ifndef DISABLE_FFT
	Q_ASSERT(buffer.size() == mSamplesNumber * bytesPerSample);

	// Initialize data array
	const char *ptr = buffer.constData();
	for(int i = 0; i < mSamplesNumber; ++i)
	{
		const qint16 pcmSample = *reinterpret_cast<const qint16*>(ptr);
		// Scale down to range [-1.0, 1.0]
		const ViDataType realSample = pcmToReal(pcmSample);
		const ViDataType windowedSample = realSample * mWindow[i];
		mInput[i] = windowedSample;
		ptr += bytesPerSample;
	}

	// Calculate the FFT
	mFourierTransform->calculateFFT(mOutput.data(), mInput.data());

	// Analyze output to obtain amplitude and phase for each frequency
	for(int i = 2; i <= mSamplesNumber/2; ++i)
	{
		// Calculate frequency of this complex sample
		mSpectrum[i].mFrequency = qreal(i * inputFrequency) / (mSamplesNumber);

		const qreal real = mOutput[i];
		qreal imag = 0.0;
		if(i>0 && i<mSamplesNumber/2)
		{
			imag = mOutput[mSamplesNumber/2 + i];
		}

		const qreal magnitude = sqrt(real*real + imag*imag);
		qreal amplitude = cSpectrumAnalyserMultiplier * log(magnitude);

		// Bound amplitude to [0.0, 1.0]
		mSpectrum[i].mClipped = (amplitude > 1.0);
		amplitude = qMax(qreal(0.0), amplitude);
		amplitude = qMin(qreal(1.0), amplitude);
		mSpectrum[i].mAmplitude = amplitude;
	}
#endif

	emit calculationComplete(mSpectrum);
}


//=============================================================================
// ViSpectrumAnalyser
//=============================================================================

ViSpectrumAnalyser::ViSpectrumAnalyser(QObject *parent)
	: QObject(parent)
	, mThread(new ViSpectrumAnalyserThread(this))
	, mState(Idle)
#ifdef DUMP_ViSpectrumAnalyser
	, mCount(0)
#endif
{
	ViObject::connect(mThread, SIGNAL(calculationComplete(FrequencySpectrum)), this, SLOT(calculationComplete(FrequencySpectrum)));
}

ViSpectrumAnalyser::~ViSpectrumAnalyser()
{

}

#ifdef DUMP_ViSpectrumAnalyser
void ViSpectrumAnalyser::setOutputPath(const QString &outputDir)
{
	mOutputDir.setPath(outputDir);
	mTextFile.setFileName(mOutputDir.filePath("spectrum.txt"));
	mTextFile.open(QIODevice::WriteOnly | QIODevice::Text);
	mTextStream.setDevice(&mTextFile);
}
#endif

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

void ViSpectrumAnalyser::setWindowFunction(ViWindowFunction type)
{
	const bool b = QMetaObject::invokeMethod(mThread, "setWindowFunction", Qt::AutoConnection, Q_ARG(ViWindowFunction, type));
	Q_ASSERT(b);
	Q_UNUSED(b) // suppress warnings in release builds
}

void ViSpectrumAnalyser::calculate(const QByteArray &buffer, const QAudioFormat &format)
{
	if(isReady())
	{
		Q_ASSERT(isPCMS16LE(format));

		const int bytesPerSample = format.sampleSize() * format.channels() / 8;

#ifdef DUMP_ViSpectrumAnalyser
		mCount++;
		const QString pcmFileName = mOutputDir.filePath(QString("spectrum_%1.pcm").arg(mCount, 4, 10, QChar('0')));
		QFile pcmFile(pcmFileName);
		pcmFile.open(QIODevice::WriteOnly);
		const int bufferLength = mSamplesNumber * bytesPerSample;
		pcmFile.write(buffer, bufferLength);

		mTextStream << "TimeDomain " << mCount << "\n";
		const qint16* input = reinterpret_cast<const qint16*>(buffer);
		for(int i = 0; i < mSamplesNumber; ++i)
		{
			mTextStream << i << "\t" << *input << "\n";
			input += format.channels();
		}
#endif

		mState = Busy;

		// Invoke ViSpectrumAnalyserThread::calculateSpectrum using QMetaObject.  If
		// mThread is in a different thread from the current thread, the
		// calculation will be done in the child thread.
		// Once the calculation is finished, a calculationChanged signal will be
		// emitted by mThread.
		const bool b = QMetaObject::invokeMethod(mThread, "calculateSpectrum", Qt::AutoConnection, Q_ARG(QByteArray, buffer), Q_ARG(int, format.frequency()), Q_ARG(int, bytesPerSample));
		Q_ASSERT(b);
		Q_UNUSED(b) // suppress warnings in release builds

#ifdef DUMP_ViSpectrumAnalyser
		mTextStream << "FrequencySpectrum " << mCount << "\n";
		ViFrequencySpectrum::const_iterator x = mSpectrum.begin();
		for(int i = 0; i < mSamplesNumber; ++i, ++x)
		{
			mTextStream << i << "\t" << x->frequency << "\t" << x->amplitude<< "\t" << x->phase << "\n";
		}
#endif
	}
}

bool ViSpectrumAnalyser::isReady() const
{
	return (Idle == mState);
}

void ViSpectrumAnalyser::cancelCalculation()
{
	if(Busy == mState)
	{
		mState = Cancelled;
	}
}


//-----------------------------------------------------------------------------
// Private slots
//-----------------------------------------------------------------------------

void ViSpectrumAnalyser::calculationComplete(const ViFrequencySpectrum &spectrum)
{
	Q_ASSERT(Idle != mState);
	if(Busy == mState)
	{
		emit spectrumChanged(spectrum);
	}
	mState = Idle;
}
