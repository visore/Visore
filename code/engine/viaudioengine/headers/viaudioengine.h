#ifndef VIAUDIOENGINE_H
#define VIAUDIOENGINE_H

#include "viaudioconnection.h"
#include "visingleton.h"
#include "vimanager.h"

#include "viprocessingchain.h"
#include "visingleexecutor.h"

#include "vienmfpidentifier.h"

#include "vispectrumanalyzer.h"
#include "viwaveformer.h"
#include "vienddetector.h"
#include "visongdetector.h"

#include "vicrosscorrelator.h"
#include "visamplecorrelator.h"

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
		void setPosition(int seconds);

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
		void startProject(QString name, QString filePath, ViAudioFormat format, short recordSides, ViAudio::Type type, bool existingProject);

	signals:

		//General
		void progressStarted();
		void progress(short progress);
		void progressFinished();
		void statusChanged(QString status);

		//Chain
		void chainChanged();
		void buffering(short progress);

		//Playback
		void positionChanged(ViAudioPosition position);
		void lengthChanged(ViAudioPosition length);

		//Input
		void inputChanged(ViAudio::Input input);		

		//Song detector
		void songDetected(ViSongInfo info);

		//Song start/end
		void recordStarted();
		void recordEnded();
		void songStarted();
		void songEnded();

		//Spectrum
		void spectrumChanged(ViRealSpectrum spectrum, qint64 milliseconds);
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

		ViWaveForm& wave(ViAudio::Mode mode);
		ViCorrelation& correlation();

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

		ViEnmfpIdentifier mSongIdentifier;
		ViSongDetector mSongDetector;

		ViCrossCorrelator mCrossCorrelator;
		ViSampleCorrelator mSampleCorrelator;

		ViSpectrumAnalyzer mSpectrumAnalyzer;
		ViEndDetector *mEndDetector;
		ViWaveFormer mInputWaveFormer;
		ViWaveFormer mOutputWaveFormer;

};

#endif
