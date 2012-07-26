#ifndef VIAUDIOENGINE_H
#define VIAUDIOENGINE_H

#include "viaudioconnection.h"
#include "visingleton.h"
#include "vimanager.h"

#include "viprocessingchain.h"
#include "visingleexecutor.h"

#include "vispectrumanalyzer.h"
#include "vicorrelator.h"
#include "viwaveformer.h"
#include "vienddetector.h"
#include "visongdetector.h"

class ViAudioEngine : public ViSingleton<ViAudioEngine>
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

		//Correlation
		void calculateCorrelation();

		//Project
		void loadProject(QString filePath);
		void saveProject(QString filePath);
		void startRecordingProject();

	signals:

		//Chain
		void chainChanged();
		void buffering(short progress);

		//Playback
		void positionChanged(ViAudioPosition position);

		//Input
		void inputChanged(ViAudio::Input input);		

		//Song detector
		void songDetected(ViSongInfo info);

		//Spectrum
		void spectrumProgressed(short percentage);
		void spectrumFinished();

		//Correlation
		void correlationProgressed(short percentage);
		void correlationFinished();

		//Project
		void loadProjectStarted();
		void saveProjectStarted();
		void projectFinished();

	public:

		~ViAudioEngine();

		ViRealSpectrum spectrum();
		ViWaveForm& wave(ViAudio::Mode mode);
		ViCorrelationResult& correlation();

	protected:

		friend class ViSingleton<ViAudioEngine>;
		ViAudioEngine();

	private:

		ViAudioConnection mConnection;
		ViFileInput *mFileInput;
		ViFileOutput *mFileOutput;
		ViStreamInput *mStreamInput;
		ViStreamOutput *mStreamOutput;

		ViProcessingChain mProcessingChain;
		ViSingleExecutor mExecutor;

		ViSongDetector mSongDetector;
		ViSpectrumAnalyzer mSpectrumAnalyzer;
		ViCorrelator mCorrelator;
		ViEndDetector mEndDetector;
		ViWaveFormer mInputWaveFormer;
		ViWaveFormer mOutputWaveFormer;

};

#endif
