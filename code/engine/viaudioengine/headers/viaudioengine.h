#ifndef VIAUDIOENGINE_H
#define VIAUDIOENGINE_H

#include "viaudioconnection.h"
#include "visingleton.h"

#include "viprocessingchain.h"
#include "visingleexecutor.h"

#include "vispectrumanalyzer.h"
#include "vicorrelator.h"
#include "viwaveformer.h"
#include "visongdetector.h"

class ViAudioEngine : public ViSingleton
{
    Q_OBJECT

	public slots:

		//Input
		void changeInput(ViAudio::Input input);

		//Playback
		void startPlayback();
		void stopPlayback();
		void pausePlayback();

		//Recording
		void startRecording();
		void stopRecording();
		void pauseRecording();

		//File I/O
		void openFile(QString filePath);
		void saveFile(QString filePath);
		
		//Spectrum
		void calculateSpectrum(qint32 size, QString windowFunction);

	signals:

		//Chain
		void chainChanged();

		//Playback
		void positionChanged(ViAudioPosition position);

		//Input
		void inputChanged(ViAudio::Input input);		

		//Spectrum
		void spectrumProgressed(short percentage);
		void spectrumFinished();

		//Correlation
		void correlationProgressed(short percentage);
		void correlationFinished();

	public:

		~ViAudioEngine();
		static ViAudioEngine* instance();

		ViRealSpectrum spectrum();
		ViWaveForm& wave(ViAudio::Mode mode);

	protected:

		ViAudioEngine();

	private:

		static ViAudioEngine *mEngine;

		ViAudioConnection mConnection;
		ViFileInput *mFileInput;
		ViFileOutput *mFileOutput;
		ViStreamInput *mStreamInput;
		ViStreamOutput *mStreamOutput;

		ViProcessingChain mProcessingChain;
		ViSingleExecutor mExecutor;

		ViSpectrumAnalyzer mSpectrumAnalyzer;
		ViWaveFormer mInputWaveFormer;
		ViWaveFormer mOutputWaveFormer;

};

#endif
