#ifndef VIAUDIOENGINE_H
#define VIAUDIOENGINE_H

#include "viaudioconnection.h"
#include "visingleton.h"

#include "viprocessingchain.h"
#include "visingleexecutor.h"

#include "vispectrumanalyzer.h"
#include "viwaveformer.h"
#include "visongdetector.h"

class ViAudioEngine : public ViSingleton
{
    Q_OBJECT

	public slots:

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

		void chainChanged();

		void inputChanged(ViAudioConnection::Type type);		

		void spectrumProgressed(short percentage);
		void spectrumFinished();

	public:

		~ViAudioEngine();
		static ViAudioEngine* instance();

		ViRealSpectrum spectrum();
		ViWaveForm& wave(ViAudioConnection::Direction direction);

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
